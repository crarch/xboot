/* SPDX-License-Identifier: MIT */
/*
 * Copyright(c) 2021 Sanpe <sanpeqf@gmail.com>
 */

#include <linkage.h>
#include <fcontext.h>

ENTRY(make_fcontext)
    addi.w  $t1, $r0, (FCONTEXT_MAX * 4)
    sub.w   $a0, $sp, $t1
    st.w    $a2, $a0, (FCONTEXT_LR * 4)
    jr $ra
END(make_fcontext)

ENTRY(jump_fcontext)
    /* save registers */
    addi.w  $t1, $r0, (FCONTEXT_MAX * 4)
    sub.w   $sp, $sp, $t1

    st.w    $ra, $a0, 4*0
    st.w    $tp, $a0, 4*1
    st.w    $sp, $a0, 4*2
    st.w    $a0, $a0, 4*3
    st.w    $a1, $a0, 4*4
    st.w    $a2, $a0, 4*5
    st.w    $a3, $a0, 4*6
    st.w    $a4, $a0, 4*7
    st.w    $a5, $a0, 4*8
    st.w    $a6, $a0, 4*9
    st.w    $a7, $a0, 4*10
    st.w    $x,  $a0, 4*11
    st.w    $fp, $a0, 4*12
    st.w    $s0, $a0, 4*13
    st.w    $s1, $a0, 4*14
    st.w    $s2, $a0, 4*15
    st.w    $s3, $a0, 4*16
    st.w    $s4, $a0, 4*17
    st.w    $s5, $a0, 4*18
    st.w    $s6, $a0, 4*19
    st.w    $s7, $a0, 4*20
    st.w    $s8, $a0, 4*21

    /* switch stack */
    addi.w  $a2, $sp, 0
    addi.w  $sp, $a0, 0

    /* restore registers */
    ld.w    $ra, $a0, 4*0
    ld.w    $tp, $a0, 4*1
    ld.w    $sp, $a0, 4*2
    ld.w    $a0, $a0, 4*3
    ld.w    $a1, $a0, 4*4
    ld.w    $a2, $a0, 4*5
    ld.w    $a3, $a0, 4*6
    ld.w    $a4, $a0, 4*7
    ld.w    $a5, $a0, 4*8
    ld.w    $a6, $a0, 4*9
    ld.w    $a7, $a0, 4*10
    ld.w    $x,  $a0, 4*11
    ld.w    $fp, $a0, 4*12
    ld.w    $s0, $a0, 4*13
    ld.w    $s1, $a0, 4*14
    ld.w    $s2, $a0, 4*15
    ld.w    $s3, $a0, 4*16
    ld.w    $s4, $a0, 4*17
    ld.w    $s5, $a0, 4*18
    ld.w    $s6, $a0, 4*19
    ld.w    $s7, $a0, 4*20
    ld.w    $s8, $a0, 4*21
    
    addi.w  $sp, $r0, (FCONTEXT_MAX * 4)

    /*
     * pass transfer_t as first arg in context function
     * a0 == FCTX, a1 == PRIV (not changed)
     */
    addi.w   $a0, $a2, 0

    jr $ra
END(jump_fcontext)
