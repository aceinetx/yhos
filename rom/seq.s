;;
;; yhOS
;; Copyright (c) aceinet
;; License: GPL-2.0
;;

include 'yhos.inc'

BUF_LEN = 10

public _start
_start:
	push ebp
	mov ebp, esp

	mov eax, SYS_EXEARG
	int 0x80

	mov [num_s], eax

	cmp byte [eax], 0
	je .quit

	;;;;;;;;

	mov eax, SYS_STOI
	mov ebx, [num_s]
	int 0x80
	mov [num], eax

	mov edx, 1
.loop:
	cmp edx, dword [num]
	jg .quit

	push edx

	mov eax, SYS_ITOA
	mov ebx, edx
	mov ecx, buf
	mov edx, BUF_LEN
	int 0x80

	mov eax, SYS_WRITE
	mov ebx, buf
	int 0x80

	mov eax, SYS_WRITEC
	mov ebx, 10
	int 0x80

	pop edx

	inc edx

	jmp .loop

.quit:
	mov eax, SYS_FREE
	mov ebx, [num_s]
	int 0x80

	mov eax, 0
	leave
	ret

num_s: dd 0
num: dd 0
buf: rb BUF_LEN
