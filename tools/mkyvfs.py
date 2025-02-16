##
## yhOS
## Copyright (c) aceinet
## License: GPL-2.0
##
import os, sys

print("yhOS vfs ROM generator")

argc = len(sys.argv)

if argc < 2:
    print("(no files are provided)")
    exit(1)

cgen = f"// Auto-generated ROM using {sys.argv[0]}"
cgen += """
#pragma once
#include <kernel/syscall.h>
void set_rom(){
"""

filename_i = 0
total_bytes = 0
for _filename in sys.argv[1::]:
    filename = os.path.basename(_filename)
    with open(_filename, "+rb") as f:
        print(f"[...] processing \x1b[38;5;2m{filename}\x1b[0m")
        cgen += f"unsigned char dat{filename_i}[] = "+"{"

        for ch in f.read():
            cgen += hex(ch)+","
            total_bytes += 1

        if cgen.endswith(","):
            cgen = cgen[:-1]

        cgen += ", 0x0};\n"
        cgen += f"syscall(SYS_VFSWRITE, \"/{filename}\", dat{filename_i}, sizeof(dat{filename_i}));\n"

        filename_i += 1
        pass

cgen += """}"""

with open("rom.h", "+w") as f:
    f.write(cgen)
print(f"[...] Total size of ROM: {round(total_bytes, 3)}B, {round(total_bytes/1024, 3)}kB, {round(total_bytes/1024/1024, 3)}mB")
print(f"[...] ROM written to rom.h")
