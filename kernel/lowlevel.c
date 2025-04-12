/*
 * yhOS
 * Copyright (c) aceinet
 * License: GPL-2.0
 */
#include <kernel/lowlevel.h>
#include <kernel/std.h>

void outb(word port, byte value) {
	asm volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

void outw(word port, word value) {
	asm volatile("outw %0, %1" : : "a"(value), "Nd"(port));
}

byte inb(word port) {
	byte value;
	asm volatile("inb %1, %0" : "=a"(value) : "Nd"(port));
	return value;
}

void vga_scroll() {
	dword vga_size = VGA_WIDTH * VGA_HEIGHT * sizeof(word);
	word buf[vga_size];
	memset(buf, 0, vga_size);
	memcpy(buf, VGA_BUFFER + VGA_WIDTH, vga_size - VGA_WIDTH);
	memcpy(VGA_BUFFER, buf, vga_size);
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
		goto vga_putc_newlend;
	}

	VGA_BUFFER[pos.y * 80 + pos.x] = (word)c | 0x0700;
	pos.x++;
	if (pos.x > VGA_WIDTH) {
		goto vga_putc_newlend;
	}

	goto vga_putc_end;

vga_putc_newlend:
	pos.x = 0;
	pos.y++;
	if (pos.y >= 25) {
		pos.y--;
		vga_scroll();
	}
vga_putc_end:
	set_cursor_pos(pos.x, pos.y);
}
