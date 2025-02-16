#!/usr/bin/bash
##
## yhOS
## Copyright (c) aceinet
## License: GPL-2.0
##
set -e

rm kernel/rom.h -f

cd rom
echo -e "Building \x1b[1mROM\x1b[0m"
echo ++++++++++++
./makerom.sh
cd ..
echo ------------

echo -e "Building \x1b[1myhOS\x1b[0m"
echo +++++++++++++
./makegen
echo
make
echo -------------
