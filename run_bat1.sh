#!/bin/sh
set -eu

bat_dir=$(realpath Bat1)

if ! [ -x "$bat_dir/agm" ]; then
  g++ -o "$bat_dir/agm" "$bat_dir/agm.cpp"
fi

for name in dijkstra kruskal prim; do
  OUT_FILE="$bat_dir/$name.bin" make -C $name
done

cd "$bat_dir"
exec ./Bat1.sh
