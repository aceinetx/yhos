#pragma once

#include <stdint.h>
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
  char name[VFS_MAX_FILE_NAME];
  char content[VFS_MAX_FILE_SIZE];
} vfs_file;

typedef struct {
  char x;
  char y;
} vgavec2;
