
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

yhse_load_addr = 0x40000;
SECTIONS
{
	. = yhse_load_addr;

	.header : {
		BYTE(0x59);
		BYTE(0x48);
		BYTE(0x53);
		BYTE(0x45);
		BYTE(0x00);

		LONG(yhse_load_addr);
		LONG(_start);
		LONG(LOADADDR(.yhse_symtab));
		LONG(0xb);
	}

	.text : SUBALIGN(1) {
		*(.text .text.*)
	}

	.rodata : SUBALIGN(1) {
		*(.rodata .rodata.*)
	}

	.data : SUBALIGN(1) {
		*(.data .data.*)
	}

	.bss : SUBALIGN(1) {
		*(.bss .bss.*)
		*(COMMON)
	}


	.yhse_symtab : SUBALIGN(1) {
		BYTE(0x5f); /* _ */
		BYTE(0x73); /* s */
		BYTE(0x74); /* t */
		BYTE(0x61); /* a */
		BYTE(0x72); /* r */
		BYTE(0x74); /* t */
		BYTE(0x0);
		LONG(_start);
	}



	/DISCARD/ : {
		*(.note.*)
		*(.comment)
		*(.eh_frame)
	}

}
