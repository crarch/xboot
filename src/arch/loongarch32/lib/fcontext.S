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

    /* save sp copy */
    move    $t0, $sp

    /* save registers */
    st.w    $s0, $sp, 0
    addi.w  $sp, $sp, 4
    st.w    $s1, $sp, 0
    addi.w  $sp, $sp, 4
    st.w    $s2, $sp, 0
    addi.w  $sp, $sp, 4
    st.w    $s3, $sp, 0
    addi.w  $sp, $sp, 4
    st.w    $s4, $sp, 0
    addi.w  $sp, $sp, 4
    st.w    $s5, $sp, 0
    addi.w  $sp, $sp, 4
    st.w    $s6, $sp, 0
    addi.w  $sp, $sp, 4
    st.w    $s7, $sp, 0
    addi.w  $sp, $sp, 4
    st.w    $s8, $sp, 0
    addi.w  $sp, $sp, 4
    st.w    $ra, $sp, 0

    /*
    fst.s   $f0, $sp, 0
    addi.w  $sp, $sp, 4
    fst.s   $f1, $sp, 0
    addi.w  $sp, $sp, 4
    fst.s   $f2, $sp, 0
    addi.w  $sp, $sp, 4
    fst.s   $f3, $sp, 0
    addi.w  $sp, $sp, 4
    fst.s   $f4, $sp, 0
    addi.w  $sp, $sp, 4
    fst.s   $f5, $sp, 0
    addi.w  $sp, $sp, 4
    fst.s   $f6, $sp, 0
    addi.w  $sp, $sp, 4
    fst.s   $f7, $sp, 0
    addi.w  $sp, $sp, 4
    fst.s   $f8, $sp, 0
    addi.w  $sp, $sp, 4
    fst.s   $f9, $sp, 0
    addi.w  $sp, $sp, 4
    fst.s   $f10, $sp, 0
    addi.w  $sp, $sp, 4
    fst.s   $f11, $sp, 0
    addi.w  $sp, $sp, 4
    fst.s   $f12, $sp, 0
    addi.w  $sp, $sp, 4
    fst.s   $f13, $sp, 0
    addi.w  $sp, $sp, 4
    fst.s   $f14, $sp, 0
    addi.w  $sp, $sp, 4
    fst.s   $f15, $sp, 0
    addi.w  $sp, $sp, 4
    fst.s   $f16, $sp, 0
    addi.w  $sp, $sp, 4
    fst.s   $f17, $sp, 0
    addi.w  $sp, $sp, 4
    fst.s   $f18, $sp, 0
    addi.w  $sp, $sp, 4
    fst.s   $f19, $sp, 0
    addi.w  $sp, $sp, 4
    fst.s   $f20, $sp, 0
    addi.w  $sp, $sp, 4
    fst.s   $f21, $sp, 0
    addi.w  $sp, $sp, 4
    fst.s   $f22, $sp, 0
    addi.w  $sp, $sp, 4
    fst.s   $f23, $sp, 0
    addi.w  $sp, $sp, 4
    fst.s   $f24, $sp, 0
    addi.w  $sp, $sp, 4
    fst.s   $f25, $sp, 0
    addi.w  $sp, $sp, 4
    fst.s   $f26, $sp, 0
    addi.w  $sp, $sp, 4
    fst.s   $f27, $sp, 0
    addi.w  $sp, $sp, 4
    fst.s   $f28, $sp, 0
    addi.w  $sp, $sp, 4
    fst.s   $f29, $sp, 0
    addi.w  $sp, $sp, 4
    fst.s   $f30, $sp, 0
    addi.w  $sp, $sp, 4
    fst.s   $f31, $sp, 0
    addi.w  $sp, $sp, 4
    */

    /* reload sp */
    move    $sp, $t0

    st.w    $ra, $sp, 4*(FCONTEXT_MAX-1)

    move    $a2, $sp

    move    $sp, $a0

    move    $t0, $sp

    /* restore registers */
    ld.w    $s0, $sp, 0
    addi.w  $sp, $sp, 4
    ld.w    $s1, $sp, 0
    addi.w  $sp, $sp, 4
    ld.w    $s2, $sp, 0
    addi.w  $sp, $sp, 4
    ld.w    $s3, $sp, 0
    addi.w  $sp, $sp, 4
    ld.w    $s4, $sp, 0
    addi.w  $sp, $sp, 4
    ld.w    $s5, $sp, 0
    addi.w  $sp, $sp, 4
    ld.w    $s6, $sp, 0
    addi.w  $sp, $sp, 4
    ld.w    $s7, $sp, 0
    addi.w  $sp, $sp, 4
    ld.w    $s8, $sp, 0
    addi.w  $sp, $sp, 4
    ld.w    $ra, $sp, 0

    /*
    fld.s   $f0, $sp, 0
    addi.w  $sp, $sp, 4
    fld.s   $f1, $sp, 0
    addi.w  $sp, $sp, 4
    fld.s   $f2, $sp, 0
    addi.w  $sp, $sp, 4
    fld.s   $f3, $sp, 0
    addi.w  $sp, $sp, 4
    fld.s   $f4, $sp, 0
    addi.w  $sp, $sp, 4
    fld.s   $f5, $sp, 0
    addi.w  $sp, $sp, 4
    fld.s   $f6, $sp, 0
    addi.w  $sp, $sp, 4
    fld.s   $f7, $sp, 0
    addi.w  $sp, $sp, 4
    fld.s   $f8, $sp, 0
    addi.w  $sp, $sp, 4
    fld.s   $f9, $sp, 0
    addi.w  $sp, $sp, 4
    fld.s   $f10, $sp, 0
    addi.w  $sp, $sp, 4
    fld.s   $f11, $sp, 0
    addi.w  $sp, $sp, 4
    fld.s   $f12, $sp, 0
    addi.w  $sp, $sp, 4
    fld.s   $f13, $sp, 0
    addi.w  $sp, $sp, 4
    fld.s   $f14, $sp, 0
    addi.w  $sp, $sp, 4
    fld.s   $f15, $sp, 0
    addi.w  $sp, $sp, 4
    fld.s   $f16, $sp, 0
    addi.w  $sp, $sp, 4
    fld.s   $f17, $sp, 0
    addi.w  $sp, $sp, 4
    fld.s   $f18, $sp, 0
    addi.w  $sp, $sp, 4
    fld.s   $f19, $sp, 0
    addi.w  $sp, $sp, 4
    fld.s   $f20, $sp, 0
    addi.w  $sp, $sp, 4
    fld.s   $f21, $sp, 0
    addi.w  $sp, $sp, 4
    fld.s   $f22, $sp, 0
    addi.w  $sp, $sp, 4
    fld.s   $f23, $sp, 0
    addi.w  $sp, $sp, 4
    fld.s   $f24, $sp, 0
    addi.w  $sp, $sp, 4
    fld.s   $f25, $sp, 0
    addi.w  $sp, $sp, 4
    fld.s   $f26, $sp, 0
    addi.w  $sp, $sp, 4
    fld.s   $f27, $sp, 0
    addi.w  $sp, $sp, 4
    fld.s   $f28, $sp, 0
    addi.w  $sp, $sp, 4
    fld.s   $f29, $sp, 0
    addi.w  $sp, $sp, 4
    fld.s   $f30, $sp, 0
    addi.w  $sp, $sp, 4
    fld.s   $f31, $sp, 0
    addi.w  $sp, $sp, 4
    */

    move    $sp, $t0

    /* load pc */
    ld.w    $a2, $sp, 4*(FCONTEXT_MAX-1)

    /* restore stack */
    addi.w  $sp, $sp, (FCONTEXT_MAX * 4)

    /* return transfer_t from jump */
    move $a0, $a2

    jr $a2
END(jump_fcontext)
