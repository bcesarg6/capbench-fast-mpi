#!/bin/bash
count="2 3 4 5 6 7 8 9 10 11"
n_procs="1 3 5 9 17"
dirs_home="$PWD"

echo Começando a execução a partir de $dirs_home

echo -e "\tLendo resultados"
echo

cd saida

rm main_out_med.txt
touch main_out_med.txt

for n in $n_procs; do
      echo "-----Lendo FAST com $n fluxos-----"
      rm main_out_$n.txt
      touch main_out_$n.txt
      for i in $count; do
           grep -i "time spent" ./$n/out_$i.txt | awk '{print $3}' >>main_out_$n.txt
           grep -i "total time" ./$n/out_$i.txt | awk '{print $3}' >>main_out_$n.txt
           echo "" >>main_out_$n.txt
      done
      python orgFAST.py main_out_$n.txt >>main_out_med.txt
      echo "" >>main_out_med.txt
done
