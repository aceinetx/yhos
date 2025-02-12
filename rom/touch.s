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

	;; cleanup

	mov eax, SYS_FREE
	mov ebx, [filename]
	int 0x80

	leave
	ret

filename: rd 1
buf: rd 1
