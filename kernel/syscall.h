#pragma once
#include <kernel/types.h>

dword do_syscall(dword eax, dword ebx, dword ecx);
void syscall_handler(regs *r);
