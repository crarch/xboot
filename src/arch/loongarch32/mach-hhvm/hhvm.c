/* SPDX-License-Identifier: MIT */
/*
 * Copyright(c) 2021 Sanpe <sanpeqf@gmail.com>
 */

#include <xboot.h>
#include <hhvm.h>

static int mach_detect(struct machine_t * mach)
{
	STOP_CPU;
	machine_mmap(mach, "sseg1", 0x1c000000, 0x00000000, 0xf000000, 0);
	return 1;
}

static void mach_smpinit(struct machine_t * mach)
{
}

static void mach_smpboot(struct machine_t * mach, void (*func)(void))
{
}

static void mach_shutdown(struct machine_t * mach)
{
}

static void mach_reboot(struct machine_t * mach)
{
}

static void mach_sleep(struct machine_t * mach)
{
}

static void mach_cleanup(struct machine_t * mach)
{
}

// static void mach_logger(struct machine_t * mach, const char * buf, int count)
void mach_logger(struct machine_t * mach, const char * buf, int count)
{
	while(count--)
		sys_uart_putc(*buf++);
	sys_uart_putc('\0');
}

static const char * mach_uniqueid(struct machine_t * mach)
{
	return NULL;
}

static int mach_keygen(struct machine_t * mach, const char * msg, void * key)
{
	return 0;
}

static struct machine_t hhvm = {
	.name		= "hhvm",
	.desc		= "HHVM Development Platform",
	.detect 	= mach_detect,
	.smpinit	= mach_smpinit,
	.smpboot	= mach_smpboot,
	.shutdown	= mach_shutdown,
	.reboot		= mach_reboot,
	.sleep		= mach_sleep,
	.cleanup	= mach_cleanup,
	.logger		= mach_logger,
	.uniqueid	= mach_uniqueid,
	.keygen		= mach_keygen,
};

static __init void hhvm_machine_init(void)
{
	register_machine(&hhvm);
	// STOP_CPU;
}

static __exit void hhvm_machine_exit(void)
{
	unregister_machine(&hhvm);
}

machine_initcall(hhvm_machine_init);
machine_exitcall(hhvm_machine_exit);
