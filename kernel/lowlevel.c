#include <kernel/lowlevel.h>
#define VGA_BUFFER ((word *)0xB8000)
#define VGA_WIDTH 80

void outb(word port, byte value) {
  asm volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

byte inb(word port) {
  byte value;
  asm volatile("inb %1, %0" : "=a"(value) : "Nd"(port));
  return value;
}

void set_cursor_pos(word x, word y) {
  word pos = y * VGA_WIDTH + x;
  outb(0x3D4, 0x0F);
  outb(0x3D5, (word)(pos & 0xFF));
  outb(0x3D4, 0x0E);
  outb(0x3D5, (word)((pos >> 8) & 0xFF));
}

word get_cursor_pos_ex() {
  word pos = 0;
  outb(0x3D4, 0x0F);
  pos |= inb(0x3D5);
  outb(0x3D4, 0x0E);
  pos |= ((word)inb(0x3D5)) << 8;
  return pos;
}

vgavec2 get_cursor_pos() {
  word pos = 0;
  outb(0x3D4, 0x0F);
  pos |= inb(0x3D5);
  outb(0x3D4, 0x0E);
  pos |= ((word)inb(0x3D5)) << 8;
  vgavec2 vec;
  vec.x = pos % VGA_WIDTH;
  vec.y = pos / VGA_WIDTH;
  return vec;
}

void vga_putc(char c) {
  vgavec2 pos = get_cursor_pos();

  if (c == '\n') {
    pos.y++;
    pos.x = 0;
    goto vga_putc_end;
  }

  VGA_BUFFER[pos.y * 80 + pos.x] = (word)c | 0x0F00;
  pos.x++;
  if (pos.x > VGA_WIDTH) {
    pos.x = 0;
    pos.y++;
  }

vga_putc_end:
  set_cursor_pos(pos.x, pos.y);
}
