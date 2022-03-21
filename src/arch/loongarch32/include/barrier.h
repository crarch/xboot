/* SPDX-License-Identifier: MIT */
/*
 * Copyright(c) 2021 Sanpe <sanpeqf@gmail.com>
 */

#ifndef __LOONGARCH32_BARRIER_H__
#define __LOONGARCH32_BARRIER_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Read and write memory barrier */
#define mb()
/* Read memory barrier */
#define rmb()		mb()
/* Write memory barrier */
#define wmb()		mb()

/* SMP read and write memory barrier */
#define smp_mb()	mb()
/* SMP read memory barrier */
#define smp_rmb()	rmb()
/* SMP write memory barrier */
#define smp_wmb()	wmb()

#ifdef __cplusplus
}
#endif

#endif /* __LOONGARCH32_BARRIER_H__ */
