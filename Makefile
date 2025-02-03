dir_guard = @mkdir -p build
BINS = build/yhos.img build/kernel_entry.o build/kernel.o build/void.bin build/boot.bin build/lowlevel.o build/syscall.o build/keyboard.o build/shell.o build/std.o
GIT_COMMIT = $(shell git describe --always)
CFLAGS = -I. -Wall -Wpedantic -Werror -Wextra -DGIT_COMMIT='"$(GIT_COMMIT)"'

all: $(BINS)

build/yhos.img: $(BINS)
	$(dir_guard)
	i386-elf-ld -o build/kernel.bin -Ttext 0x1000 build/kernel_entry.o build/kernel.o build/lowlevel.o build/syscall.o build/keyboard.o build/shell.o build/std.o --oformat binary
	cat build/boot.bin build/kernel.bin build/void.bin > build/yhos.bin
	cp build/yhos.bin build/yhos.img

build/kernel_entry.o: kernel/kernel_entry.asm
	$(dir_guard)
	nasm kernel/kernel_entry.asm -f elf -o build/kernel_entry.o

build/keyboard.o: kernel/keyboard.c kernel/keyboard.h kernel/types.h kernel/lowlevel.h
	$(dir_guard)
	i386-elf-gcc -m32 -ffreestanding -g -c kernel/keyboard.c -o build/keyboard.o $(CFLAGS)

build/syscall.o: kernel/syscall.c kernel/syscall.h kernel/types.h kernel/lowlevel.h kernel/keyboard.h
	$(dir_guard)
	i386-elf-gcc -m32 -ffreestanding -g -c kernel/syscall.c -o build/syscall.o $(CFLAGS)

build/lowlevel.o: kernel/lowlevel.c kernel/lowlevel.h kernel/types.h
	$(dir_guard)
	i386-elf-gcc -m32 -ffreestanding -g -c kernel/lowlevel.c -o build/lowlevel.o $(CFLAGS)

build/shell.o: kernel/shell.c kernel/shell.h kernel/syscall.h kernel/std.h kernel/std.c kernel/version.h kernel/lowlevel.h
	$(dir_guard)
	i386-elf-gcc -m32 -ffreestanding -g -c kernel/shell.c -o build/shell.o $(CFLAGS)

build/std.o: kernel/std.c kernel/std.h kernel/syscall.h
	$(dir_guard)
	i386-elf-gcc -m32 -ffreestanding -g -c kernel/std.c -o build/std.o $(CFLAGS)

build/kernel.o: kernel/kernel.c kernel/syscall.h kernel/types.h kernel/keyboard.h kernel/shell.h kernel/version.h
	$(dir_guard)
	i386-elf-gcc -m32 -ffreestanding -g -c kernel/kernel.c -o build/kernel.o $(CFLAGS)

build/void.bin: asm/void.asm
	$(dir_guard)
	nasm asm/void.asm -f bin -o build/void.bin

build/boot.bin: asm/boot.asm
	$(dir_guard)
	nasm asm/boot.asm -f bin -o build/boot.bin
