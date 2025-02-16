#!/usr/bin/bash
##
## yhOS
## Copyright (c) aceinet
## License: GPL-2.0
##
set -e

make

cp build ../tools -r
cp etc/* ../tools/build -r
cd ../tools
rm build/*.o
echo
python mkyvfs.py build/*
mv rom.h ../kernel
rm build -rf
cd ../rom
