/*
 * yhOS
 * Copyright (c) aceinet
 * License: GPL-2.0
 */
#pragma once
#include <kernel/types.h>

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

#define PATH_LIMIT 255

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

extern vfs_file *vfs;
extern dword vfs_size;
extern char cwd[PATH_LIMIT];

dword do_syscall(dword eax, dword ebx, dword ecx, dword edx);
void syscall_handler(regs *r);
