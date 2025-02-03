#pragma once
#include <kernel/types.h>

void outb(word port, byte value);
byte inb(word port);
void vga_putc(char c);
