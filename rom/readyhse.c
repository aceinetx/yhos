#include "yhos.h"

void _start() {
  char *filename = (char *)syscall(SYS_EXEARG);

  dword size = syscall(SYS_VFSQUERY, filename);
  if (size == -1) {
    syscall(SYS_WRITE, "No such file or directory\n");
    return;
  }

  char *buf = (char *)syscall(SYS_ALLOC, size);
  syscall(SYS_VFSREAD, filename, buf, size);

  yhse_hdr *header = (yhse_hdr *)buf;
  if (strcmp(header->ident, "YHSE") != 0) {
    syscall(SYS_WRITE, "Not a executable\n");
    return;
  }

  syscall(SYS_WRITE, filename);
  syscall(SYS_WRITE, ": yhSE executable (yhOS Static Executable)\n");
  syscall(SYS_WRITE, "Entry point: ");

  {
    char h[16];
    syscall(SYS_ITOA16, header->entry, h, sizeof(h));
    syscall(SYS_WRITE, "0x");
    syscall(SYS_WRITE, h);
    syscall(SYS_WRITEC, '\n');
  }

  syscall(SYS_FREE, filename);
  return;
}
