all:
	mpicc main.c fast.c lib/src/timer.c lib/src/util.c -g -O3 -o mpi_fast -L/home/react/mpiP-3.4.1/lib -lmpiP -lm -lbfd -liberty -lunwind

clean:
	rm -f mpi_fast
