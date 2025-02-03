#pragma once
#include <kernel/types.h>

void outb(word port, byte value);
byte inb(word port);
void set_cursor_pos(word x, word y);
vgavec2 get_cursor_pos();
word get_cursor_pos_ex();
void vga_putc(char c);
