/*
 * init/main.c
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
#include <init.h>

static void init_task(struct task_t * task, void * data)
{
	/* Do initial vfs */
	do_init_vfs();
	// asm(".word 0x80000005");

	/* Do initial calls */
	do_initcalls();
	asm(".word 0x80000006");

	/* Do initial setting */
	do_init_setting();
	asm(".word 0x80000007");

	/* Do show logo */
	do_show_logo();
	asm(".word 0x80000008");

	/* Do play audio */
	do_play_audio();
	asm(".word 0x80000009");

	/* Do auto mount */
	do_auto_mount();
	asm(".word 0x8000000a");

	/* Do idle task */
	do_idle_task();
	asm(".word 0x8000000b");

	/* Do shell task */
	do_shell_task();
	asm(".word 0x8000000c");

	/* Do auto boot */
	do_auto_boot();
	asm(".word 0x8000000d");
}

void xboot_main(void)
{
	/* Do initial memory */
	do_init_mem();

	/* Do initial scheduler */
	do_init_sched();

	/* Create init task */
	task_create(scheduler_self(), "init", NULL, NULL, init_task, NULL, 0, 0);

	void *p = malloc(1);

	Log("in main: %p", p);

	free(p);

	Log("in main: free done.");

	/* Scheduler loop */
	// scheduler_loop();

	init_task(NULL, NULL);
}
