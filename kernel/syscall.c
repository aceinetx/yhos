#include <kernel/keyboard.h>
#include <kernel/lowlevel.h>
#include <kernel/syscall.h>

dmem_entry dmem_addr[DMEM_ADDR_LIMIT] = {0};
dmem_entry dmem_unmapped_addr[DMEM_ADDR_LIMIT] = {0};
dword dmem_unmapped_i = -1;
dword dmem_next_addr = DMEM_BEGIN;

dword do_syscall(dword eax, dword ebx, dword ecx) {
  asm volatile("int $0x80" : : "a"(eax), "b"(ebx), "c"(ecx));
  asm volatile("push %eax\n");
  dword ret;
  asm volatile("pop %0\n" : "=a"(ret));
  return ret;
}

void syscall_handler(regs *r) {
  dword syscall_num = r->eax;
  if (syscall_num == SYS_WRITEC) {
    char c = (char)r->ebx;
    vga_putc(c);
  } else if (syscall_num == SYS_WRITE) {
    char *c = (char *)r->ebx;
    while (*c != 0) {
      vga_putc(*c);
      c++;
    }
  } else if (syscall_num == SYS_GETS) {
    // string in ebx
    // length in ecx
    char *c = (char *)r->ebx;
    for (;;) {
      keyboard_result kc = keyboard_handle_input();
      byte is_not_full = (dword)c < (dword)(r->ebx + r->ecx);
      byte is_empty = (dword)c == r->ebx;

      if (kc.ch == '\n' && kc.is_valid) {
        vga_putc(kc.ch);
        *c = '\0';
        break;
      }

      if (kc.is_valid && is_not_full) {
        *c = kc.ch;
        vga_putc(kc.ch);
        c++;
      } else {
        if (kc.ch == 0x0E && !is_empty) {
          vgavec2 pos = get_cursor_pos();
          pos.x -= 1;
          set_cursor_pos(pos.x, pos.y);

          vga_putc(' ');

          set_cursor_pos(pos.x, pos.y);
          c--;
          *c = 0;
        }
      }
    }
  } else if (syscall_num == SYS_DMAP) {
    r->eax = 0;
    dword len = r->ebx;

    for (int i = 0; i < DMEM_ADDR_LIMIT; i++) {
    }

    for (int i = 0; i < DMEM_ADDR_LIMIT; i++) {
      dmem_entry *entry = &dmem_addr[i];
      if (entry->addr == 0) {
        // found free address
        dmem_next_addr -= len;
        entry->addr = dmem_next_addr;
        entry->len = len;
        r->eax = entry->addr;
      }
    }
  } else if (syscall_num == SYS_DUNMAP) {
    r->eax = 0;

    for (int i = 0; i < DMEM_ADDR_LIMIT; i++) {
      dmem_entry *entry = &dmem_addr[i];
      if (entry->addr == r->ebx) {
        dmem_unmapped_i++;
        dmem_entry *unmapped_entry = &dmem_unmapped_addr[dmem_unmapped_i];
        last_unmapped_addr = entry->addr;
        last_unmapped_len = entry->len;
        entry->addr = 0;
        entry->len = 0;
        r->eax = entry->addr;
      }
    }
  }
}
