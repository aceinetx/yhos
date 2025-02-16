#!/usr/bin/bash
##
## yhOS
## Copyright (c) aceinet
## License: GPL-2.0
##
set -e

rm kernel/rom.h -f

echo -e "Building \x1b[1mROM\x1b[0m"
echo ++++++++++++
cd rom
./makerom.sh
cd ..
echo ------------

echo -e "Building \x1b[1myhOS\x1b[0m"
echo +++++++++++++
echo
./ymake
echo -------------
