#!/bin/bash

echo "Compilando gráficos"
cd graphs
make
cp *.pdf ../
echo ""

echo "Compilando TEX"
cd ..
make >ttt
make >ttt
rm ttt
echo ""
