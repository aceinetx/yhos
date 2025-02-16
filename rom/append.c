/*
 * yhOS
 * Copyright (c) aceinet
 * License: GPL-2.0
 */
#include <yhos.h>

_start() {
  dword ret = 0;
  char *filename = (char *)syscall(SYS_EXEARG);

  if (filename[0] == 0) {
    print("Usage: append [filename] [str]\n");
    ret = 1;
    goto cleanup_1;
  }

  char *str = (char *)syscall(SYS_EXEARG);

  if (str[0] == 0) {
    print("Usage: append [filename] [str]\n");
    ret = 1;
    goto cleanup_2;
  }

  dword old_size = syscall(SYS_VFSQUERY, filename);
  if (old_size == -1) {
    print("No such file or directory\n");
    ret = 1;
    goto cleanup_2;
  }

  print("Appending \"");
  print(str);
  print("\" to ");
  print(filename);
  printc('\n');

  dword new_size = old_size + strlen(str);

  char *new_buf = (char *)syscall(SYS_ALLOC, new_size + 1);
  syscall(SYS_VFSREAD, filename, new_buf, old_size);

  strncpy(new_buf + old_size - 1, str, strlen(str));

  syscall(SYS_VFSWRITE, filename, new_buf, new_size + 1);
  syscall(SYS_FREE, new_buf);

cleanup_2:
  syscall(SYS_FREE, str);
cleanup_1:
  syscall(SYS_FREE, filename);
  return ret;
}
