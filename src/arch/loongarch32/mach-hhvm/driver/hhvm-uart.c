/* SPDX-License-Identifier: MIT */
/*
 * Copyright(c) 2021 Sanpe <sanpeqf@gmail.com>
 */

#include <xboot.h>
#include <hhvm-uart.h>
#include <uart/uart.h>

struct hhvm_device {
    virtual_addr_t base;
    int baud;
    char *clk;
};

static inline uint32_t
hhvm_read(struct hhvm_device *gdev, unsigned int reg)
{
    return read32(gdev->base + reg);
}

static inline void
hhvm_write(struct hhvm_device *gdev, unsigned int reg, uint32_t val)
{
    write32(gdev->base + reg, val);
}

static ssize_t uart_hhvm_get(struct uart_t *uart, int *baud, int *data, int *parity, int *stop)
{
    struct hhvm_device *gdev = uart->priv;

    *baud = gdev->baud;
    *data = 8;
    *parity = 0;
    *stop = 1;

    return 1;
}

static ssize_t uart_hhvm_set(struct uart_t *uart, int baud, int data, int parity, int stop)
{
    return 1;
}

static ssize_t uart_hhvm_read(struct uart_t *uart, u8_t *buf, size_t count)
{
    // struct hhvm_device *gdev = uart->priv;
    size_t xfer;

    // for (xfer = 0; xfer <= count; ++xfer) {
    //     if (!(hhvm_read(gdev, hhvm_UART_STA) & hhvm_UART_STA_DR))
    //         break;
    //     *buf++ = hhvm_read(gdev, HHVM_UART_DAT) & 0xff;
    // }

    // return xfer;
    // ignore read
    for (xfer = 0; xfer <= count; ++xfer) *buf++ = '\0';
    return xfer;
}

static ssize_t uart_hhvm_write(struct uart_t *uart, const u8_t *buf, size_t count)
{
    struct hhvm_device *gdev = uart->priv;
    size_t xfer;

    for (xfer = 0; xfer < count; ++xfer) {
        hhvm_write(gdev, 0, *buf++);
    }

    return xfer;
}

static struct device_t *uart_hhvm_probe(struct driver_t *drv, struct dtnode_t *node)
{
    struct hhvm_device *gdev;
    struct device_t *dev;
    struct uart_t *uart;
    uint32_t baud;
    
    gdev = malloc(sizeof(*gdev));
    if (!gdev)
        return NULL;

    uart = malloc(sizeof(struct uart_t));
    if (!uart) {
        free(gdev);
        return NULL;
    }

    gdev->base = phys_to_virt(dt_read_address(node));
    gdev->clk = "";

    uart->name = alloc_device_name(dt_read_name(node), -1);
    uart->set = uart_hhvm_set;
    uart->get = uart_hhvm_get;
    uart->read = uart_hhvm_read;
    uart->write = uart_hhvm_write;
    uart->priv = gdev;

    baud = dt_read_int(node, "baud-rates", 115200);
    uart_hhvm_set(uart, baud, 8, 0, 1);

    if (!(dev = register_uart(uart, drv))) {
        free(gdev->clk);
        free_device_name(uart->name);
        free(uart->priv);
        free(uart);
        return NULL;
    }

    return dev;
}

static void uart_hhvm_remove(struct device_t *dev)
{
    struct uart_t *uart = dev->priv;
    struct hhvm_device *gdev = uart->priv;

    if (uart) {
        unregister_uart(uart);
        // clk_disable(gdev->clk);
        free(gdev->clk);
        free_device_name(uart->name);
        free(uart->priv);
        free(uart);
    }
}

static void uart_hhvm_suspend(struct device_t *dev) {}
static void uart_hhvm_resume(struct device_t *dev) {}

static struct driver_t uart_hhvm = {
    .name = "uart-hhvm",
    .probe = uart_hhvm_probe,
    .remove = uart_hhvm_remove,
    .suspend = uart_hhvm_suspend,
    .resume = uart_hhvm_resume,
};

static __init void uart_hhvm_driver_init(void)
{
    STOP_CPU;
    register_driver(&uart_hhvm);
}

static __exit void uart_hhvm_driver_exit(void)
{
    unregister_driver(&uart_hhvm);
}

driver_initcall(uart_hhvm_driver_init);
driver_exitcall(uart_hhvm_driver_exit);
