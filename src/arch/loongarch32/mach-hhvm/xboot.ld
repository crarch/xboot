/* SPDX-License-Identifier: MIT */
/*
 * Copyright(c) 2021 Sanpe <1512819690@qq.com>
 */

OUTPUT_ARCH(loongarch32)
ENTRY(_start)

STACK_EXC_SIZE  = 0x10000;
STACK_USR_SIZE  = 0x40000;
HEAP_SIZE       = 0x3000000;
DRAM_START      = 0x1c000000;

MEMORY {
    DRAM (rwx): ORIGIN = DRAM_START, LENGTH = 0xf000000
}

SECTIONS {
    _ld_image_start = .;

    .vector : {
        KEEP(*(.vector));
    } > DRAM

    .stext ALIGN(8) : {
        *(.startup.text)
        *(.startup.text*)
    } > DRAM

    .sdata ALIGN(8) : {
        *(.startup.data)
        *(.startup.data*)
    } > DRAM

    __preload_end = .;

    .text ALIGN(8) : {
        *(.text)
        *(.text*)
    } > DRAM

    .initcall ALIGN(8) : {
        PROVIDE(__initcall_start = .);
        KEEP(*(.initcall_0.text))
        KEEP(*(.initcall_1.text))
        KEEP(*(.initcall_2.text))
        KEEP(*(.initcall_3.text))
        KEEP(*(.initcall_4.text))
        KEEP(*(.initcall_5.text))
        KEEP(*(.initcall_6.text))
        KEEP(*(.initcall_7.text))
        KEEP(*(.initcall_8.text))
        KEEP(*(.initcall_9.text))
        PROVIDE(__initcall_end = .);
    } > DRAM

    .exitcall ALIGN(8) : {
        PROVIDE(__exitcall_start = .);
        KEEP(*(.exitcall_9.text))
        KEEP(*(.exitcall_8.text))
        KEEP(*(.exitcall_7.text))
        KEEP(*(.exitcall_6.text))
        KEEP(*(.exitcall_5.text))
        KEEP(*(.exitcall_4.text))
        KEEP(*(.exitcall_3.text))
        KEEP(*(.exitcall_2.text))
        KEEP(*(.exitcall_1.text))
        KEEP(*(.exitcall_0.text))
        PROVIDE(__exitcall_end = .);
    } > DRAM

    .ksymtab ALIGN(16) : {
        PROVIDE(__ksymtab_start = .);
        KEEP(*(.ksymtab.text))
        PROVIDE(__ksymtab_end = .);
    } > DRAM

    .romdisk ALIGN(8) : {
        PROVIDE(__romdisk_start = .);
        KEEP(*(.romdisk))
        PROVIDE(__romdisk_end = .);
    } > DRAM

    .rodata ALIGN(8) : {
        *(.rodata)
        *(.rodata*)
    } > DRAM

    .data ALIGN(8) : {
        *(.data)
        *(.data*)
    } > DRAM

    _ld_image_end = .;

    .bss ALIGN(8) (NOLOAD) : {
        PROVIDE(__bss_start = .);
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(8);
        PROVIDE(__bss_end = .);
    } > DRAM

    .stack ALIGN(8) (NOLOAD) : {
        PROVIDE(__stack_start = .);
        PROVIDE(__stack_usr_start = .);
        . += STACK_USR_SIZE;
        PROVIDE(__stack_usr_end = .);
        PROVIDE(__stack_exc_start = .);
        . += STACK_EXC_SIZE;
        PROVIDE(__stack_exc_end = .);
        PROVIDE(__stack_end = .);
    } > DRAM

    .heap ALIGN(8) (NOLOAD) : {
        PROVIDE(__heap_start = .);
        . += HEAP_SIZE;
        . = ALIGN(8);
        PROVIDE(__heap_end = .);
    } > DRAM
}

