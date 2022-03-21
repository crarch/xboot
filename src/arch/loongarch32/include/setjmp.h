/* SPDX-License-Identifier: MIT */
/*
 * Copyright(c) 2021 Sanpe <sanpeqf@gmail.com>
 */

#ifndef __LOONGARCH32_SETJMP_H__
#define __LOONGARCH32_SETJMP_H__

#define JMP_BUF_MAX 32

#ifndef __ASSEMBLY__

#ifdef __cplusplus
extern "C" {
#endif

struct __jmp_buf {
    unsigned long __jmp_buf[JMP_BUF_MAX];
};

typedef struct __jmp_buf jmp_buf[1];

int setjmp(jmp_buf);
void longjmp(jmp_buf, int);

#ifdef __cplusplus
}
#endif

#endif  /* __ASSEMBLY__ */
#endif 	/* __LOONGARCH32_SETJMP_H__ */
