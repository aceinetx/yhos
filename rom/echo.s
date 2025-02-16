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

.loop:
	mov eax, SYS_EXEARG
	int 0x80

	cmp byte [eax], 0
	je .quit
	;;;;;;;;;;

	push eax
	push eax
	
	mov eax, SYS_WRITE
	pop ebx
	int 0x80

	mov eax, SYS_FREE
	pop ebx
	int 0x80

	mov eax, SYS_WRITEC
	mov ebx, ' '
	int 0x80

	;;;;;;;;;;
	jmp .loop

.quit:
	push eax
	mov eax, SYS_FREE
	pop ebx
	int 0x80
	
	mov eax, SYS_WRITEC
	mov ebx, 10
	int 0x80

	mov eax, 0
	leave
	ret
