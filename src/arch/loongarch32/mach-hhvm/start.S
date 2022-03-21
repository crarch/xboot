/* SPDX-License-Identifier: MIT */
/*
 * Copyright(c) 2021 Sanpe <sanpeqf@gmail.com>
 */

#include <linkage.h>
#include <entry.h>

    .section .vector, "ax"
ENTRY(_start)
    pcaddu12i $ra, 0x0
    addi.w $ra, $ra, 0x14
    la $sp, __stack_end
    b init
END(_start)

    .section .startup.text, "ax"
ENTRY(init)
    b xboot_main
    b halt
END(init)

ENTRY(halt)
    .word 0x80000000
END(halt)
