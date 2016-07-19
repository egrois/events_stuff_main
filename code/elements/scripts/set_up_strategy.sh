#!/bin/bash

name=$1
mkdir $name
cd $name
ln -s ../scripts
ln -s ../params.proto
cp ../templates/* .
sed -i -e "s/REPLACE/$name/g" meson.build 
sed -i -e "s/REPLACE/$name/g" main.cpp 
sed -i -e "s/REPLACE/$name/g" REPLACE.param
mv REPLACE.param $name.param
mkdir build
meson build
./make
