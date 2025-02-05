#pragma once
#include <kernel/types.h>
#define VGA_BUFFER ((word *)0xB8000)
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

void outb(word port, byte value);
byte inb(word port);
void vga_scroll();
void set_cursor_pos(word x, word y);
vgavec2 get_cursor_pos();
word get_cursor_pos_ex();
void vga_putc(char c);
