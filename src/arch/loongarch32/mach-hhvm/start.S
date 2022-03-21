/* SPDX-License-Identifier: MIT */
/*
 * Copyright(c) 2021 Sanpe <sanpeqf@gmail.com>
 */

#include <linkage.h>
#include <entry.h>

ENTRY(_start)

    addi.w $r0, $r2, 1
    
END(_start)

ENTRY(init)
    
END(init)

ENTRY(halt)
    .word 0x80000000
END(halt)

    .section    .text, "ax"
