#include <kernel/shell.h>
#include <kernel/std.h>
#include <kernel/syscall.h>
#include <kernel/version.h>

char help_msg[] = "Available commands: \n"
                  "EXIT - halt the system\n"
                  "HELP - print this message\n"
                  "VER - print yhOS version\n";

char cmd[255];
char arrow[255];

void default_arrow() { strncpy(arrow, "____ > ", 255); }

void shell() {
  default_arrow();
  for (;;) {
    do_syscall(SYS_WRITE, (dword)arrow, 0);
    do_syscall(SYS_GETS, (dword)cmd, sizeof(cmd));
    default_arrow();
    if (strcmp(cmd, "EXIT") == 0) {
      do_syscall(SYS_WRITE, (dword)("halted\n"), 0);
      break;
    } else if (strcmp(cmd, "HELP") == 0) {
      do_syscall(SYS_WRITE, (dword)help_msg, 0);
    } else if (strcmp(cmd, "VER") == 0) {
      do_syscall(SYS_WRITE, (dword)YHOS_VER_FULL "\n\0", 0);
    } else {
      if (cmd[0] != '\0') {
        do_syscall(SYS_WRITE, (dword)("(no match)\n"), 0);
        strncpy(arrow, "FAIL > ", 255);
      }
    }
  }
}
