/*
 * Copyright(C) 2014 	Alyson D. Pereira <alyson.deies@outlook.com>,
 * 			Pedro H. Penna <pedrohenriquepenna@gmail.com>,
 *
 * FAST corner detection Benchmark Kernel.
 */

#include "lib/include/global.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include "lib/include/timer.h"
#include "lib/include/util.h"
#include "fast.h"

/*
 * Problem.
 */
struct problem
{
	int maskcolumns; /* Mask columns. */
	int maskrows;    /* Mask Rows.    */
	int masksize;	 /* Mask Size.    */
	int imgsize;  	 /* Image size.   */
};

/* Problem sizes. */
static struct problem tiny     = { 2,27,54,  2048};
static struct problem small    = { 2,27,54,  4096};
static struct problem standard = { 2,27,54,  8192};
static struct problem large    = { 2,27,54, 16384};
static struct problem huge     = { 2,27,54, 24576};

int nprocs;
int tid;

/* Be verbose? */
int verbose = 0;

/* Seed number. */
static int seed = 0;

/* Problem. */
static struct problem *p = &tiny;

int mask[] = {-1, -3, 0, -3, 1, -3, 2, -2, 3, -1, 3, 0, 3, 1, 2, 2, 1, 3, 0, 3, -1, 3, -2, 2, -3, 1,
	-3, 0, -3, -1, -2, -2, -1, -3, 0, -3, 1, -3, 2, -2, 3, -1, 3, 0, 3, 1, 2, 2, 1, 3, 0, 3, -1, 3};

/*
 * Prints program usage and exits.
 */
static void usage(void){
	printf("Usage: gf [options]\n");
	printf("Brief: FAST Corner Detection Kernel\n");
	printf("Options:\n");
	printf("  --help             Display this information and exit\n");
	printf("  --class <name>     Set problem class:\n");
	printf("                       - small\n");
	printf("                       - standard\n");
	printf("                       - large\n");
	printf("                       - huge\n");
	printf("  --verbose          Be verbose\n");
	exit(0);
}

/*
 * Reads command line arguments.
 */
static void readargs(int argc, char **argv){
	int i;     /* Loop index.       */
	char *arg; /* Working argument. */
	int state; /* Processing state. */

	/* State values. */
	#define READ_ARG     0 /* Read argument.         */
	#define SET_CLASS    2 /* Set problem class.     */

	state = READ_ARG;

	/* Read command line arguments. */
	for (i = 1; i < argc; i++){
		arg = argv[i];

		/* Set value. */
		if (state != READ_ARG){

			switch (state){
				/* Set problem class. */
				case SET_CLASS :
					if (!strcmp(argv[i], "tiny"))
						p = &tiny;
					else if (!strcmp(argv[i], "small"))
						p = &small;
					else if (!strcmp(argv[i], "standard"))
						p = &standard;
					else if (!strcmp(argv[i], "large"))
						p = &large;
					else if (!strcmp(argv[i], "huge"))
						p = &huge;
					else
						usage();
					state = READ_ARG;
					break;

				default:
					usage();
			}

			continue;
		}

		/* Parse argument. */
		if (!strcmp(arg, "--verbose"))
			verbose = 1;
		else if (!strcmp(arg, "--class"))
			state = SET_CLASS;
		else
			usage();
	}
}

/*
 * Runs benchmark.
 */
int main(int argc, char **argv){
	int i, countp, countm, offset = 0, additional,	elements_per_process, n_elements, outside, total;              /* Loop index.            */
	uint64_t end;       /* End time.              */
	uint64_t start;     /* Start time.            */
	char *img; 			/* Image.                 */
	int numcorners = 0, totalcorners = 0;	/* Total corners detected */

	/* initialize MPI */
	MPI_Init(&argc, &argv);

	// Get the number of cores in the MPI cluster
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

	// Get the tID number of this core in the MPI cluster
	MPI_Comm_rank(MPI_COMM_WORLD, &tid);
	/* end mpi */

	readargs(argc, argv);

	n_elements = p->imgsize*p->imgsize;
	if(tid == 0){
		timer_init();
		srandnum(seed);

		if (verbose)
			printf("initializing...\n");

		start = timer_get();

		img = smalloc(n_elements*sizeof(char));

		for (i = 0; i < n_elements; i++){
			char val = randnum() & 0xff;
			img[i] = (val>0) ? val : val*(-1);
		}

		additional = p->imgsize * 3; //3 linhas
		elements_per_process = n_elements/(nprocs-1);
		for(i = 1; i < nprocs; i++){
			offset = elements_per_process * (i-1);
			countp = additional*2;

			outside = additional - offset;
			if(outside > 0){
				countm = -offset;
				countp = countp - outside;
			}
			else{
				countm = -additional;
			}

			total = offset+countm+elements_per_process+countp;
			countp = total > n_elements ? countp - (total - n_elements)/p->imgsize : countp;

			total = elements_per_process+countp;
			MPI_Send(&total, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			MPI_Send(img+(offset+countm), elements_per_process+countp, MPI_BYTE, i, 0, MPI_COMM_WORLD);
		}
	}
	else{
		MPI_Recv(&total, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, 0);

		img = smalloc(total*sizeof(char));

		MPI_Recv(img, total, MPI_BYTE, 0, 0, MPI_COMM_WORLD, 0);
	}

	MPI_Barrier(MPI_COMM_WORLD);

	if(tid == 0){
		end = timer_get();

		if (verbose){
			printf("  time spent: %f\n", timer_diff(start, end)*MICROSEC);
			printf("detecting corners...\n");
		}
	}

	MPI_Barrier(MPI_COMM_WORLD);

	if(tid == 0)
		start = timer_get();
	else
		numcorners = fast(img, p->imgsize);

	MPI_Reduce(&numcorners, &totalcorners, 1, MPI_INT, MPI_SUM, 0,
             MPI_COMM_WORLD);

	if(tid == 0){
		end = timer_get();
		printf("timing statistics:\n");
		printf("  total time:       %f\n", timer_diff(start, end)*MICROSEC);

		printf("  corners detected: %d\n", totalcorners);
	}

	/* House keeping. */
	free(img);

	MPI_Finalize();
	return (0);
}
