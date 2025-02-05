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
  dword result = do_syscall(SYS_DMAP, (dword)(1), 0);
  if (result == 0) {
    do_syscall(SYS_WRITE,
               (dword)("[.!.] dynamic memory check failed. cannot continue "
                       "executing\n"),
               0);
    return;
  } else {
    do_syscall(SYS_WRITE, (dword)("[...] dynamic memory check passed\n"), 0);
  }

  shell();
}
