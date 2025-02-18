#include <yhos.h>

_start() {
  dword ret = 0;
  char *filename = (char *)syscall(SYS_EXEARG);

  if (filename[0] == 0) {
    print("Usage: readyhse [filename]\n");
    ret = 1;
    goto cleanup;
  }

  dword size = syscall(SYS_VFSQUERY, filename);
  if (size == -1) {
    print("No such file or directory\n");
    ret = 1;
    goto cleanup;
  }

  char *buf = (char *)syscall(SYS_ALLOC, size);
  syscall(SYS_VFSREAD, filename, buf, size);

  yhse_hdr *header = (yhse_hdr *)buf;
  if (strcmp(header->ident, "YHSE") != 0) {
    print("Not a executable\n");
    ret = 1;
    goto cleanup;
  }

  print(filename);
  print(": yhSE executable (yhOS Static Executable)\n");

  print("Load address: ");

  {
    char h[16];
    syscall(SYS_ITOA16, header->load_addr, h, sizeof(h));
    print("0x");
    print(h);
    printc('\n');
  }

  print("Entry point address: ");

  {
    char h[16];
    syscall(SYS_ITOA16, header->entry, h, sizeof(h));
    print("0x");
    print(h);
    printc('\n');
  }

  print("Symbol table address: ");

  {
    char h[16];
    syscall(SYS_ITOA16, header->symtab_addr, h, sizeof(h));
    print("0x");
    print(h);
    printc('\n');
  }

  print("Symbol table size: ");

  {
    char h[16];
    syscall(SYS_ITOA, header->symtab_size, h, sizeof(h));
    print(h);
    print(" bytes\n");
  }

  print("Executable size: ");

  {
    char h[16];
    syscall(SYS_ITOA, size, h, sizeof(h));
    print(h);
    print("B, ");

    syscall(SYS_ITOA, (dword)(size / 1024), h, sizeof(h));
    print(h);
    print("kB, ");

    syscall(SYS_ITOA, (dword)(size / 1024 / 1024), h, sizeof(h));
    print(h);
    print("mB");

    printc('\n');
  }

cleanup:
  syscall(SYS_FREE, filename);
  syscall(SYS_FREE, buf);
  return ret;
}
