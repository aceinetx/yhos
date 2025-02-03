#include <kernel/lowlevel.h>
#include <kernel/syscall.h>
#include <kernel/types.h>

void main() {
  char str[] = "yhos v1.0 - aceinetx (c) 2022-2025\n";
  // asm volatile("int $0x80" : : "a"(1), "b"(str));
  do_syscall(1, (dword)str, 0);
}
