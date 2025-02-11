#!/usr/bin/bash
##
## yhOS
## Copyright (c) aceinet
## License: GPL-2.0
##
set -e

rm kernel/rom.h -f

cd rom
echo Building ROM
./makerom.sh
cd ..

echo Building yhOS
./makegen
make
