dir_guard = @mkdir -p build

build/zero: build/zero.o
	$(dir_guard)
	i386-elf-ld -o build/zero -nostdlib -Ttext=0x1000 --oformat=binary build/zero.o

build/zero.o: zero.s
	$(dir_guard)
	fasm zero.s build/zero.o
