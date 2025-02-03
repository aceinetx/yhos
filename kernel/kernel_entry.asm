section .text
[bits 32]
[extern main]

start_main:
    call main
    jmp $
