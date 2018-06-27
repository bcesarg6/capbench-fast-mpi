#!/bin/bash
count="1 2 3 4 5 6 7 8 9 10 11"
n_procs=('3' '5' '9' '17')
host_files=('hosts2' 'hotsts4' 'hosts8' 'hosts16')
dirs_home="$PWD"
export MPIP="-f ./saida/3"

echo Começando a execução a partir de $dirs_home

echo -e "\tExecutando programas"
echo

cd code/FAST

for i in $count; do
	echo "-----Executando run $i : 1 -----"
	./fast --nthreads 1 --verbose --class huge >./../../saida/1/out_$i.txt
done
echo "---"

cd ../mpi_FAST

for (( p=0; p<${#n_procs[@]}; p++ )); do
	hostfile=${host_files[${p}]}
	n=${n_procs[${p}]}

	for i in $count; do
		echo "-----Executando run $i : $n com hostfile $hostfile -----"

		mpirun -np $n -hostfile hostfile mpi_fast --verbose --class huge >./../../saida/$n/out_$i.txt
	done

	echo "-----Executando run com mpiP : $n com hostfile $hostfile -----"
	mpirun -np -hostfile hostfile $n  mpi_fast_p --verbose --class huge

	echo "---"
done
