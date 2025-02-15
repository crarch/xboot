/*
 * kernel/core/task.c
 *
 * Copyright(c) 2007-2022 Jianjun Jiang <8192542@qq.com>
 * Official site: http://xboot.org
 * Mobile phone: +86-18665388956
 * QQ: 8192542
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include <xboot.h>
#include <xboot/task.h>

struct transfer_t
{
	void * fctx;
	void * priv;
};

extern void * make_fcontext(void * stack, size_t size, void (*func)(struct transfer_t));
extern struct transfer_t jump_fcontext(void * fctx, void * priv);

struct scheduler_t __sched[CONFIG_MAX_SMP_CPUS];
EXPORT_SYMBOL(__sched);

static const uint32_t nice_to_weight[40] = {
 /* -20 */     88761,     71755,     56483,     46273,     36291,
 /* -15 */     29154,     23254,     18705,     14949,     11916,
 /* -10 */      9548,      7620,      6100,      4904,      3906,
 /*  -5 */      3121,      2501,      1991,      1586,      1277,
 /*   0 */      1024,       820,       655,       526,       423,
 /*   5 */       335,       272,       215,       172,       137,
 /*  10 */       110,        87,        70,        56,        45,
 /*  15 */        36,        29,        23,        18,        15,
};

static const uint32_t nice_to_wmult[40] = {
 /* -20 */     48388,     59856,     76040,     92818,    118348,
 /* -15 */    147320,    184698,    229616,    287308,    360437,
 /* -10 */    449829,    563644,    704093,    875809,   1099582,
 /*  -5 */   1376151,   1717300,   2157191,   2708050,   3363326,
 /*   0 */   4194304,   5237765,   6557202,   8165337,  10153587,
 /*   5 */  12820798,  15790321,  19976592,  24970740,  31350126,
 /*  10 */  39045157,  49367440,  61356676,  76695844,  95443717,
 /*  15 */ 119304647, 148102320, 186737708, 238609294, 286331153,
};

static inline uint64_t mul_u32_u32(uint32_t a, uint32_t b)
{
	return (uint64_t)a * b;
}

static inline uint64_t mul_u64_u32_shr(uint64_t a, uint32_t mul, unsigned int shift)
{
	uint32_t ah, al;
	uint64_t ret;

	al = a;
	ah = a >> 32;

	ret = mul_u32_u32(al, mul) >> shift;
	if(ah)
		ret += mul_u32_u32(ah, mul) << (32 - shift);
	return ret;
}

static inline uint64_t calc_delta(struct task_t * task, uint64_t delta)
{
	uint64_t fact = 1024;
	int shift = 32;

	if(unlikely(fact >> 32))
	{
		while(fact >> 32)
		{
			fact >>= 1;
			shift--;
		}
	}
	fact = (uint64_t)(uint32_t)fact * nice_to_wmult[task->dynice];

	while(fact >> 32)
	{
		fact >>= 1;
		shift--;
	}
	return mul_u64_u32_shr(delta, fact, shift);
}

static inline uint64_t calc_delta_fair(struct task_t * task, uint64_t delta)
{
	if(unlikely(task->dynice != 20))
		delta = calc_delta(task, delta);
	return delta;
}

static inline struct task_t * scheduler_next_ready_task(struct scheduler_t * sched)
{
	struct rb_node * leftmost = rb_first_cached(&sched->ready);

	if(!leftmost)
		return NULL;
	return rb_entry(leftmost, struct task_t, node);
}

static inline void scheduler_enqueue_task(struct scheduler_t * sched, struct task_t * task)
{
	struct rb_node ** link = &sched->ready.rb_root.rb_node;
	struct rb_node * parent = NULL;
	struct task_t * next, * entry;
	int leftmost = 1;

	while(*link)
	{
		parent = *link;
		entry = rb_entry(parent, struct task_t, node);
		if((int64_t)(task->vtime - entry->vtime) < 0)
		{
			link = &parent->rb_left;
		}
		else
		{
			link = &parent->rb_right;
			leftmost = 0;
		}
	}

	rb_link_node(&task->node, parent, link);
	rb_insert_color_cached(&task->node, &sched->ready, leftmost);
	next = scheduler_next_ready_task(sched);
	if(likely(next))
		sched->min_vtime = next->vtime;
	else if(sched->running)
		sched->min_vtime = sched->running->vtime;
	else
		sched->min_vtime = 0;
}

