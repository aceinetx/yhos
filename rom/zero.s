;;
;; yhOS
;; Copyright (c) aceinet
;; License: GPL-2.0
;;

include 'yhos.inc'

public _start
_start:
	push ebp
	mov ebp, esp

	;; get file name (eax)
	mov eax, SYS_EXEARG
	int 0x80

	mov [filename], eax

	;; print zeroing out
	mov eax, SYS_WRITE
	mov ebx, msg_zeroing
	int 0x80

	mov eax, SYS_WRITE
	mov ebx, [filename]
	int 0x80

	mov eax, SYS_WRITEC
	mov ebx, 10
	int 0x80

	;; get file address
	mov eax, SYS_VFSHANDLE
	mov ebx, [filename]
	int 0x80

	cmp eax, 0
	je .error

	add eax, 4
	mov [content], eax

	;; free the file content
	mov eax, SYS_FREE
	mov ebx, [content]
	mov ebx, [ebx]
	int 0x80

	;; allocate a new file content with a single zero
	mov eax, SYS_ALLOC
	mov ebx, 1
	int 0x80

	mov byte [eax], 0
	mov ebx, eax

	mov eax, [content]
	mov [eax], ebx

	mov eax, 0
	push eax
	jmp .quit
.error:
	;; print no such file
	mov eax, SYS_WRITE
	mov ebx, msg_nofile
	int 0x80

	mov eax, 1
	push eax
	jmp .silent_quit

.quit:
	mov eax, 1
	mov ebx, msg_done
	int 0x80
.silent_quit:

	;; cleanup

	mov eax, SYS_FREE
	mov ebx, [filename]
	int 0x80
	pop eax

	leave
	ret

msg_zeroing: db "Zeroing out: ", 0
msg_nofile: db "No such file or directory", 10, 0
msg_done: db "Done", 10, 0
filename: rd 1
content: rd 1
