#include <kernel/keyboard.h>
#include <kernel/lowlevel.h>
#include <kernel/std.h>
#include <kernel/syscall.h>

byte vfs[VFS_SIZE] = {0};

dword do_syscall(dword eax, dword ebx, dword ecx, dword edx) {
  asm volatile("int $0x80" : : "a"(eax), "b"(ebx), "c"(ecx), "d"(edx));
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
  } else if (syscall_num == SYS_VFSWRITE) {
    char *filename = (char *)r->ebx;
    char *buf = (char *)r->ecx;
    dword filename_size = strlen(filename);
    dword buf_size = r->edx;
    r->eax = 0;

    for (int i = 0; i < (int)VFS_SIZE;) {
      char c = vfs[i];
      if (c == '\0') {
        if (i + filename_size + buf_size > VFS_SIZE) {
          break;
        }

        memcpy(&vfs[i], filename, filename_size + 1);
        memcpy(&vfs[i + filename_size + 1], &buf_size, sizeof(dword));
        memcpy(&vfs[i + filename_size + 1 + sizeof(dword)], buf, buf_size);

        r->eax = (dword)(&vfs[i]);
        break;
      } else {
        dword name_size;
        name_size += strlen((char *)&vfs[i]) + 1;
        dword size;
        memcpy(&size, &vfs[i + name_size], sizeof(dword));
        i += name_size + size + 1;
      }
    }
  } else if (syscall_num == SYS_VFSREAD) {
    char *filename = (char *)r->ebx;
    char *buf = (char *)r->ecx;
    dword filename_size = strlen(filename);
    dword buf_size = r->edx;
    r->eax = 0;

    for (int i = 0; i < (int)VFS_SIZE;) {
      char c = vfs[i];
      if (c != '\0') {
        if (strcmp((char *)&vfs[i], filename) != 0) {
          break;
        }

        memcpy(buf, &vfs[i + filename_size + 1 + sizeof(dword)], buf_size);
        r->eax = (dword)(&vfs[i]);
        break;
      } else {
        dword name_size;
        name_size += strlen((char *)&vfs[i]) + 1;
        dword size;
        memcpy(&size, &vfs[i + name_size], sizeof(dword));
        i += name_size + size + 1;
      }
    }
  }
}
