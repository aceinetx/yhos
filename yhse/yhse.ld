OUTPUT_FORMAT("binary")
ENTRY(_start)

SECTIONS
{
  . = 0x40000;
  .header 0 : {
    BYTE(89);
    BYTE(72);
    BYTE(83);
    BYTE(69);
    BYTE(0);
    LONG(_start+0x40000);
  }

  .text : {
    *(.text)
    *(.text.*)
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
