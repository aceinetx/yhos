all: build/yhos.img build/kernel_entry.o build/kernel.o build/void.bin build/boot.bin

build/yhos.img: build/boot.bin build/kernel_entry.o build/kernel.o build/void.bin
	i386-elf-ld -o build/kernel.bin -Ttext 0x1000 build/kernel_entry.o build/kernel.o --oformat binary
	cat build/boot.bin build/kernel.bin build/void.bin > build/yhos.bin
	cp build/yhos.bin build/yhos.img

build/kernel_entry.o: kernel/kernel_entry.asm
	nasm kernel/kernel_entry.asm -f elf -o build/kernel_entry.o

build/kernel.o: kernel/kernel.c
	i386-elf-gcc -m32 -ffreestanding -g -c kernel/kernel.c -o build/kernel.o

build/void.bin: asm/void.asm
	nasm asm/void.asm -f bin -o build/void.bin

build/boot.bin: asm/boot.asm
	nasm asm/boot.asm -f bin -o build/boot.bin
