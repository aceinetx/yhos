# yhos
DOS-like operating system for x86 legacy hardware. Here are it's features:
- Shell
- System calls via int 0x80
- Own executable format: yhSE (yhOS Static Executable, toolchain included)
- Virtual file system stored in RAM
- Dynamic memory allocator
## Build
Requires i386-elf toolchain, nasm, fasm and dcc (if available). To install the toolchain use homebrew
```
./make
```
## Run
Make sure you have qemu-system-i386 installed
```
./run.sh
```
## Makefile usage
You should sometimes use ./ymake and not make since ./ymake also rebuilds the makefile
