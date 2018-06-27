#!/bin/bash

echo "Compilando programas FAST"
echo ""
cd code
./compile_fast.sh
echo "Pronto"
echo ""

echo "Compilando relatório"
echo ""
cd ../relatorio
./compile_relatorio.sh
echo "Pronto"
echo ""
