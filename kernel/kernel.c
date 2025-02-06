#include <kernel/keyboard.h>
#include <kernel/lowlevel.h>
#include <kernel/shell.h>
#include <kernel/std.h>
#include <kernel/syscall.h>
#include <kernel/types.h>
#include <kernel/version.h>

void vfs_check1() {
  dword result;
  result = syscall(SYS_VFSWRITE, ".vfstest1", "1", 2);
  if (result == 0) {
    syscall(SYS_WRITE, "[ ! ] vfs failed (writing), cannot continue\n");
    asm volatile("hlt\n");
  } else {
    syscall(SYS_WRITE, "[...] first check passed, continuing\n");
  }

  char buf[2];
  result = syscall(SYS_VFSREAD, ".vfstest1", buf, 2);
  if (result == 0) {
    syscall(SYS_WRITE, "[ ! ] vfs failed (reading), cannot continue\n");
    asm volatile("hlt\n");
  } else {
    if (strcmp(buf, "1\0") != 0) {
      syscall(SYS_WRITE, "[ ! ] vfs failed (reading: cmp), cannot continue\n");
      syscall(SYS_WRITE, "[ ! ] buf: ");
      syscall(SYS_WRITE, buf);
      syscall(SYS_WRITE, "\n");
      syscall(SYS_WRITE, "[ ! ] excepted: 1\n");
      asm volatile("hlt\n");
    }
    syscall(SYS_WRITE, "[...] second check passed, continuing\n");
  }
}

void vfs_check2() {
  dword result;
  result = syscall(SYS_VFSWRITE, ".vfstest2", "2", 2);
  if (result == 0) {
    syscall(SYS_WRITE, "[ ! ] vfs failed (writing), cannot continue\n");
    asm volatile("hlt\n");
  } else {
    syscall(SYS_WRITE, "[...] first check passed, continuing\n");
  }

  char buf[2];
  result = syscall(SYS_VFSREAD, ".vfstest2", buf, 2);
  if (result == 0) {
    syscall(SYS_WRITE, "[ ! ] vfs failed (reading), cannot continue\n");
    asm volatile("hlt\n");
  } else {
    if (strcmp(buf, "2\0") != 0) {
      syscall(SYS_WRITE, "[ ! ] vfs failed (reading: cmp), cannot continue\n");
      syscall(SYS_WRITE, "[ ! ] buf: ");
      syscall(SYS_WRITE, buf);
      syscall(SYS_WRITE, "\n");
      syscall(SYS_WRITE, "[ ! ] excepted: 2\n");
      asm volatile("hlt\n");
    }
    syscall(SYS_WRITE, "[...] second check passed, continuing\n");
  }
}

void main() {
  memcpy((void *)0x140000, VGA_BUFFER, VGA_WIDTH * VGA_HEIGHT * sizeof(word));
  syscall(SYS_WRITE, "yhOS " YHOS_VER " - aceinetx (c) 2022-2025\n");
  syscall(SYS_WRITE, "[...] git commit: " GIT_COMMIT "\n");

  syscall(SYS_WRITE, "[...] mem reset\n");
  memset(vfs, 0, VFS_SIZE);

  syscall(SYS_WRITE, "[...] testing vfs\n");

  vfs_check1();
  vfs_check2();

  syscall(SYS_WRITE,
          "[...] all checks passed, vfs supports reading & writing\n");

  shell();
}
