;;
;; yhOS
;; Copyright (c) aceinet
;; License: GPL-2.0
;;

include 'yhos.inc'

usage:
	push eax
	push ebx

	mov eax, SYS_WRITE
	mov ebx, usage_msg
	int 0x80

	pop ebx
	pop eax
	ret

public _start
_start:
	push ebp
	mov ebp, esp

	;; get file name (eax)
	mov eax, SYS_EXEARG
	int 0x80

	mov [filename], eax

	cmp byte [eax], 0
	jne .arg_valid

	call usage

	mov eax, 0
	push eax
	jmp .quit

.arg_valid:
	;; allocate empty buffer
	mov eax, SYS_ALLOC
	mov ebx, 1
	int 0x80

	mov [buf], eax
	mov byte [eax], 0

	;; write
	mov eax, SYS_VFSWRITE
	mov ebx, [filename]
	mov ecx, [buf]
	mov edx, 1
	int 0x80

	;; free buffer as write syscall allocates it one more time
	mov eax, SYS_FREE
	mov ebx, [buf]
	int 0x80

	mov eax, 0
	push eax
.quit:
	;; cleanup

	mov eax, SYS_FREE
	mov ebx, [filename]
	int 0x80

	pop eax
	leave
	ret

usage_msg: db "Usage: touch [filename]", 10, 0
filename: rd 1
buf: rd 1
