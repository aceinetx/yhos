#include <yhos.h>

void _start() {
  char *filename = (char *)syscall(SYS_EXEARG);
  char *str = (char *)syscall(SYS_EXEARG);

  dword old_size = syscall(SYS_VFSQUERY, filename);
  if (old_size == -1) {
    syscall(SYS_WRITE, "No such file or directory\n");
    return;
  }
  dword new_size = old_size + strlen(str);

  char *new_buf = (char *)syscall(SYS_ALLOC, new_size + 1);
  syscall(SYS_VFSREAD, filename, new_buf, old_size);

  strncpy(new_buf + old_size - 1, str, strlen(str));

  syscall(SYS_VFSWRITE, filename, new_buf, new_size + 1);
  syscall(SYS_FREE, new_buf);

  syscall(SYS_WRITE, "Appending \"");
  syscall(SYS_WRITE, str);
  syscall(SYS_WRITE, "\" to ");
  syscall(SYS_WRITE, filename);
  syscall(SYS_WRITEC, 10);

  syscall(SYS_FREE, filename);
  syscall(SYS_FREE, str);
  return;
}
