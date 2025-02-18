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
  init_allocator((void *)ADDR_YALLOC_START, 0x200000); // Init memory allocator
  vfs = NULL;                                          // Reset VFS
  strncpy(cwd, "/", PATH_LIMIT); // Set current working directory

#ifdef ROM_EXISTS
  set_rom();
#endif

  shell();
}
