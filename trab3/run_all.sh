#!/bin/bash
count="1 2 3 4 5 6 7 8 9 10 11"
n_procs=('2' '3' '5' '9' '9')
n_threads=('2' '2' '2' '2' '4')
n_out=('2' '4' '8' '16' '32')
host_files=('hosts2' 'hotsts4' 'hosts8' 'hosts16' 'hosts32')
dirs_home="$PWD"

echo Começando a execução a partir de $dirs_home

echo -e "\tExecutando programas"
echo

cd code/FAST

for i in $count; do
	echo "-----Executando run $i : 1 -----"
	./fast --nthreads 1 --verbose --class xhuge >./../../saida/1/out_$i.txt
done
echo "---"

cd ../mpi_FAST_threads

for (( p=0; p<${#n_procs[@]}; p++ )); do
	hostfile=${host_files[${p}]}
	n=${n_procs[${p}]}
	n_t=${n_threads[${p}]}
	n_o=${n_out[${p}]}

	for i in $count; do
		echo "-----Executando run $i : $n processos, $n_t threads, com hostfile $hostfile -----"

		mpirun -np $n -hostfile hostfile mpi_fast --verbose --class xhuge --nthreads $n_t >./../../saida/$n_o/out_$i.txt
	done

	export MPIP="-f ./saida/$n_o"
	echo "-----Executando run com mpiP : $n processos, $n_t threads, com hostfile $hostfile -----"
	mpirun -np $n -hostfile hostfile mpi_fast_p --verbose --class xhuge --nthreads $n_t

	echo "---"
done
