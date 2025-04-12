/*
 * yhOS
 * Copyright (c) aceinet
 * License: GPL-2.0
 */
#include <kernel/keyboard.h>
#include <kernel/lowlevel.h>
#include <kernel/shell.h>
#include <kernel/std.h>
#include <kernel/syscall.h>
#include <kernel/yalloc.h>

vfs_file* vfs;
dword vfs_size;
char cwd[PATH_LIMIT];

dword do_syscall(dword eax, dword ebx, dword ecx, dword edx) {
	asm volatile("int $0x80" : : "a"(eax), "b"(ebx), "c"(ecx), "d"(edx));
	asm volatile("push %eax\n");
	dword ret;
	asm volatile("pop %0\n" : "=a"(ret));
	return ret;
}

void syscall_handler(regs* r) {
	dword syscall_num = r->eax;
	if (syscall_num == SYS_WRITEC) {
		char c = (char)r->ebx;
		vga_putc(c);
	} else if (syscall_num == SYS_WRITE) {
		char* c = (char*)r->ebx;
		while (*c != 0) {
			vga_putc(*c);
			c++;
		}
	} else if (syscall_num == SYS_GETS) {
		// string in ebx
		// length in ecx
		char* c = (char*)r->ebx;
		for (;;) {
			keyboard_result kc = keyboard_handle_input();
			byte is_not_full = (dword)c < (dword)(r->ebx + r->ecx);
			byte is_empty = (dword)c == r->ebx;

			if (kc.ch == '\n' && kc.is_valid) {
				vga_putc(kc.ch);
				*c = '\0';
				break;
			}

			if (kc.is_valid && is_not_full) {
				*c = kc.ch;
				vga_putc(kc.ch);
				c++;
			} else {
				if (kc.ch == 0x0E && !is_empty) {
					vgavec2 pos = get_cursor_pos();
					pos.x -= 1;
					set_cursor_pos(pos.x, pos.y);

					vga_putc(' ');

					set_cursor_pos(pos.x, pos.y);
					c--;
					*c = 0;
					keyboard_handle_input();
				}
			}
		}
	} else if (syscall_num == SYS_VFSWRITE) {
		char* _filename = (char*)r->ebx;
		char* buf = (char*)r->ecx;
		dword filename_size = strlen(_filename);
		dword buf_size = r->edx;
		r->eax = 0;

		dword cwd_size = strlen(cwd);
		char filename[filename_size + cwd_size + 1];
		if (_filename[0] != '/') {

			strncpy(filename, cwd, cwd_size);
			strncpy(filename + cwd_size, _filename, filename_size + 1);

			filename_size += cwd_size;
		} else {
			strncpy(filename, _filename, filename_size + 1);
		}

		if (vfs == NULL) {
			vfs = yalloc(sizeof(vfs_file));

			vfs_file file;
			char* vbuf = yalloc(buf_size);
			memcpy(vbuf, buf, buf_size);

			char* vfilename = yalloc(filename_size + 1);
			memcpy(vfilename, filename, filename_size + 1);

			file.size = buf_size;
			file.content = vbuf;
			file.name = vfilename;

			vfs[0] = file;

			r->eax = (dword)vfs;
			vfs_size = 1;
		} else {
			for (dword i = 0; i < vfs_size; i++) {
				vfs_file* file = &vfs[i];
				if (file->content == NULL)
					continue;

				if (strcmp(file->name, filename) == 0) {
					char* new_content = yalloc(buf_size);
					memcpy(new_content, buf, buf_size);
					yfree(file->content);
					file->content = new_content;
					file->size = buf_size;

					r->eax = (dword)file;
					break;
				}
			}

			if (r->eax == 0) {
				vfs_file* new_vfs = yalloc(sizeof(vfs_file) * (vfs_size + 1));
				memcpy(new_vfs, vfs, sizeof(vfs_file) * vfs_size);
				yfree(vfs);
				vfs = new_vfs;

				vfs_file file;
				char* vbuf = yalloc(buf_size);
				memcpy(vbuf, buf, buf_size);

				char* vfilename = yalloc(filename_size + 1);
				memcpy(vfilename, filename, filename_size + 1);

				file.size = buf_size;
				file.content = vbuf;
				file.name = vfilename;

				new_vfs[vfs_size] = file;
				r->eax = (dword)(&new_vfs[vfs_size]);

				vfs_size++;
			}
		}

		if (r->eax == 0) {
			if (filename[0] != '/') {
				char fbuf[255];
				strncpy(fbuf, cwd, 255);
				strncpy(fbuf + strlen(cwd), filename, 255);
				r->eax = syscall(SYS_VFSWRITE, fbuf, buf, buf_size);
			}
		}
	} else if (syscall_num == SYS_VFSREAD) {
		char* _filename = (char*)r->ebx;
		char* buf = (char*)r->ecx;
		dword buf_size = r->edx;
		dword filename_size = strlen(_filename);
		r->eax = 0;

		dword cwd_size = strlen(cwd);
		char filename[filename_size + cwd_size + 1];
		if (_filename[0] != '/') {

			strncpy(filename, cwd, cwd_size);
			strncpy(filename + cwd_size, _filename, filename_size + 1);

			filename_size += cwd_size;
		} else {
			strncpy(filename, _filename, filename_size + 1);
		}

		if (vfs != NULL) {
			for (dword i = 0; i < vfs_size; i++) {
				vfs_file* file = &vfs[i];
				if (file->content == NULL)
					continue;

				if (strcmp(file->name, filename) == 0) {
					memcpy(buf, file->content, buf_size);
					r->eax = (dword)file;
					break;
				}
			}
		}
	} else if (syscall_num == SYS_VFSQUERY) {
		char* _filename = (char*)r->ebx;
		dword filename_size = strlen(_filename);
		r->eax = -1;

		dword cwd_size = strlen(cwd);
		char filename[filename_size + cwd_size + 1];
		if (_filename[0] != '/') {

			strncpy(filename, cwd, cwd_size);
			strncpy(filename + cwd_size, _filename, filename_size + 1);

			filename_size += cwd_size;
		} else {
			strncpy(filename, _filename, filename_size + 1);
		}

		if (vfs != NULL) {
			for (dword i = 0; i < vfs_size; i++) {
				vfs_file* file = &vfs[i];
				if (file->content == NULL)
					continue;

				if (strcmp(file->name, filename) == 0) {
					r->eax = file->size;
					break;
				}
			}
		}
	} else if (syscall_num == SYS_ALLOC) {
		r->eax = (dword)yalloc(r->ebx);
	} else if (syscall_num == SYS_FREE) {
		yfree((void*)r->ebx);
	} else if (syscall_num == SYS_VFSBASE) {
		r->eax = (dword)vfs;
	} else if (syscall_num == SYS_EXEARG) {
		nextarg();
		r->eax = (dword)yalloc(SHELL_CONST1);
		memcpy((void*)r->eax, arg_buf, SHELL_CONST1);
	} else if (syscall_num == SYS_VFSHANDLE) {
		char* _filename = (char*)r->ebx;
		dword filename_size = strlen(_filename);
		r->eax = 0;

		dword cwd_size = strlen(cwd);
		char filename[filename_size + cwd_size + 1];
		if (_filename[0] != '/') {

			strncpy(filename, cwd, cwd_size);
			strncpy(filename + cwd_size, _filename, filename_size + 1);

			filename_size += cwd_size;
		} else {
			strncpy(filename, _filename, filename_size + 1);
		}

		if (vfs != NULL) {
			for (dword i = 0; i < vfs_size; i++) {
				vfs_file* file = &vfs[i];
				if (file->content == NULL)
					continue;

				if (strcmp(file->name, filename) == 0) {
					r->eax = (dword)file;
					break;
				}
			}
		}
	} else if (syscall_num == SYS_ITOA) {
		dword num = r->ebx;
		char* buf = (char*)r->ecx;
		dword buf_size = r->edx;

		dword i = 0;

		for (;;) {
			if (num == 0 || i >= buf_size) {
				if (i == 0)
					memcpy(buf + i, "0\0", 2);
				break;
			}

			dword digit = num % 10;
			num /= 10;

			buf[i] = '0' + digit;
			buf[i + 1] = 0;

			i++;
		}

		char* temp = yalloc(buf_size);
		memcpy(temp, buf, buf_size);

		dword slen = i - 1;
		for (int i = slen; i >= 0; i--) {
			if (temp[slen - i] == 0)
				continue;
			buf[i] = temp[slen - i];
		}

		yfree(temp);
	} else if (syscall_num == SYS_ITOA16) {
		dword num = r->ebx;
		char* buf = (char*)r->ecx;
		dword buf_size = r->edx;

		dword i = 0;

		for (;;) {
			if (num == 0 || i >= buf_size) {
				if (i == 0)
					memcpy(buf + i, "0\0", 2);
				break;
			}

			dword digit = num % 16;
			num /= 16;

			if (digit < 10) {
				buf[i] = '0' + digit;
			} else {
				buf[i] = 'A' + (digit - 10);
			}
			buf[i + 1] = 0; // FIXME: Potential buffer overflow if (i+1 >= buf_size)

			i++;
		}

		// Reverse the string
		char* temp = yalloc(buf_size);

		memcpy(temp, buf, buf_size);

		dword slen = i - 1;
		for (int j = slen; j >= 0; j--) {
			if (temp[slen - j] == 0)
				continue;
			buf[j] = temp[slen - j];
		}

		yfree(temp);
	} else if (syscall_num == SYS_GETCWD) {
		r->eax = (dword)cwd;
	} else if (syscall_num == SYS_GETC) {
		keyboard_result kc = keyboard_handle_input();
		r->eax = (dword)kc.ch;
	} else if (syscall_num == SYS_STOI) {
		char* s = (char*)r->ebx;
		r->eax = 0;

		while (*s != 0) {
			r->eax *= 10;
			r->eax += *s - '0';
			s++;
		}
	}
}
