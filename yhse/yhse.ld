/*
 * yhOS
 * Copyright (c) aceinet
 * License: GPL-2.0
 */

/*
 * A linker script for yhSE toolchain
 * This linker script has support for
 * - GCC
 * - G++
 * - FASM
 * If you found a language linking properly, you should add it to this list via a PR at https://github.com/aceinetx/yhos
 */

OUTPUT_FORMAT("binary")
ENTRY(_start)

SECTIONS
{
    /* Load address set to 0x40000 as per your OS's loader */
    . = 0x40000;

    /* Header section with magic and entry point */
    .header : {
        /* Magic identifier "YHSE\0" (5 bytes) */
        BYTE(0x59); /* 'Y' */
        BYTE(0x48); /* 'H' */
        BYTE(0x53); /* 'S' */
        BYTE(0x45); /* 'E' */
        BYTE(0x00); /* Null terminator */

        /* Entry point address (4 bytes) */
        LONG(_start); /* _start's VMA will be after the header */
    }

    /* Text section immediately follows the header in memory */
    .text : SUBALIGN(1) { /* SUBALIGN(1) prevents padding */
        *(.text .text.*)  /* Collect all .text sections */
    }

    /* Other sections follow text with no gaps */
    .rodata : SUBALIGN(1) {
        *(.rodata .rodata.*)
    }

    .data : SUBALIGN(1) {
        *(.data .data.*)
    }

    .bss : SUBALIGN(1) {
        *(.bss .bss.*)
        *(COMMON)       /* COMMON symbols for uninitialized globals */
    }

    /* Discard unnecessary sections to reduce binary size */
    /DISCARD/ : {
        *(.note.*)
        *(.comment)
        *(.eh_frame)    /* Omit exception handling frames if not needed */
    }
}
