#!/usr/bin/bash
set -e

rm kernel/rom.h -f

cd rom
echo Building ROM
./makerom.sh
cd ..

echo Building yhOS
./makegen
make
