#!/bin/bash

echo "Compilando gráficos"
cd graphs
make
cp *.pdf ../
echo ""

echo "Compilano TEX"
cd ..
make >ttt
make >ttt
rm ttt
echo ""
