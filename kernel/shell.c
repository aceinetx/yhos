#include <kernel/lowlevel.h>
#include <kernel/shell.h>
#include <kernel/std.h>
#include <kernel/syscall.h>
#include <kernel/version.h>

char help_msg[] = "Available commands: \n"
                  "EXIT   - halt the system\n"
                  "REBOOT - reboot the system\n"
                  "HELP   - print this message\n"
                  "VER    - print yhOS version\n"
                  "TEST   - do a test\n"
                  "LS     - list files in current working directory\n"
                  "CAT    - print contents of a file\n"
                  "CLEAR  - clear the screen\n";

void kernel_test();

char cmd[255];
char arrow[255];
char args[sizeof(cmd)][10];

void default_arrow() { strncpy(arrow, "> ", 255); }

void ls() {
  for (int i = 0; i < (int)VFS_ESIZE; i++) {
    vfs_file *file = &vfs[i];
    if (file->name[0] == '\0' && file->content[0] == '\0')
      break;
    syscall(SYS_WRITE, file->name);
    syscall(SYS_WRITE, "\n");
  }
}

void splitcmd() {
  int i = 0;
  int k = 0;
  memset(args, 0, sizeof(cmd) * 10);

  char buf[sizeof(cmd)];
  memset(buf, 0, sizeof(buf));
  for (int j = 0; j < (int)sizeof(cmd); j++) {
    if (j != ' ') {
      buf[k] = j;
      k++;
    } else {
      memcpy(&args[i], buf, sizeof(cmd));
      memset(buf, 0, sizeof(buf));
      k = 0;
      i++;
    }
  }
}

void shell() {
  default_arrow();
  for (;;) {
    syscall(SYS_WRITE, arrow);
    vgavec2 initial_pos = get_cursor_pos();
    syscall(SYS_GETS, cmd, sizeof(cmd));
    default_arrow();
    splitcmd();

    syscall(SYS_WRITE, args[1]);
    if (strcmp(cmd, "EXIT") == 0) {
      syscall(SYS_WRITE, "halted\n");
      break;
    } else if (strcmp(cmd, "REBOOT") == 0) {
      byte good = 0x02;
      while (good & 0x02)
        good = inb(0x64);
      outb(0x64, 0xFE);
      asm volatile("hlt\n");
      /*asm volatile(
          ".intel_syntax noprefix\nmov ebx, 0\ndiv ebx\n.att_syntax
         prefix\n");*/
    } else if (strcmp(cmd, "HELP") == 0) {
      syscall(SYS_WRITE, help_msg);
    } else if (strcmp(cmd, "VER") == 0) {
      syscall(SYS_WRITE,
              YHOS_VER_FULL " (https://github.com/aceinetx/yhos)\n\0");
    } else if (strcmp(cmd, "TEST") == 0) {
      kernel_test();
    } else if (strcmp(cmd, "LS") == 0) {
      ls();
    } else if (strcmp(cmd, "CLEAR") == 0) {
      memcpy(VGA_BUFFER, (void *)0x140000,
             VGA_WIDTH * VGA_HEIGHT * sizeof(word));
      set_cursor_pos(0, 0);
    } else {
      if (cmd[0] != '\0') {
        syscall(SYS_WRITE, "(no match)\n");
        vgavec2 old_pos = get_cursor_pos();
        set_cursor_pos(VGA_WIDTH - 4, initial_pos.y);
        syscall(SYS_WRITE, "FAIL");
        set_cursor_pos(old_pos.x, old_pos.y);
      }
    }
  }
}
