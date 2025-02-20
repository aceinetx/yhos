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

dword page_table[1024] __attribute__((aligned(4096)));
dword page_directory[1024] __attribute__((aligned(4096)));

void main() {
  memcpy((void *)ADDR_VGA_EMPTY, VGA_BUFFER,
	 VGA_WIDTH * VGA_HEIGHT * sizeof(word)); // Save empty screen data
  syscall(SYS_WRITE, "yhOS " YHOS_VER " - aceinetx (c) 2022-2025\n");
  syscall(SYS_WRITE, "[...] git commit: " GIT_COMMIT "\n");

  syscall(SYS_WRITE, "[...] reset\n");
  init_allocator((void *)ADDR_YALLOC_START, 0x10000); // Init memory allocator
  vfs = NULL;					      // Reset VFS
  strncpy(cwd, "/", PATH_LIMIT); // Set current working directory
  {
    int i;
    for (i = 0; i < 1024; i++) {
      // This sets the following flags to the pages:
      //   Supervisor: Only kernel-mode can access them
      //   Write Enabled: It can be both read from and written to
      //   Not Present: The page table is not present
      page_directory[i] = 0x00000002;
    }
  }
  {
    unsigned int i;

    // we will fill all 1024 entries in the table, mapping 4 megabytes
    for (i = 0; i < 1024; i++) {
      // As the address is page aligned, it will always leave 12 bits zeroed.
      // Those bits are used by the attributes ;)
      page_table[i] = (i * 0x1000) |
		      3; // attributes: supervisor level, read/write, present.
    }
  }
  page_directory[0] = ((unsigned int)page_table) | 3;

  asm volatile(
      "mov %0, %%eax\n"	   // Load physical address of page_directory into EAX
      "mov %%eax, %%cr3\n" // Set CR3 to point to the page directory
      "mov %%cr0, %%eax\n"
      "or $0x80000000, %%eax\n"
      "mov %%eax, %%cr0\n" ::"r"(page_directory)
      : "eax", "memory");

#ifdef ROM_EXISTS
  set_rom();
#endif

  shell();
}
