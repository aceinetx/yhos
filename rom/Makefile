##
## yhOS
## Copyright (c) aceinet
## License: GPL-2.0
##
dir_guard = @mkdir -p build
text_start = 0x40000
CFLAGS = -I.. -Wall -Werror -I../yhse/include
LD = ../yhse/i386-yhse-ld
CC = ../yhse/i386-yhse-gcc
CPP = ../yhse/i386-yhse-g++

all: build/touch build/zero build/append build/readyhse build/cpptest

build/cpptest: build/cpptest.o
	$(dir_guard)
	# i386-elf-ld -o $@ -nostdlib -Ttext=$(text_start) --oformat=binary $<
	$(LD) -o $@ $<

build/cpptest.o: cpptest.cpp
	$(dir_guard)
	# i386-elf-gcc -o $@ -c -m32 -ffreestanding $< $(CFLAGS)
	$(CPP) -c -o $@ $< $(CFLAGS)

build/append: build/append.o
	$(dir_guard)
	# i386-elf-ld -o $@ -nostdlib -Ttext=$(text_start) --oformat=binary $<
	$(LD) -o $@ $<

build/append.o: append.c
	$(dir_guard)
	# i386-elf-gcc -o $@ -c -m32 -ffreestanding $< $(CFLAGS)
	$(CC) -c -o $@ $< $(CFLAGS)

build/readyhse: build/readyhse.o
	$(dir_guard)
	# i386-elf-ld -o $@ -nostdlib -Ttext=$(text_start) --oformat=binary $<
	$(LD) -o $@ $<

build/readyhse.o: readyhse.c
	$(dir_guard)
	# i386-elf-gcc -o $@ -c -m32 -ffreestanding $< $(CFLAGS)
	$(CC) -c -o $@ $< $(CFLAGS)

build/touch: build/touch.o
	$(dir_guard)
	# i386-elf-ld -o $@ -nostdlib -Ttext=$(text_start) --oformat=binary $<
	$(LD) -o $@ $<

build/touch.o: touch.s yhos.inc
	$(dir_guard)
	fasm $< $@

build/zero: build/zero.o
	$(dir_guard)
	# i386-elf-ld -o $@ -nostdlib -Ttext=$(text_start) --oformat=binary $<
	$(LD) -o $@ $<

build/zero.o: zero.s yhos.inc
	$(dir_guard)
	fasm $< $@

build/std.o: ../kernel/std.c
	i386-elf-gcc ../kernel/std.c -o build/std.o -ffreestanding -I.. -m32 -c
