/*
 * yhOS
 * Copyright (c) aceinet
 * License: GPL-2.0
 */

/*
 * A linker script for yhSE toolchain
 * This linker script has support for
 * - GCC
 * - FASM
 */

OUTPUT_FORMAT("binary")
ENTRY(_start)

SECTIONS
{
  .header 0 : {
    BYTE(89);
    BYTE(72);
    BYTE(83);
    BYTE(69);
    BYTE(0);
    LONG(_start);
  }
  
	. = 0x40000 + SIZEOF(.header);

  .text : AT(LOADADDR(.header) + SIZEOF(.header)) {
    *(.text)
    *(.text.*)
  }

  .flat : AT(LOADADDR(.header) + SIZEOF(.header) + 0x3) {
    *(.flat)
    *(.flat.*)
  }

  .rodata : {
    *(.rodata)
    *(.rodata.*)
  }

  .data : {
    *(.data)
    *(.data.*)
  }

  .bss : {
    *(COMMON)
    *(.bss)
    *(.bss.*)
  }
}
