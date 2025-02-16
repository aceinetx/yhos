/*
 * yhOS
 * Copyright (c) aceinet
 * License: GPL-2.0
 */
// This file must be included at the very top of the code
#pragma once
// asm(".string \"YHSE\"");
// asm(".long _start");

#include <kernel/types.h>

#ifndef YHSE_NOLIB
dword do_syscall(dword eax, dword ebx, dword ecx, dword edx) {
  asm volatile("int $0x80" : : "a"(eax), "b"(ebx), "c"(ecx), "d"(edx));
  asm volatile("push %eax\n");
  dword ret;
  asm volatile("pop %0\n" : "=a"(ret));
  return ret;
}
#endif

#define vrg_cnt(vrg1, vrg2, vrg3, vrg4, vrg5, vrg6, vrg7, vrg8, vrgN, ...) vrgN
#define vrg_argn(...) vrg_cnt(__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define vrg_cat0(x, y) x##y
#define vrg_cat(x, y) vrg_cat0(x, y)

#define vrg(vrg_f, ...) vrg_cat(vrg_f, vrg_argn(__VA_ARGS__))(__VA_ARGS__)

#define syscall(...) vrg(syscall, __VA_ARGS__)
#define syscall1(eax) do_syscall((dword)(eax), 0, 0, 0)
#define syscall2(eax, ebx) do_syscall((dword)(eax), (dword)(ebx), 0, 0)
#define syscall3(eax, ebx, ecx)                                                \
  do_syscall((dword)(eax), (dword)(ebx), (dword)(ecx), 0)
#define syscall4(eax, ebx, ecx, edx)                                           \
  do_syscall((dword)(eax), (dword)(ebx), (dword)(ecx), (dword)(edx))

#define SYS_WRITEC 0
#define SYS_WRITE 1
#define SYS_GETS 2
#define SYS_VFSWRITE 3
#define SYS_VFSREAD 4
#define SYS_VFSQUERY 5
#define SYS_ALLOC 6
#define SYS_FREE 7
#define SYS_VFSBASE 8
#define SYS_EXEARG 9
#define SYS_VFSHANDLE 10
#define SYS_ITOA 11
#define SYS_ITOA16 12
#define SYS_GETCWD 13
#define SYS_GETC 14

#define print(buf) syscall(SYS_WRITE, buf)
#define printc(c) syscall(SYS_WRITEC, c)

#ifndef YHSE_NOLIB
int strcmp(char *p1, char *p2) {
  char *s1 = (char *)p1;
  char *s2 = (char *)p2;
  char c1, c2;

  do {
    c1 = (char)*s1++;
    c2 = (char)*s2++;
    if (c1 == '\0')
      return c1 - c2;
  } while (c1 == c2);

  return c1 - c2;
}

void *memcpy(void *dest, void *src, dword len) {
  char *d = (char *)dest;
  char *s = (char *)src;
  while (len--)
    *d++ = *s++;
  return dest;
}

void *memset(void *dest, dword val, dword len) {
  char *ptr = (char *)dest;
  while (len-- > 0)
    *ptr++ = val;
  return dest;
}

dword strnlen(char *s, dword maxlen) {
  dword i;

  for (i = 0; i < maxlen; ++i)
    if (s[i] == '\0')
      break;
  return i;
}

dword strlen(char *s) {
  dword i;

  for (i = 0; 1; ++i)
    if (s[i] == '\0')
      break;
  return i;
}

char *strncpy(char *s1, char *s2, dword n) {
  dword size = strnlen(s2, n);
  if (size != n)
    memset(s1 + size, '\0', n - size);
  return (char *)memcpy((void *)s1, (void *)s2, size);
}
#endif

#define _start int _start

#ifdef __cplusplus
#undef _start
#define _start extern "C" int _start
#endif
