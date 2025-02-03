#include <kernel/lowlevel.h>
#include <kernel/types.h>

dword main() {
  char str[] = "Hello, World!\n";
  asm volatile("int $0x80" : : "a"(1), "b"(str));
}
