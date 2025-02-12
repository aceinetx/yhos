/*
 * yhOS
 * Copyright (c) aceinet
 * License: GPL-2.0
 */
#pragma once

#include <stdint.h>

#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef __cplusplus
typedef enum { false = 0, true = 1, undef = 0xFFFFFFFF } bool;
#endif

typedef uint8_t byte;
typedef uint16_t word;
typedef uint32_t dword;
typedef void (*entry_t)();

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
  dword entry;
} yhse_hdr;
#pragma pack(pop)
