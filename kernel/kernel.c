#include <kernel/keyboard.h>
#include <kernel/lowlevel.h>
#include <kernel/shell.h>
#include <kernel/syscall.h>
#include <kernel/types.h>
#include <kernel/version.h>

void main() {
  do_syscall(SYS_WRITE, (dword)("yhOS " YHOS_VER " - aceinetx (c) 2022-2025\n"),
             0);
  do_syscall(SYS_WRITE, (dword)("[...] git commit: " GIT_COMMIT "\n"), 0);

  shell();
}
