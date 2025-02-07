#pragma once

#include <stdint.h>

#ifndef NULL
#define NULL ((void *)0)
#endif

typedef uint8_t byte;
typedef uint16_t word;
typedef uint32_t dword;

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
} vfs_file;

typedef struct {
  char x;
  char y;
} vgavec2;
