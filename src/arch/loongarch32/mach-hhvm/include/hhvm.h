#include <io.h>
#include <stdint.h>

#define __startup       __attribute__((__section__(".startup.text")))
#define __startup_data  __attribute__((__section__(".startup.data")))
#define STOP_CPU asm(".word 0x80000000")

#define DDR_BASE      ((virtual_addr_t)0x1c000000)
#define DEVICE_BASE   ((virtual_addr_t)0xa0000000)

#define UART_BASE   (DEVICE_BASE + 0x03f8)

#define MSIZE 0xf000000

#define IMAGE_SIZE ((uint32_t)&_ld_image_end - (uint32_t)&_ld_image_start)

extern __startup void sys_uart_putc(char ch);