##
## yhOS
## Copyright (c) aceinet
## License: GPL-2.0
##
dir_guard = @mkdir -p build
text_start = 0x40000
CFLAGS = -I.. -Wall -Werror -I../yhse/include -Wno-sometimes-uninitialized -Wno-implicit-exception-spec-mismatch
LDFLAGS =
LD = ../yhse/i386-yhse-ld
CC = ../yhse/i386-yhse-gcc
CPP = ../yhse/i386-yhse-g++

all: build/touch build/zero build/append build/readyhse build/cpptest build/echo build/typing build/example build/seq build/ed

build/example: build/example.o
	$(dir_guard)
	$(LD) $(LDFLAGS) -o $@ $<

build/example.o: example.c
	$(dir_guard)
	$(CC) -c -o $@ $< $(CFLAGS)

build/ed: build/ed.o
	$(dir_guard)
	$(LD) $(LDFLAGS) -o $@ $<

build/ed.o: ed.cpp
	$(dir_guard)
	$(CPP) -c -o $@ $< $(CFLAGS)

build/cpptest: build/cpptest.o
	$(dir_guard)
	$(LD) $(LDFLAGS) -o $@ $<

build/cpptest.o: cpptest.cpp
	$(dir_guard)
	$(CPP) -c -o $@ $< $(CFLAGS)

build/append: build/append.o
	$(dir_guard)
	$(LD) $(LDFLAGS) -o $@ $<

build/append.o: append.c
	$(dir_guard)
	$(CC) -c -o $@ $< $(CFLAGS)

build/readyhse: build/readyhse.o
	$(dir_guard)
	$(LD) $(LDFLAGS) -o $@ $<

build/readyhse.o: readyhse.c
	$(dir_guard)
	$(CC) -c -o $@ $< $(CFLAGS)

build/touch: build/touch.o
	$(dir_guard)
	$(LD) $(LDFLAGS) -o $@ $<

build/touch.o: touch.s yhos.inc
	$(dir_guard)
	fasm $< $@

build/zero: build/zero.o
	$(dir_guard)
	$(LD) $(LDFLAGS) -o $@ $<

build/zero.o: zero.s yhos.inc
	$(dir_guard)
	fasm $< $@

build/typing: build/typing.o
	$(dir_guard)
	$(LD) $(LDFLAGS) -o $@ $<

build/typing.o: typing.s yhos.inc
	$(dir_guard)
	fasm $< $@

build/seq: build/seq.o
	$(dir_guard)
	$(LD) $(LDFLAGS) -o $@ $<

build/seq.o: seq.s yhos.inc
	$(dir_guard)
	fasm $< $@

build/echo: build/echo.o
	$(dir_guard)
	$(LD) $(LDFLAGS) -o $@ $<

build/echo.o: echo.s yhos.inc
	$(dir_guard)
	fasm $< $@

build/std.o: ../kernel/std.c
	i386-elf-gcc ../kernel/std.c -o build/std.o -ffreestanding -I.. -m32 -c
