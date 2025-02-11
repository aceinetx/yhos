#pragma once

#include <stdint.h>

#ifndef NULL
#define NULL ((void *)0)
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
typedef struct {
  byte ident[YHSE_IDENT];
  dword entry;
} yhse_hdr;
