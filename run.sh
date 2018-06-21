#!/bin/bash
progs="mpi_fast"
modes="seq mutex spin semaforo trans psemaforo"
not_modes="ptrans tbb"
count="1 2 3 4 5 6 7 8 9 10 11"
n_procs="2 4 8 16"
dirs_home="$PWD"

echo Começando a execução a partir de $dirs_home

echo -e "\tExecutando programas"
echo

for n in $n_procs; do
	echo "-----Executando com $n fluxos-----"
	for i in $count; do
		echo "-----Executando run $i-----"
		mpirun

	done
done
