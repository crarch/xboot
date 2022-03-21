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
extern void * make_fcontext(
    void * stack,                       // $a0
    size_t size,                        // $a1
    void (*func)(struct transfer_t)     // $a2
);
extern struct transfer_t jump_fcontext(
    void * fctx,                        // $a0
    void * priv                         // $a1
);
*/

ENTRY(make_fcontext)
    addi.w  $a0, $a0, (FCONTEXT_MAX * 4)
    st.w $a2, $a0, ((FCONTEXT_MAX-1)*4)
    jr $ra
END(make_fcontext)


/**
 * jump to a context: 
 */
ENTRY(jump_fcontext)
    
    /* prepare stack */
    addi.w  $sp, $sp, -(FCONTEXT_MAX * 4)

    /* save registers */
    st.w    $ra, $sp, 4*0
    //st.w    $tp, $sp, 4*1
    //st.w    $sp, $sp, 4*2
    //st.w    $a0, $sp, 4*3
    //st.w    $a1, $sp, 4*4
    //st.w    $a2, $sp, 4*5
    //st.w    $a3, $sp, 4*6
    //st.w    $a4, $sp, 4*7
    //st.w    $a5, $sp, 4*8
    //st.w    $a6, $sp, 4*9
    //st.w    $a7, $sp, 4*10
    //st.w    $x,  $sp, 4*11
    //st.w    $fp, $sp, 4*12
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

    st.w    $ra, $sp, 4*23

    move  $a2, $sp

    move  $sp, $a0

    /* restore registers */
    ld.w    $ra, $sp, 4*0
    //ld.w    $tp, $sp, 4*1
    //ld.w    $sp, $sp, 4*2
    //ld.w    $a0, $sp, 4*3
    //ld.w    $a1, $sp, 4*4
    //ld.w    $a2, $sp, 4*5
    //ld.w    $a3, $sp, 4*6
    //ld.w    $a4, $sp, 4*7
    //ld.w    $a5, $sp, 4*8
    //ld.w    $a6, $sp, 4*9
    //ld.w    $a7, $sp, 4*10
    //ld.w    $x,  $sp, 4*11
    //ld.w    $fp, $sp, 4*12
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

    /* load pc */
    ld.w    $a2, $sp, 4*23

    /* restore stack */
    addi.w  $sp, $sp, (FCONTEXT_MAX * 4)

    /* return transfer_t from jump */
    move $a0, $a2

    jr $a2
END(jump_fcontext)
