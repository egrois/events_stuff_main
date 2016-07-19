#!/bin/bash

./clean
./make

version=$1

root=click_trader.${version}

mkdir $root

cp click_trader.param $root
cp cme_md_config.properties $root
cp field_mask $root
cp oms_config.properties $root
cp params.proto $root 
cp params_pb2.py $root
cp user.proto $root
cp user_pb2.py $root


mkdir $root/build
mkdir $root/scripts

cp ./scripts/* $root/scripts
cp ./build/click_trader $root/build

tar -zcvf $root.tar.gz $root

rm -rf $root
