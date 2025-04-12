/*
 * yhOS
 * Copyright (c) aceinet
 * License: GPL-2.0
 */
#include <yhos.h>

char input[255];
char* buf;

_start() {
	print("EDitor for yhOS\n");
	char* filename = (char*)syscall(SYS_EXEARG);
	if (*filename == 0) {
		print("Usage: ed [filename]\n");
		delete filename;
		return 1;
	}

	dword file_size = syscall(SYS_VFSQUERY, filename);
	if (file_size == (dword)-1) {
		print("No such file or directory\n");
		delete filename;
		return 1;
	}

	buf = new char[file_size];
	syscall(SYS_VFSREAD, filename, buf, file_size);

	for (;;) {
		print("> ");
		syscall(SYS_GETS, input, 255);
		if (strcmp(input, (char*)"q") == 0) {
			break;
		} else if (strcmp(input, (char*)"p") == 0) {
			syscall(SYS_WRITE, buf);
		}
	}

	delete filename;
	delete buf;
	return 0;
}
