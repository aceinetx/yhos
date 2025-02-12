;;
;; yhOS
;; Copyright (c) aceinet
;; License: GPL-2.0
;;

format ELF
org 0x40000

;; yhse header begin
db "YHSE", 0
dd _start
;; yhse header end

public _start
_start:
	push ebp
	mov ebp, esp

	;; get file name (eax)
	mov eax, 9
	int 0x80

	mov [filename], eax

	;; allocate empty buffer
	mov eax, 6
	mov ebx, 1
	int 0x80

	mov [buf], eax
	mov byte [eax], 0

	;; write
	mov eax, 3
	mov ebx, [filename]
	mov ecx, [buf]
	mov edx, 1
	int 0x80

	;; free buffer as write syscall allocates it one more time
	mov eax, 7
	mov ebx, [buf]
	int 0x80

	leave
	ret

filename: rd 1
buf: rd 1
