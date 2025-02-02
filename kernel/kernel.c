#include "lowlevel.h"
#include "types.h"

#define VGA_WIDTH 80

typedef struct {
  char x;
  char y;
} vgavec2;

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

void puts(char *s) {
  char *p = (char *)(0xB8000 + get_cursor_pos_ex() * 2);
  for (;;) {
    if (*s == 0)
      break;
    if (*s == 10) {
      vgavec2 pos = get_cursor_pos();
      pos.y++;
      pos.x = 0;
      set_cursor_pos(pos.x, pos.y);
      p += 2;
      s++;
      continue;
    }

    *p = *s;

    vgavec2 pos = get_cursor_pos();
    pos.x++;
    set_cursor_pos(pos.x, pos.y);

    p += 2;
    s++;
  }
}

int main() {
  // outb(0x03D4, 0x0A);
  // outb(0x03D5, 0x20);
  puts("yhOS v1.0 aceinetx (c) 2022-2025\n");

  for (;;) {
  }
  return 0;
}
