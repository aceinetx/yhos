#!/bin/bash
set -e

make

cp build ../tools -r
cp etc/* ../tools/build -r
cd ../tools
rm build/*.o
python mkyvfs.py build/*
mv rom.h ../kernel
rm build -rf
cd ../rom
