##
## yhOS
## Copyright (c) aceinet
## License: GPL-2.0
##
dir_guard = @mkdir -p build
text_start = 0x40000
CFLAGS = -I.. -Wall -Werror

all: build/touch build/zero build/append build/readyhse

build/append: build/append.o
	$(dir_guard)
	i386-elf-ld -o $@ -nostdlib -Ttext=$(text_start) --oformat=binary $<

build/append.o: append.c yhos.h
	$(dir_guard)
	i386-elf-gcc -o $@ -c -m32 -ffreestanding $< $(CFLAGS)

build/readyhse: build/readyhse.o
	$(dir_guard)
	i386-elf-ld -o $@ -nostdlib -Ttext=$(text_start) --oformat=binary $<

build/readyhse.o: readyhse.c yhos.h
	$(dir_guard)
	i386-elf-gcc -o $@ -c -m32 -ffreestanding $< $(CFLAGS)

build/touch: build/touch.o
	$(dir_guard)
	i386-elf-ld -o $@ -nostdlib -Ttext=$(text_start) --oformat=binary $<

build/touch.o: touch.s yhos.inc
	$(dir_guard)
	fasm $< $@

build/zero: build/zero.o
	$(dir_guard)
	i386-elf-ld -o $@ -nostdlib -Ttext=$(text_start) --oformat=binary $<

build/zero.o: zero.s yhos.inc
	$(dir_guard)
	fasm $< $@

build/std.o: ../kernel/std.c
	i386-elf-gcc ../kernel/std.c -o build/std.o -ffreestanding -I.. -m32 -c
