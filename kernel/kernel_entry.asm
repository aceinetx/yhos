section .text
[bits 32]
[extern main]
global interrupt_handler

start_main:
    cli
    call setup_pic      ; Initialize PIC first
    call setup_idt
    sti                ; Now safe to enable interrupts
    call main
    jmp $

setup_pic:
    ; Initialize PICs
    mov al, 0x11
    out 0x20, al       ; ICW1 to master
    out 0xA0, al       ; ICW1 to slave
    
    mov al, 0x20
    out 0x21, al       ; Master vector offset
    mov al, 0x28
    out 0xA1, al       ; Slave vector offset
    
    mov al, 0x04
    out 0x21, al       ; Tell master about slave
    mov al, 0x02
    out 0xA1, al
    
    mov al, 0x01
    out 0x21, al       ; 8086 mode
    out 0xA1, al
    
    mov al, 0xFF
    out 0x21, al       ; Mask all interrupts
    out 0xA1, al
    
    ret

; Add before interrupt_handler
global default_handler
default_handler:
    ; Optional: Add error handling here
    iret

; Update IDT setup code to use default_handler for all entries
setup_idt:
    mov ecx, 256
    mov edi, idt
    mov eax, default_handler
.set_entry:
    mov [edi], word ax         ; Low 16 bits
    mov [edi+2], word 0x08     ; Code segment
    mov [edi+4], byte 0x00     ; Reserved
    mov [edi+5], byte 0x8E     ; Present, DPL=0, 32-bit gate
    shr eax, 16
    mov [edi+6], word ax       ; High 16 bits
    add edi, 8
    loop .set_entry

    ; THEN overwrite the 0x80 entry specifically
    mov edi, idt + 0x80*8
    mov eax, interrupt_handler
    mov [edi], word ax
    mov [edi+2], word 0x08
    mov [edi+4], byte 0x00
    mov [edi+5], byte 0xEE     ; DPL=3 for user-accessible syscalls
    shr eax, 16
    mov [edi+6], word ax
    
    lidt [idt_descriptor]
    ret

interrupt_handler:
    cmp eax, 0              ; Check syscall number
    je print_char
    iret                    ; Return if unknown

print_char:
    mov edx, 0xB8000        ; Video memory address
    mov [edx], bl           ; Write character from BL
    iret

section .data
idt:
    times 256*8 db 0        ; IDT (256 entries)

idt_descriptor:
    dw 256*8 - 1            ; IDT size - 1
    dd idt                  ; IDT address
