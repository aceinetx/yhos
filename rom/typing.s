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

	mov eax, SYS_GETC
	int 0x80

	mov dx, 0
.loop:
	cmp dx, 10
	je .quit

	mov eax, SYS_GETC
	int 0x80
	mov edx, eax

	mov eax, SYS_WRITEC
	mov ebx, edx
	int 0x80

	jmp .loop

.quit:
	mov eax, 0
	leave
	ret
