/*
 * yhOS
 * Copyright (c) aceinet
 * License: GPL-2.0
 */
#include <kernel/keyboard.h>
#include <kernel/lowlevel.h>
#include <kernel/shell.h>
#include <kernel/std.h>
#include <kernel/syscall.h>
#include <kernel/types.h>
#include <kernel/version.h>
#include <kernel/yalloc.h>

#ifdef ROM_EXISTS
#include <kernel/rom.h>
#endif

dword page_table_1[1024] __attribute__((aligned(4096))); // Maps 4MB - 8MB
dword page_table_2[1024] __attribute__((aligned(4096))); // Maps 8MB - 12MB
dword page_table[1024] __attribute__((aligned(4096)));
dword page_directory[1024] __attribute__((aligned(4096)));

void setup_paging() {
  int i;

  // Identity map the first 4MB
  for (i = 0; i < 1024; i++) {
    page_table[i] = (i * 0x1000) | 3; // Present, RW, Supervisor
  }
  page_directory[0] = ((unsigned int)page_table) | 3;

  // Map 4MB - 8MB
  for (i = 0; i < 1024; i++) {
    page_table_1[i] = (i * 0x1000 + 0x400000) | 3;
  }
  page_directory[1] = ((unsigned int)page_table_1) | 3;

  // Map 8MB - 12MB (including 9MB)
  for (i = 0; i < 1024; i++) {
    page_table_2[i] = (i * 0x1000 + 0x800000) | 3;
  }
  page_directory[2] = ((unsigned int)page_table_2) | 3;

  // Load page directory into CR3
  asm volatile("mov %0, %%cr3" ::"r"(page_directory));

  // Enable paging (set PG bit in CR0)
  asm volatile("mov %cr0, %eax\n"
	       "or $0x80000000, %eax\n"
	       "mov %eax, %cr0\n");
}

void main() {
  memcpy((void *)ADDR_VGA_EMPTY, VGA_BUFFER,
	 VGA_WIDTH * VGA_HEIGHT * sizeof(word)); // Save empty screen data
  syscall(SYS_WRITE, "yhOS " YHOS_VER " - aceinetx (c) 2022-2025\n");
  syscall(SYS_WRITE, "[...] git commit: " GIT_COMMIT "\n");

  syscall(SYS_WRITE, "[...] reset\n");
  init_allocator((void *)ADDR_YALLOC_START, 0x10000); // Init memory allocator
  vfs = NULL;					      // Reset VFS
  strncpy(cwd, "/", PATH_LIMIT); // Set current working directory

  setup_paging();

#ifdef ROM_EXISTS
  set_rom();
#endif

  shell();
}
