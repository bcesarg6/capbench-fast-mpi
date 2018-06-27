all:
	mpicc main.c fast.c lib/src/timer.c lib/src/util.c -O3 -o mpi_fast -lm -lpthread

mpip:
	mpicc main.c fast.c lib/src/timer.c lib/src/util.c -g -O3 -o mpi_fast_p -L/home/cristofer/mpiP-3.4.1/lib -lmpiP -lm -lbfd -liberty -lunwind

clean:
	rm mpi_fast
	rm mpi_fast_p
