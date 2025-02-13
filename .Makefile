##
## yhOS
## Copyright (c) aceinet
## License: GPL-2.0
##
dir_guard = @mkdir -p build
BINS = build/kernel_entry.o build/kernel.o build/void.bin build/boot.bin build/lowlevel.o build/syscall.o build/keyboard.o build/shell.o build/std.o build/test.o build/yalloc.o
GIT_COMMIT = $(shell git describe --always)
# cflags shall be autogenerated by ./makegen
NASM_COLOR = @echo -e -n "\x1b[38;5;94m"
GCC_COLOR = @echo -e -n "\x1b[38;5;244m"
LD_COLOR = @echo -e -n "\x1b[38;5;103m"
DCC_COLOR = @echo -e -n "\x1b[38;5;207m"
RESET_COLOR = @echo -e -n "\x1b[0m"

all: build/yhos.img build/i386-yhse-ld build/i386-yhse-gcc

build/i386-yhse-ld: yhse/i386-yhse-ld.c
	$(GCC_COLOR)
	cc -o $@ $< -DSOURCE_PATH='"$(shell pwd)/yhse"'
	$(RESET_COLOR)

build/i386-yhse-gcc: yhse/i386-yhse-gcc.c
	$(GCC_COLOR)
	cc -o $@ $< -DSOURCE_PATH='"$(shell pwd)/yhse"'
	$(RESET_COLOR)

build/yhos.img: $(BINS) $(MAKEGEN_DEPS)
	$(dir_guard)
	$(LD_COLOR)
	i386-elf-ld -o build/kernel.bin -Ttext 0x1000 build/kernel_entry.o build/kernel.o build/lowlevel.o build/syscall.o build/keyboard.o build/shell.o build/std.o build/test.o build/yalloc.o $(MAKEGEN_DEPS) --oformat binary
	$(RESET_COLOR)
	@cat build/boot.bin build/kernel.bin build/void.bin > build/yhos.bin
	@cp build/yhos.bin build/yhos.img
	@rm -rf build/*.o build/*.bin build/*.ll

build/kernel_entry.o: kernel/kernel_entry.asm
	$(dir_guard)
	$(NASM_COLOR)
	nasm $< -f elf -o $@
	$(RESET_COLOR)

build/keyboard.o: kernel/keyboard.c kernel/keyboard.h kernel/types.h kernel/lowlevel.h
	$(dir_guard)
	$(GCC_COLOR)
	i386-elf-gcc -m32 -ffreestanding -g -c $< -o $@ $(CFLAGS)
	$(RESET_COLOR)

build/syscall.o: kernel/syscall.c kernel/syscall.h kernel/types.h kernel/lowlevel.h kernel/keyboard.h kernel/std.h kernel/yalloc.h
	$(dir_guard)
	$(GCC_COLOR)
	i386-elf-gcc -m32 -ffreestanding -g -c $< -o $@ $(CFLAGS)
	$(RESET_COLOR)

build/lowlevel.o: kernel/lowlevel.c kernel/lowlevel.h kernel/types.h kernel/std.h
	$(dir_guard)
	$(GCC_COLOR)
	i386-elf-gcc -m32 -ffreestanding -g -c $< -o $@ $(CFLAGS)
	$(RESET_COLOR)

build/yalloc.o: kernel/yalloc.c kernel/yalloc.h kernel/std.h kernel/syscall.h kernel/types.h
	$(dir_guard)
	$(GCC_COLOR)
	i386-elf-gcc -m32 -ffreestanding -g -c $< -o $@ $(CFLAGS)
	$(RESET_COLOR)

build/shell.o: kernel/shell.c kernel/shell.h kernel/syscall.h kernel/std.h kernel/std.c kernel/version.h kernel/lowlevel.h kernel/test.asm kernel/yalloc.h
	$(dir_guard)
	$(GCC_COLOR)
	i386-elf-gcc -m32 -ffreestanding -g -c $< -o $@ $(CFLAGS)
	$(RESET_COLOR)

build/std.o: kernel/std.c kernel/std.h kernel/syscall.h
	$(dir_guard)
	$(GCC_COLOR)
	i386-elf-gcc -m32 -ffreestanding -g -c $< -o $@ $(CFLAGS)
	$(RESET_COLOR)

build/kernel.o: kernel/kernel.c kernel/syscall.h kernel/types.h kernel/keyboard.h kernel/shell.h kernel/version.h kernel/std.h kernel/yalloc.h $(ROMFILE)
	$(dir_guard)
	$(GCC_COLOR)
	i386-elf-gcc -m32 -ffreestanding -g -c $< -o $@ $(CFLAGS)
	$(RESET_COLOR)

build/test.o: kernel/test.asm
	$(dir_guard)
	$(NASM_COLOR)
	fasm $< $@
	$(RESET_COLOR)

build/void.bin: asm/void.asm
	$(dir_guard)
	$(NASM_COLOR)
	nasm $< -f bin -o $@
	$(RESET_COLOR)

build/boot.bin: asm/boot.asm
	$(dir_guard)
	$(NASM_COLOR)
	nasm $< -f bin -o $@
	$(RESET_COLOR)
