#!/bin/bash
set -e

cp build ../tools -r
cd ../tools
python mkyvfs.py build/*
mv rom.h ../kernel
rm build -rf
cd ../rom
