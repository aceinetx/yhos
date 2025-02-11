#!/bin/bash
set -e

cp build ../tools -r
cd ../tools
rm build/*.o
python mkyvfs.py build/*
mv rom.h ../kernel
rm build -rf
cd ../rom
