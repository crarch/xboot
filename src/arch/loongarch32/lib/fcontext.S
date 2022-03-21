/* SPDX-License-Identifier: MIT */
/*
 * Copyright(c) 2021 Sanpe <sanpeqf@gmail.com>
 */

#include <linkage.h>
#include <fcontext.h>

/*
struct transfer_t
{
    void * fctx;
    void * priv;
};
extern struct transfer_t jump_fcontext(void * fctx, void * priv);
*/

ENTRY(make_fcontext)
    .word 0x80000000
    /* shift address in a0 (allocated stack) to lower 16 byte boundary */
    /* andi.w  $a0, $a0, ~0xf */
    andi  $a0, $a0, 0xff0

    /* reserve space for context-data on context-stack */
    addi.w $a0, $a0, -(FCONTEXT_MAX * 4)

    /* third arg of make_fcontext() == address of context-function */
    /* store address as a pc to jump in */
    st.w $a2, $a0, ((FCONTEXT_MAX-1)*4)

    /* save address of finish as return-address for context-function */
    /* will be entered after context-function returns (ra register) */
    la.local $a4, finish
    st.w $a4, $a0, ((FCONTEXT_MAX-2)*4)
    /* return pointer to context-data (a0) */
    jr $ra
finish:
    /* exit code is zero */
    addi.w $a0, $r0, 0
    /* exit application */
    /*
        auipc x6, offset[32:12]
        jalr x0, x6, offset[11:0]
    */
    /* tail exit */
    /* la.local $a6, $ */
    b exit
END(make_fcontext)


/**
 * make a jump: 
 */
ENTRY(jump_fcontext)
    
    /* prepare stack */
    addi.w  $sp, $sp, -(FCONTEXT_MAX * 4)

    /* save registers */
    st.w    $ra, $sp, 4*0
    st.w    $tp, $sp, 4*1
    st.w    $sp, $sp, 4*2
    st.w    $a0, $sp, 4*3
    st.w    $a1, $sp, 4*4
    st.w    $a2, $sp, 4*5
    st.w    $a3, $sp, 4*6
    st.w    $a4, $sp, 4*7
    st.w    $a5, $sp, 4*8
    st.w    $a6, $sp, 4*9
    st.w    $a7, $sp, 4*10
    st.w    $x,  $sp, 4*11
    st.w    $fp, $sp, 4*12
    st.w    $s0, $sp, 4*13
    st.w    $s1, $sp, 4*14
    st.w    $s2, $sp, 4*15
    st.w    $s3, $sp, 4*16
    st.w    $s4, $sp, 4*17
    st.w    $s5, $sp, 4*18
    st.w    $s6, $sp, 4*19
    st.w    $s7, $sp, 4*20
    st.w    $s8, $sp, 4*21
    st.w    $ra, $sp, 4*22

    /* save ra as pc */
    st.w    $ra, $sp, 4*23

    /* store sp (pointing to context-data) in a2 */
    addi.w  $a2, $sp, 0

    /* restore sp (pointing to context-data) from a0 */
    addi.w  $sp, $a0, 0

    /* restore registers */
    ld.w    $ra, $sp, 4*0
    ld.w    $tp, $sp, 4*1
    ld.w    $sp, $sp, 4*2
    ld.w    $sp, $sp, 4*3
    ld.w    $a1, $sp, 4*4
    ld.w    $a2, $sp, 4*5
    ld.w    $a3, $sp, 4*6
    ld.w    $a4, $sp, 4*7
    ld.w    $a5, $sp, 4*8
    ld.w    $a6, $sp, 4*9
    ld.w    $a7, $sp, 4*10
    ld.w    $x,  $sp, 4*11
    ld.w    $fp, $sp, 4*12
    ld.w    $s0, $sp, 4*13
    ld.w    $s1, $sp, 4*14
    ld.w    $s2, $sp, 4*15
    ld.w    $s3, $sp, 4*16
    ld.w    $s4, $sp, 4*17
    ld.w    $s5, $sp, 4*18
    ld.w    $s6, $sp, 4*19
    ld.w    $s7, $sp, 4*20
    ld.w    $s8, $sp, 4*21
    ld.w    $ra, $sp, 4*22

    /* return transfer_t from jump */
    /* pass transfer_t as first arg in context function */
    addi.w  $a0, $a2, 0

    /* load pc */
    ld.w    $a2, $sp, 4*23

    /* restore stack */
    addi.w  $sp, $r0, (FCONTEXT_MAX * 4)

    jr $a2
END(jump_fcontext)
