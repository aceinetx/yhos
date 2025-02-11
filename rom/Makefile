dir_guard = @mkdir -p build

build/ed: build/ed.o
	$(dir_guard)
	i386-elf-ld -o build/ed -nostdlib -Ttext=0x1000 --oformat=binary build/ed.o

build/ed.o: ed.s
	$(dir_guard)
	fasm ed.s build/ed.o
