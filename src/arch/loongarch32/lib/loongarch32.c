/* SPDX-License-Identifier: MIT */
/*
 * Copyright(c) 2021 Sanpe <sanpeqf@gmail.com>
 */

#include <xboot.h>
#include <loongarch32.h>

void proc_idle(void)
{
    // asm volatile ("stop");
    asm volatile (".word 0x80000000");
}

void proc_halt(void)
{
    local_irq_disable();
    for (;;)
        proc_idle();
}