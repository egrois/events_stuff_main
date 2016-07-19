#!/bin/bash

dirs="
snapgen
click_trader
"


for dir in $dirs
do
    cd $dir
    rm -rf build
    mkdir build
    meson build
    ./make
    cd ../
done 
