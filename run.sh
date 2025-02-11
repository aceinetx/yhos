#!/bin/bash
set -e

#if [ ! -f Makefile ]; then
#	chmod +x makegen
#	./makegen
#fi

#make
./buildall.sh
qemu-system-x86_64 -drive format=raw,file="build/yhos.bin",index=0,if=floppy,  -m 128M $@
