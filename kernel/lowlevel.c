#include <kernel/lowlevel.h>

void outb(word port, byte value) {
  asm volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

byte inb(word port) {
  byte value;
  asm volatile("inb %1, %0" : "=a"(value) : "Nd"(port));
  return value;
}

#define VGA_BUFFER ((word *)0xB8000)

void vga_putc(char c) {
  static dword row = 0;
  static dword col = 0;

  if (c == '\n') {
    row++;
    col = 0;
  } else {
    VGA_BUFFER[row * 80 + col] = (word)c | 0x0F00; // White on black
    col++;
    if (col >= 80) {
      col = 0;
      row++;
    }
    if (row >= 25) {
      // Simple scroll by resetting to last row
      row = 24;
    }
  }
}
