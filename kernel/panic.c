/*
 * File    : panic.c
 * Author  : Anthony Javier Coreas
 * Date    : 2025-07-20
 * Purpose : Panic handler para YHOS, auto-contenido, verbose, sin dependencias externas.
 */

typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;
typedef unsigned long uintptr_t;

#define VGA_WIDTH   80
#define VGA_HEIGHT  25
#define VGA_ADDRESS ((volatile uint16_t*)0xB8000)
#define COLOR_WHITE 0x0F
#define COLOR_RED   0x4F

static volatile uint16_t* vga_buffer = VGA_ADDRESS;
static uint8_t cursor_row = 0;
static uint8_t cursor_col = 0;
static uint8_t current_color = COLOR_WHITE;

static void vga_setcolor(uint8_t color) {
    current_color = color;
}

static void vga_clear() {
    uint16_t blank = (uint16_t)(' ') | ((uint16_t)current_color << 8);
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_buffer[i] = blank;
    }
    cursor_row = 0;
    cursor_col = 0;
}

static void vga_scroll() {
    for (int row = 1; row < VGA_HEIGHT; row++) {
        for (int col = 0; col < VGA_WIDTH; col++) {
            vga_buffer[(row - 1) * VGA_WIDTH + col] = vga_buffer[row * VGA_WIDTH + col];
        }
    }
    uint16_t blank = (uint16_t)(' ') | ((uint16_t)current_color << 8);
    for (int col = 0; col < VGA_WIDTH; col++) {
        vga_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + col] = blank;
    }
    if (cursor_row > 0) cursor_row--;
}

static void vga_putc(char c) {
    if (c == '\n') {
        cursor_col = 0;
        cursor_row++;
        if (cursor_row >= VGA_HEIGHT) {
            vga_scroll();
            cursor_row = VGA_HEIGHT - 1;
        }
        return;
    }
    if (c == '\r') {
        cursor_col = 0;
        return;
    }
    if (cursor_col >= VGA_WIDTH) {
        cursor_col = 0;
        cursor_row++;
        if (cursor_row >= VGA_HEIGHT) {
            vga_scroll();
            cursor_row = VGA_HEIGHT - 1;
        }
    }
    vga_buffer[cursor_row * VGA_WIDTH + cursor_col] = (uint16_t)c | ((uint16_t)current_color << 8);
    cursor_col++;
}

static void vga_puts(const char* str) {
    while (*str) {
        vga_putc(*str++);
    }
}

static void vga_puthex(uint32_t num) {
    vga_puts("0x");
    for (int i = 7; i >= 0; i--) {
        uint8_t nibble = (num >> (i * 4)) & 0xF;
        char c = (nibble < 10) ? ('0' + nibble) : ('A' + nibble - 10);
        vga_putc(c);
    }
}

static void vga_putdec(int num) {
    if (num == 0) {
        vga_putc('0');
        return;
    }
    if (num < 0) {
        vga_putc('-');
        num = -num;
    }

    char buf[11]; // max digits for 32bit int + null
    int i = 0;
    while (num > 0) {
        buf[i++] = '0' + (num % 10);
        num /= 10;
    }
    for (int j = i - 1; j >= 0; j--) {
        vga_putc(buf[j]);
    }
}

// Simple printf con soporte limitado: %s, %x, %d, %c, %%
static void vga_printf(const char* fmt, ...) {
    __builtin_va_list args;
    __builtin_va_start(args, fmt);

    for (; *fmt != 0; fmt++) {
        if (*fmt == '%') {
            fmt++;
            if (*fmt == '\0') break;
            switch (*fmt) {
                case 's': {
                    const char* s = __builtin_va_arg(args, const char*);
                    vga_puts(s);
                    break;
                }
                case 'x': {
                    uint32_t val = __builtin_va_arg(args, uint32_t);
                    vga_puthex(val);
                    break;
                }
                case 'd': {
                    int val = __builtin_va_arg(args, int);
                    vga_putdec(val);
                    break;
                }
                case 'c': {
                    char c = (char)__builtin_va_arg(args, int);
                    vga_putc(c);
                    break;
                }
                case '%': {
                    vga_putc('%');
                    break;
                }
                default:
                    vga_putc('%');
                    vga_putc(*fmt);
                    break;
            }
        } else {
            vga_putc(*fmt);
        }
    }

    __builtin_va_end(args);
}

static inline uint32_t read_cr0(void) { uint32_t val; asm volatile("mov %%cr0, %0" : "=r"(val)); return val; }
static inline uint32_t read_cr2(void) { uint32_t val; asm volatile("mov %%cr2, %0" : "=r"(val)); return val; }
static inline uint32_t read_cr3(void) { uint32_t val; asm volatile("mov %%cr3, %0" : "=r"(val)); return val; }
static inline uint32_t read_cr4(void) { uint32_t val; asm volatile("mov %%cr4, %0" : "=r"(val)); return val; }

static void print_registers(void) {
    uint32_t eax, ebx, ecx, edx, esi, edi, ebp, esp;
    uint32_t eip, eflags;

    asm volatile ("mov %%eax, %0" : "=r"(eax));
    asm volatile ("mov %%ebx, %0" : "=r"(ebx));
    asm volatile ("mov %%ecx, %0" : "=r"(ecx));
    asm volatile ("mov %%edx, %0" : "=r"(edx));
    asm volatile ("mov %%esi, %0" : "=r"(esi));
    asm volatile ("mov %%edi, %0" : "=r"(edi));
    asm volatile ("mov %%ebp, %0" : "=r"(ebp));
    asm volatile ("mov %%esp, %0" : "=r"(esp));

    asm volatile (
        "call 1f\n"
        "1: pop %0\n"
        : "=r"(eip)
    );

    asm volatile ("pushf\n pop %0" : "=r"(eflags));

    vga_printf("\n--- CPU REGISTERS ---\n");
    vga_printf("EAX= %x  EBX= %x  ECX= %x  EDX= %x\n", eax, ebx, ecx, edx);
    vga_printf("ESI= %x  EDI= %x  EBP= %x  ESP= %x\n", esi, edi, ebp, esp);
    vga_printf("EIP= %x  EFLAGS= %x\n", eip, eflags);
    vga_printf("CR0= %x  CR2= %x  CR3= %x  CR4= %x\n",
               read_cr0(), read_cr2(), read_cr3(), read_cr4());
}

static void print_stack_trace(void){
    vga_printf("\n--- STACK TRACE (max 16 frames) ---\n");

    uint32_t *ebp;
    asm volatile ("mov %%ebp, %0" : "=r"(ebp));

    for (int i = 0; i < 16; i++) {
        if (!ebp || (uintptr_t)ebp < 0x1000 || (uintptr_t)ebp > 0xF0000000) break;

        uint32_t ret_addr = *(ebp + 1);
        if (!ret_addr) break;

        vga_printf("Frame %d: 0x%x\n", i, ret_addr);

        ebp = (uint32_t *)(*ebp);
    }
}

void panic(const char *message) {
    // Disable interrupts
    asm volatile ("cli");

    vga_setcolor(COLOR_WHITE | (COLOR_RED << 4));
    vga_clear();

    vga_printf("\n*** YHOS PANIC ***\n");
    vga_printf("Fatal error: %s\n\n", message);

    print_registers();
    print_stack_trace();

    vga_printf("\nSystem halted. Reset or power off.\n");

    while (1) {
        asm volatile ("hlt");
    }
}

