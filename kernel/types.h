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
