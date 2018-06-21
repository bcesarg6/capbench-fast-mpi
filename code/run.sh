#!/bin/bash
progs="mpi_fast"
not_modes="ptrans tbb"
count="1 2 3 4 5 6 7 8 9 10 11"
n_procs=('3' '5' '9' '17')
host_files=('hosts2' 'hotsts4' 'hosts8' 'hosts16')
dirs_home="$PWD"

echo Começando a execução a partir de $dirs_home

echo -e "\tExecutando programas"
echo

cd mpi_FAST

for (( p=0; p<${#n_procs[@]}; p++ )); do
	for i in $count; do
		hostfile=${host_files[${p}]}
		n=${n_procs[${p}]}
		echo "-----Executando run $i : $n com hostfile $hostfile -----"

		mpirun -np $n -hostfile $hostfile mpi_fast --verbose --class huge >./../../saida/$n/out_$i.txt

	done

	echo "---"
done

# mpirun -np $n -hostfile $hostfile mpi_fast --verbose --class huge >./../saida/$n/out_$i.txt
