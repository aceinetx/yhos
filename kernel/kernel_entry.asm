;;
;; yhOS
;; Copyright (c) aceinet
;; License: GPL-2.0
;;
section .text
[bits 32]
[extern main]
[extern syscall_handler]

global isr80_handler

start_main:
    call setup_idt
    call main
    jmp $

setup_idt:
    ;; Create IDT in code section to ensure it's loaded
    mov eax, isr80_handler
    mov word [idt + 0x80*8], ax
    mov word [idt + 0x80*8 + 2], 0x08    ;; CODE_SEG (0x08)
    mov byte [idt + 0x80*8 + 4], 0
    mov byte [idt + 0x80*8 + 5], 0x8E    ;; Present, DPL=0, 32-bit Interrupt Gate
    shr eax, 16
    mov word [idt + 0x80*8 + 6], ax

    lidt [idt_descriptor]
    ret

isr80_handler:
    pusha
    push esp
    call syscall_handler
    add esp, 4
    popa
    iret

section .data
align 8
idt:
    times 256*8 db 0   ;; Ensure 8-byte alignment

idt_descriptor:
    dw 256*8 - 1
    dd idt             ;; Physical address of IDT
