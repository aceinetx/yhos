;; fasm
format ELF
use32
public kernel_test
kernel_test:
	pusha
	push 670006
	mov eax, 1
	mov ebx, esp
	int 0x80
	pop eax
	popa
	ret
