#include <kernel/keyboard.h>
#include <kernel/lowlevel.h>
#include <kernel/std.h>
#include <kernel/syscall.h>

vfs_file vfs[VFS_ESIZE] = {0};

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
    r->eax = 0;

    for (int i = 0; i < (int)VFS_ESIZE; i++) {
      vfs_file *file = &vfs[i];
      if (strcmp(file->name, filename) == 0) {
        memcpy(file->content, buf, VFS_MAX_FILE_SIZE);
        r->eax = (dword)file->content;
        break;
      }
    }

    // if we didn't find the file, try creating a new one
    if (r->eax == 0) {
      for (int i = 0; i < (int)VFS_ESIZE; i++) {
        vfs_file *file = &vfs[i];
        if (file->name[0] == '\0' &&
            file->content[0] == '\0') { // essentially empty file
          memcpy(file->content, buf, VFS_MAX_FILE_SIZE);
          memcpy(file->name, filename, VFS_MAX_FILE_NAME);
          r->eax = (dword)file->content;
          break;
        }
      }
    }
  } else if (syscall_num == SYS_VFSREAD) {
    char *filename = (char *)r->ebx;
    char *buf = (char *)r->ecx;
    r->eax = 0;

    for (int i = 0; i < (int)VFS_ESIZE; i++) {
      vfs_file *file = &vfs[i];
      if (strcmp(file->name, filename) == 0) {
        memcpy(buf, file->content, VFS_MAX_FILE_SIZE);
        r->eax = (dword)file->content;
        break;
      }
    }
  }
}
