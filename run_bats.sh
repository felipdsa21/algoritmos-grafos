#!/bin/sh
set -eu

# Compilando algoritmos

if ! [ -x Bat1/agm ]; then
  g++ -o Bat1/agm Bat1/agm.cpp
fi

for name in dijkstra kosaraju kruskal prim; do
  OUT_DIR=../Bat1/ make -C $name
done

OUT_DIR=../BatExtra/ make -C bellman

# Executando Bat1
(
  cd Bat1
  exec ./Bat1.sh
)

# Executando BatExtra
(
  cd BatExtra
  exec ./BatExtra.sh
)
