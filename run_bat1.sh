#!/bin/sh
set -eu

bat_dir=$1

if ! [ -e "$bat_dir/agm" ]; then
  g++ -o "$bat_dir/agm" "$bat_dir/agm.cpp"
fi

for name in dijkstra; do
  make -C $name BINEXT=.bin
  cp $name/$name.bin "$bat_dir"
done

cd "$bat_dir"
exec ./Bat1.sh
