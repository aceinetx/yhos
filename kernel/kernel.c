#include <kernel/keyboard.h>
#include <kernel/lowlevel.h>
#include <kernel/shell.h>
#include <kernel/std.h>
#include <kernel/syscall.h>
#include <kernel/types.h>
#include <kernel/version.h>
#include <kernel/yalloc.h>

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

void add_test_exe() {
  unsigned char data[] = {
      0x7f, 0x45, 0x4c, 0x46, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x03, 0x00, 0x01, 0x00, 0x00, 0x00,
      0x54, 0x80, 0x04, 0x08, 0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x34, 0x00, 0x20, 0x00, 0x01, 0x00, 0x28, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x80, 0x04, 0x08, 0x00, 0x80, 0x04, 0x08, 0x7e, 0x00, 0x00, 0x00,
      0x7e, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00,
      0xb8, 0x05, 0x00, 0x00, 0x00, 0x83, 0xf8, 0x01, 0x7c, 0x10, 0x50, 0xb8,
      0x01, 0x00, 0x00, 0x00, 0xbb, 0x6f, 0x80, 0x04, 0x08, 0xcd, 0x80, 0x58,
      0xeb, 0xeb, 0xc3, 0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x2c, 0x20, 0x57, 0x6f,
      0x72, 0x6c, 0x64, 0x21, 0x0a, 0x00};
  syscall(SYS_VFSWRITE, "hello.elf", data, sizeof(data));
}

void main() {
  memcpy((void *)0x140000, VGA_BUFFER, VGA_WIDTH * VGA_HEIGHT * sizeof(word));
  syscall(SYS_WRITE, "yhOS " YHOS_VER " - aceinetx (c) 2022-2025\n");
  syscall(SYS_WRITE, "[...] git commit: " GIT_COMMIT "\n");

  syscall(SYS_WRITE, "[...] mem reset\n");
  init_allocator((void *)0x200000, 0x200000);
  vfs = NULL;

  syscall(SYS_WRITE, "[...] testing vfs\n");

  vfs_check1();
  vfs_check2();

  syscall(SYS_WRITE,
          "[...] all checks passed, vfs supports reading & writing\n");

  add_test_exe();

  shell();
}
