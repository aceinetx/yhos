#include <kernel/lowlevel.h>
#include <kernel/syscall.h>

dword do_syscall(dword eax, dword ebx, dword ecx) {
  asm volatile("int $0x80" : : "a"(eax), "b"(ebx), "c"(ecx));
  asm volatile("push %eax\n");
  dword ret;
  asm volatile("pop %0\n" : "=a"(ret));
  return ret;
}

void syscall_handler(regs *r) {
  dword syscall_num = r->eax;
  if (syscall_num == 0) {
    char c = (char)r->ebx;
    vga_putc(c);
  } else if (syscall_num == 1) {
    char *c = (char *)r->ebx;
    while (*c != 0) {
      vga_putc(*c);
      c++;
    }
  }
}
