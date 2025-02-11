format ELF
org 0x40000

;; yhse header begin
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

	;; print zeroing out
	mov eax, 1
	mov ebx, msg_zeroing
	int 0x80

	mov eax, 1
	mov ebx, [filename]
	int 0x80

	mov eax, 0
	mov ebx, 10
	int 0x80

	;; get file address
	mov eax, 10
	mov ebx, [filename]
	int 0x80

	cmp eax, 0
	je .error

	add eax, 4
	mov [content], eax

	;; free the file content
	mov eax, 7
	mov ebx, [content]
	mov ebx, [ebx]
	int 0x80

	;; allocate a new file content with a single zero
	mov eax, 6
	mov ebx, 1
	int 0x80

	mov byte [eax], 0
	mov dword [content], eax
	;;mov dword [eax], eax

	jmp .quit
.error:
	;; print no such file
	mov eax, 1
	mov ebx, msg_nofile
	int 0x80
	jmp .quit

.quit:
	leave
	ret

msg_zeroing: db "Zeroing out: ", 0
msg_nofile: db "No such file or directory", 10, 0
filename: rd 1
content: rd 1
