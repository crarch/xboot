/*
 * kernel/core/initcall.c
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
#include <xboot/initcall.h>

extern initcall_t __initcall_start[];
extern initcall_t __initcall_end[];
extern exitcall_t __exitcall_start[];
extern exitcall_t __exitcall_end[];

void my_out(char *str) {
	while (str && *str) write8(0xa00003f8, *(str++));
	write8(0xa00003f8, '\n');
}

void do_initcalls(void)
{
	initcall_t * call;

	call =  &(*__initcall_start);
	// asm(".word 0x80000010");
	while(call < &(*__initcall_end))
	{
		// asm(".word 0x80000020");
		// if ((size_t)*call & 0x10000000) asm(".word 0x80000050");
		// if ((size_t)call & 0x10000000) asm(".word 0x80000050");
		char buf[32] = "";
		sprintf(buf, "0x%08x", (size_t)*call);
		my_out(buf);
		(*call)();
		// asm(".word 0x80000030");
		call++;
	}
	asm(".word 0x80000040");
}

void do_exitcalls(void)
{
	exitcall_t * call;

	call =  &(*__exitcall_start);
	while(call < &(*__exitcall_end))
	{
		(*call)();
		call++;
	}
}
