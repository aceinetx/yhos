/*
 * yhOS
 * Copyright (c) aceinet
 * License: GPL-2.0
 */
#pragma once

#ifndef NULL
#define NULL ((void *)0)
#endif

#define ADDR_VGA_EMPTY 0x140000
#define ADDR_YALLOC_START 0x200000
#define ADDR_EXE_LOAD 0x40000
#define ADDR_KERNEL_LOAD 0x1000

#ifndef __cplusplus
typedef enum { false = 0, true = 1, undef = 0xFFFFFFFF } bool;
#endif

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned int dword;
typedef unsigned int size_t;
typedef int (*entry_t)();

typedef struct {
  dword edi;
  dword esi;
  dword ebp;
  dword esp;
  dword ebx;
  dword edx;
  dword ecx;
  dword eax;
} regs;

typedef struct {
  dword size;
  char *content;
  char *name;
} vfs_file;

typedef struct {
  char x;
  char y;
} vgavec2;

// yhSE header
#define YHSE_IDENT 5

#pragma pack(push, 1)
typedef struct {
  char ident[YHSE_IDENT];
  dword load_addr;
  dword entry;
  dword symtab_addr;
  dword symtab_size;
} yhse_hdr;
#pragma pack(pop)
