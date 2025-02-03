#pragma once
#include <kernel/types.h>

#define SYS_WRITEC 0
#define SYS_WRITE 1
#define SYS_GETS 2

dword do_syscall(dword eax, dword ebx, dword ecx);
void syscall_handler(regs *r);