static inline void scheduler_dequeue_task(struct scheduler_t * sched, struct task_t * task)
{
	struct task_t * next;

	rb_erase_cached(&task->node, &sched->ready);
	next = scheduler_next_ready_task(sched);
	if(likely(next))
		sched->min_vtime = next->vtime;
	else if(sched->running)
		sched->min_vtime = sched->running->vtime;
	else
		sched->min_vtime = 0;
}

static inline void scheduler_switch_task(struct scheduler_t * sched, struct task_t * task)
{
	struct task_t * running = sched->running;
	sched->running = task;
	// asm(".word 0x80000005");
	Log("Jump to %08x", task->fctx);
	struct transfer_t from = jump_fcontext(task->fctx, running);
	// asm(".word 0x80000000");
	struct task_t * t = (struct task_t *)from.priv;
	t->fctx = from.fctx;
}

static inline struct scheduler_t * scheduler_load_balance_choice(void)
{
	struct scheduler_t * sched = &__sched[0];
	uint64_t weight = ~0ULL;
	int i;

	for(i = 0; i < CONFIG_MAX_SMP_CPUS; i++)
	{
		if(__sched[i].weight < weight)
		{
			sched = &__sched[i];
			weight = __sched[i].weight;
		}
	}
	return sched;
}

static void fcontext_entry(struct transfer_t from)
{
	struct task_t * t = (struct task_t *)from.priv;
	struct scheduler_t * sched = t->sched;
	struct task_t * task = sched->running;

	t->fctx = from.fctx;
	task->func(task, task->data);
	uint32_t weight = nice_to_weight[task->nice];
	if(task->__stdin)
		__file_free(task->__stdin);
	if(task->__stdout)
		__file_free(task->__stdout);
	if(task->__stderr)
		__file_free(task->__stderr);
	if(task->name)
		free(task->name);
	if(task->fb)
		free(task->fb);
	if(task->input)
		free(task->input);
	if(task->stack)
		free(task->stack);
	free(task);

	spin_lock(&sched->lock);
	sched->weight -= weight;
	struct task_t * next = scheduler_next_ready_task(sched);
	if(likely(next))
	{
		scheduler_dequeue_task(sched, next);
		next->start = ktime_to_ns(ktime_get());
		spin_unlock(&sched->lock);
		scheduler_switch_task(sched, next);
		STOP_MACHINE;
	}
	else
	{
		spin_unlock(&sched->lock);
	}
}

struct task_t * task_create(struct scheduler_t * sched, const char * name, const char * fb, const char * input, task_func_t func, void * data, size_t stksz, int nice)
{
	struct task_t * self = task_self();
	struct task_t * task;
	void * stack;

	if(!func)
		return NULL;

	if(!sched)
		sched = scheduler_load_balance_choice();

	if(stksz <= 0)
		stksz = CONFIG_TASK_STACK_SIZE;

	if(nice < -20)
		nice = 0;
	else if(nice > 19)
		nice = 39;
	else
		nice += 20;

	task = malloc(sizeof(struct task_t));
	if(!task)
		return NULL;

	stack = malloc(stksz);
	Log("get task stack = %08x", stack);
	if(!stack)
	{
		free(task);
		return NULL;
	}

	RB_CLEAR_NODE(&task->node);
	task->name = strdup(name);
	task->fb = strdup(fb);
	task->input = strdup(input);
	task->start = ktime_to_ns(ktime_get());
	task->vtime = 0;
	task->sched = sched;
	task->stack = stack;
	task->stksz = stksz;
	task->nice = nice;
	task->dynice = nice;
	task->fctx = make_fcontext(task->stack + stksz, task->stksz, fcontext_entry);
	task->func = func;
	task->data = data;

	if(self)
		task->__con = self->__con;
	else
		task->__con = NULL;
	task->__stdin = NULL;
	task->__stdout = NULL;
	task->__stderr = NULL;
	task->__errno = 0;

	spin_lock(&sched->lock);
	task->vtime = sched->min_vtime;
	sched->weight += nice_to_weight[nice];
	scheduler_enqueue_task(sched, task);
	spin_unlock(&sched->lock);

