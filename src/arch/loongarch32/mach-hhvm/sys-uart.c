/* SPDX-License-Identifier: MIT */
/*
 * Copyright(c) 2021 Sanpe <sanpeqf@gmail.com>
 */

#include <hhvm-uart.h>

void __startup sys_uart_putc(char ch)
{
    // while (!(read32(SER_BASE + GX6605S_UART_STA) & GX6605S_UART_STA_THRE));
    write8(UART_BASE + 0x00, ch);
    // STOP_CPU;
}

void __startup sys_uart_print(const char *str)
{
    while (*str) {
        if (*str == '\n')
            sys_uart_putc('\r');
        sys_uart_putc(*str++);
    }
}

void __startup sys_uart_init(uint32_t apb, uint32_t freq)
{
    // uint32_t clkdiv, val;

    // val = read32(GCTL_BASE + GX6605S_PINMUX_PORTA);
    // val &= ~(BIT(16) | BIT(17));
    // write32(GCTL_BASE + GX6605S_PINMUX_PORTA, val);
    // val = read32(GCTL_BASE + GX6605S_PINMUX_PORTB);
    // val &= ~(BIT(9) | BIT(10));
    // write32(GCTL_BASE + GX6605S_PINMUX_PORTB, val);

    // clkdiv = apb / ((16 * freq) / 100);
    // if (clkdiv % 100 > 50)
    //     clkdiv = clkdiv / 100;
    // else
    //     clkdiv = clkdiv / 100 - 1;

    // write32(SER_BASE + GX6605S_UART_CLK, clkdiv);
    // write32(SER_BASE + GX6605S_UART_CTL, 0x600);
}
