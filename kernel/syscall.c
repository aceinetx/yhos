#include <kernel/lowlevel.h>
#include <kernel/syscall.h>

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
