/*
 * yhOS
 * Copyright (c) aceinet
 * License: GPL-2.0
 */
#include <kernel/lowlevel.h>
#include <kernel/shell.h>
#include <kernel/std.h>
#include <kernel/syscall.h>
#include <kernel/types.h>
#include <kernel/version.h>
#include <kernel/yalloc.h>

#ifdef DCC_EXISTS
void dc_test();
#endif

char help_msg[] = "Available commands: \n"
                  "exit   - halt the system\n"
                  "reboot - reboot the system\n"
                  "help   - print this message\n"
                  "ver    - print yhOS version\n"
                  "test   - do a test\n"
                  "ls     - list files in current working directory\n"
                  "cat    - print contents of a file\n"
                  "run    - run a executable\n"
                  "clear  - clear the screen\n";

void kernel_test();

char cmd[SHELL_CONST1];
char arrow[SHELL_CONST1];
char arg_buf[SHELL_CONST1];
char *arg_p;

void default_arrow() {
  strncpy(arrow, cwd, SHELL_CONST1);
  strncpy(arrow + strlen(cwd), " > ", SHELL_CONST1);
}

void ls() {
  for (dword i = 0; i < vfs_size; i++) {
    vfs_file *file = &vfs[i];
    if (file->content == NULL || file->name == NULL)
      continue;

    bool hidden = false;
    {
      bool slash = false;
      char *c = file->name;
      while (*c != 0) {
        if (slash) {
          if (*c == '.')
            hidden = true;
          slash = false;
        }

        if (*c == '/')
          slash = true;
        c++;
      }
    }
    if (hidden)
      continue;

    {
      dword buf_size = strlen(cwd) + 1;
      char buf[buf_size];
      memcpy(buf, file->name, buf_size - 1);
      buf[buf_size - 1] = 0;
      if (strcmp(buf, cwd) == 0) {
        syscall(SYS_WRITE, file->name + buf_size - 1);
        syscall(SYS_WRITE, "\n");
      }
    }
  }
}

void nextarg() {
  memset(arg_buf, 0, SHELL_CONST1);
  int i = 0;

  while (*arg_p != 0 && *arg_p != ' ') {
    arg_buf[i] = cmd[arg_p - cmd];
    i++;
    arg_p++;
  }
  arg_p++;
}

vgavec2 initial_pos;
void fail(vgavec2 old_pos) {
  set_cursor_pos(VGA_WIDTH - 4, initial_pos.y);
  syscall(SYS_WRITE, "FAIL");
  set_cursor_pos(old_pos.x, old_pos.y);
}
void fail_ex(vgavec2 old_pos, char *s) {
  set_cursor_pos(VGA_WIDTH - strlen(s), initial_pos.y);
  syscall(SYS_WRITE, s);
  set_cursor_pos(old_pos.x, old_pos.y);
}

void shell() {
  default_arrow();
  for (;;) {
    syscall(SYS_WRITE, arrow);
    initial_pos = get_cursor_pos();
    syscall(SYS_GETS, cmd, sizeof(cmd));
    default_arrow();

    arg_p = cmd;
    nextarg();

    if (strcmp(arg_buf, "exit") == 0) {
      syscall(SYS_WRITE, "halted\n");
      break;
    } else if (strcmp(arg_buf, "reboot") == 0) {
      byte good = 0x02;
      while (good & 0x02)
        good = inb(0x64);
      outb(0x64, 0xFE);
      asm volatile("hlt\n");
    } else if (strcmp(arg_buf, "help") == 0) {
      syscall(SYS_WRITE, help_msg);
    } else if (strcmp(arg_buf, "ver") == 0) {
      syscall(SYS_WRITE,
              YHOS_VER_FULL " (https://github.com/aceinetx/yhos)\n\0");
    } else if (strcmp(arg_buf, "test") == 0) {
      kernel_test();
#if DCC_EXISTS
      dc_test();
#endif
    } else if (strcmp(arg_buf, "ls") == 0) {
      ls();
    } else if (strcmp(arg_buf, "cat") == 0) {
      nextarg();

      char filename[SHELL_CONST1];
      memcpy(filename, arg_buf, SHELL_CONST1);

      dword buf_size = syscall(SYS_VFSQUERY, filename);
      if (buf_size == (dword)-1) {
        syscall(SYS_WRITE, "No such file or directory\n");
      } else {
        char *buf = yalloc(buf_size);
        memset(buf, 0, buf_size);

        dword result;
        result = syscall(SYS_VFSREAD, filename, buf, buf_size);
        if (result == 0) {
          syscall(SYS_WRITE, "No such file or directory\n");
        } else {
          if (strcmp(buf, "YHSE\0") != 0) {
            syscall(SYS_WRITE, buf);
            syscall(SYS_WRITE, "\n");
          } else {
            syscall(SYS_WRITE, "(executable file)\n");
          }
        }
      }
    } else if (strcmp(arg_buf, "clear") == 0) {
      memcpy(VGA_BUFFER, (void *)0x140000,
             VGA_WIDTH * VGA_HEIGHT * sizeof(word));
      set_cursor_pos(0, 0);
    } else if (strcmp(arg_buf, "x") == 0) {
      syscall(SYS_VFSWRITE, ".vfstest1", "Hello there", 12);
    } else if (strcmp(arg_buf, "run") == 0) {
      nextarg();
      dword exe_size = syscall(SYS_VFSQUERY, arg_buf);
      if (exe_size == (dword)-1) {
        syscall(SYS_WRITE, "No such file or directory\n");
      } else {
        void *code = (void *)0x40000;
        yhse_hdr *header = (yhse_hdr *)0x40000;

        syscall(SYS_VFSREAD, arg_buf, code, exe_size);

        if (strcmp((char *)header->ident, "YHSE\0") != 0) {
          syscall(SYS_WRITE, "Check failed: not a valid yhSE executable\n");
        } else {
          entry_t start = (entry_t)header->entry;
          int result = start();
          if (result != 0) {
            char b[32];
            strncpy(b, "FAIL: ", sizeof(b));
            dword len = strlen(b);
            syscall(SYS_ITOA, result, b + len, sizeof(b) - len);
            fail_ex(get_cursor_pos(), b);
          }
        }
      }
    } else {
      if (cmd[0] != '\0') {
        syscall(SYS_WRITE, "(no match)\n");
        fail(get_cursor_pos());
      }
    }
  }
}
