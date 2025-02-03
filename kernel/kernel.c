#include <kernel/lowlevel.h>
#include <kernel/types.h>

dword main() {
  // Test syscall: print 'A'
  asm volatile("int $0x80" : : "a"(0), "b"('A'));
}
