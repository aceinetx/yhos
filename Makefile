dir_guard=@mkdir -p build
all: build/yhos.img build/kernel_entry.o build/kernel.o build/void.bin build/boot.bin

build/yhos.img: build/boot.bin build/kernel_entry.o build/kernel.o build/void.bin
	$(dir_guard)
	i386-elf-ld -o build/kernel.bin -Ttext 0x1000 build/kernel_entry.o build/kernel.o --oformat binary
	cat build/boot.bin build/kernel.bin build/void.bin > build/yhos.bin
	cp build/yhos.bin build/yhos.img

build/kernel_entry.o: kernel/kernel_entry.asm
	$(dir_guard)
	nasm kernel/kernel_entry.asm -f elf -o build/kernel_entry.o

build/kernel.o: kernel/kernel.c
	$(dir_guard)
	i386-elf-gcc -m32 -ffreestanding -g -c kernel/kernel.c -o build/kernel.o

build/void.bin: asm/void.asm
	$(dir_guard)
	nasm asm/void.asm -f bin -o build/void.bin

build/boot.bin: asm/boot.asm
	$(dir_guard)
	nasm asm/boot.asm -f bin -o build/boot.bin
