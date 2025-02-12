##
## yhOS
## Copyright (c) aceinet
## License: GPL-2.0
##
dir_guard = @mkdir -p build

all: build/touch build/zero

build/touch: build/touch.o
	$(dir_guard)
	i386-elf-ld -o $@ -nostdlib -Ttext=0x1000 --oformat=binary $<

build/touch.o: touch.s
	$(dir_guard)
	fasm $< $@

build/zero: build/zero.o
	$(dir_guard)
	i386-elf-ld -o $@ -nostdlib -Ttext=0x1000 --oformat=binary $<

build/zero.o: zero.s
	$(dir_guard)
	fasm $< $@
