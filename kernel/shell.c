#include <kernel/shell.h>
#include <kernel/std.h>
#include <kernel/syscall.h>

void shell() {
  char cmd[255];
  for (;;) {
    do_syscall(SYS_WRITE, (dword)("> "), 0);
    do_syscall(SYS_GETS, (dword)cmd, sizeof(cmd));
    if (strcmp(cmd, "EXIT") == 0) {
      do_syscall(SYS_WRITE, (dword)("halted\n"), 0);
      break;
    }
  }
}