	Log("task created: %s, fctx: %08x, stack: [%08x, %08x]", task->name, task->fctx, task->stack, task->stack + task->stksz);

	return task;
}

void task_console(struct task_t * task, struct console_t * con)
{
	if(task)
	{
		if(task->__stdin)
		{
			__file_free(task->__stdin);
			task->__stdin = NULL;
		}
		if(task->__stdout)
		{
			__file_free(task->__stdout);
			task->__stdout = NULL;
		}
		if(task->__stderr)
		{
			__file_free(task->__stderr);
			task->__stderr = NULL;
		}
		task->__con = con;
	}
}

void task_nice(struct task_t * task, int nice)
{
	if(task)
	{
		if(nice < -20)
			nice = 0;
		else if(nice > 19)
			nice = 39;
		else
			nice += 20;

		if(task->nice != nice)
		{
			spin_lock(&task->sched->lock);
			task->sched->weight -= nice_to_weight[task->nice];
			task->sched->weight += nice_to_weight[nice];
			task->nice = nice;
			task->dynice = nice;
			spin_unlock(&task->sched->lock);
		}
	}
}

void task_yield(void)
{
	// Log("task_yield");
	struct scheduler_t * sched = scheduler_self();
	struct task_t * self = task_self();
	uint64_t now = ktime_to_ns(ktime_get());

	self->vtime += calc_delta_fair(self, now - self->start);
	if((int64_t)(self->vtime - sched->min_vtime) < 0)
	{
		self->start = now;
	}
	else
	{
		spin_lock(&sched->lock);
		scheduler_enqueue_task(sched, self);
		struct task_t * next = scheduler_next_ready_task(sched);
		if(likely(next))
		{
			scheduler_dequeue_task(sched, next);
			next->start = now;
			spin_unlock(&sched->lock);
			if(likely(next != self))
				scheduler_switch_task(sched, next);
		}
		else
		{
			spin_unlock(&sched->lock);
		}
	}
}

static void secondary_idle_task(struct task_t * task, void * data)
{
	while(1)
	{
		task_yield();
	}
}

static void smpboot_entry(void)
{
	machine_smpinit();

	struct scheduler_t * sched = scheduler_self();
	task_create(sched, "idle", NULL, NULL, secondary_idle_task, (int[]){smp_processor_id()}, SZ_8K, 19);

	spin_lock(&sched->lock);
	struct task_t * next = scheduler_next_ready_task(sched);
	if(likely(next))
	{
		sched->running = next;
		scheduler_dequeue_task(sched, next);
		next->start = ktime_to_ns(ktime_get());
		spin_unlock(&sched->lock);
		scheduler_switch_task(sched, next);
	}
	else
	{
		spin_unlock(&sched->lock);
	}
}

static void primary_idle_task(struct task_t * task, void * data)
{
	machine_smpboot(smpboot_entry);
	while(1)
	{
		task_yield();
	}
}

void do_idle_task(void)
{
	struct scheduler_t * sched = scheduler_self();
	task_create(sched, "idle", NULL, NULL, primary_idle_task, (int[]){smp_processor_id()}, SZ_8K, 19);
}

void do_init_sched(void)
{
	for(int i = 0; i < CONFIG_MAX_SMP_CPUS; i++)
	{
		struct scheduler_t * sched = &__sched[i];
		spin_lock_init(&sched->lock);
		spin_lock(&sched->lock);
		sched->ready = RB_ROOT_CACHED;
		sched->running = NULL;
		sched->min_vtime = 0;
		sched->weight = 0;
		spin_unlock(&sched->lock);
	}
}

void scheduler_loop(void)
{
	struct scheduler_t * sched = scheduler_self();
	spin_lock(&sched->lock);
	struct task_t * next = scheduler_next_ready_task(sched);
	// Log("got next: %p %s", next, next->name);
	// void *p = malloc(0x40);
	// Log("test malloc: %p", p);
	// free(p);
	if(likely(next))
	{
		sched->running = next;
		scheduler_dequeue_task(sched, next);
		next->start = ktime_to_ns(ktime_get());
		spin_unlock(&sched->lock);
		scheduler_switch_task(sched, next);
		STOP_MACHINE;
	}
	else
	{
		spin_unlock(&sched->lock);
	}
}
