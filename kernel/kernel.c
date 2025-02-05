#include <kernel/keyboard.h>
#include <kernel/lowlevel.h>
#include <kernel/shell.h>
#include <kernel/std.h>
#include <kernel/syscall.h>
#include <kernel/types.h>
#include <kernel/version.h>

void main() {
  syscall(SYS_WRITE, "yhOS " YHOS_VER " - aceinetx (c) 2022-2025\n");
  syscall(SYS_WRITE, "[...] git commit: " GIT_COMMIT "\n");

  syscall(SYS_WRITE, "[...] testing vfs\n");
  char name[VFS_MAX_FILE_SIZE] = ".vfs_test";
  char text[VFS_MAX_FILE_NAME] = "(vfs test)";
  char text_buf[VFS_MAX_FILE_NAME];
  dword result;
  result = syscall(SYS_VFSWRITE, name, text);
  if (result == 0) {
    syscall(SYS_WRITE, "[ ! ] vfs failed (writing), cannot continue\n");
    return;
  } else {
    syscall(SYS_WRITE, "[...] first check passed, continuing\n");
  }

  result = syscall(SYS_VFSREAD, name, text_buf);
  if (result == 0) {
    syscall(SYS_WRITE, "[ ! ] vfs failed (reading), cannot continue\n");
    return;
  } else {
    if (strcmp(text, text_buf) == 0) {
      syscall(SYS_WRITE, "[ ! ] vfs failed (reading), cannot continue\n");
      return;
    }
    syscall(SYS_WRITE, "[...] second check passed, continuing\n");
  }

  shell();
}
