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
 * Generates mask.
 */
static void generate_mask(int *mask){
	mask[0*p->maskcolumns + 0] = -1;
	mask[0*p->maskcolumns + 1] = -3;

	mask[1*p->maskcolumns + 0] = 0;
	mask[1*p->maskcolumns + 1] = -3;

	mask[2*p->maskcolumns + 0] = 1;
	mask[2*p->maskcolumns + 1] = -3;

	mask[3*p->maskcolumns + 0] = 2;
	mask[3*p->maskcolumns + 1] = -2;

	mask[4*p->maskcolumns + 0] = 3;
	mask[4*p->maskcolumns + 1] = -1;

	mask[5*p->maskcolumns + 0] = 3;
	mask[5*p->maskcolumns + 1] = 0;

	mask[6*p->maskcolumns + 0] = 3;
	mask[6*p->maskcolumns + 1] = 1;

	mask[7*p->maskcolumns + 0] = 2;
	mask[7*p->maskcolumns + 1] = 2;

	mask[8*p->maskcolumns + 0] = 1;
	mask[8*p->maskcolumns + 1] = 3;

	mask[9*p->maskcolumns + 0] = 0;
	mask[9*p->maskcolumns + 1] = 3;

	mask[10*p->maskcolumns + 0] = -1;
	mask[10*p->maskcolumns + 1] = 3;

	mask[11*p->maskcolumns + 0] = -2;
	mask[11*p->maskcolumns + 1] = 2;

	mask[12*p->maskcolumns + 0] = -3;
	mask[12*p->maskcolumns + 1] = 1;

	mask[13*p->maskcolumns + 0] = -3;
	mask[13*p->maskcolumns + 1] = 0;

	mask[14*p->maskcolumns + 0] = -3;
	mask[14*p->maskcolumns + 1] = -1;

	mask[15*p->maskcolumns + 0] = -2;
	mask[15*p->maskcolumns + 1] = -2;

	mask[16*p->maskcolumns + 0] = -1;
	mask[16*p->maskcolumns + 1] = -3;

	mask[17*p->maskcolumns + 0] = 0;
	mask[17*p->maskcolumns + 1] = -3;

	mask[18*p->maskcolumns + 0] = 1;
	mask[18*p->maskcolumns + 1] = -3;

	mask[19*p->maskcolumns + 0] = 2;
	mask[19*p->maskcolumns + 1] = -2;

	mask[20*p->maskcolumns + 0] = 3;
	mask[20*p->maskcolumns + 1] = -1;

	mask[21*p->maskcolumns + 0] = 3;
	mask[21*p->maskcolumns + 1] = 0;

	mask[22*p->maskcolumns + 0] = 3;
	mask[22*p->maskcolumns + 1] = 1;

	mask[23*p->maskcolumns + 0] = 2;
	mask[23*p->maskcolumns + 1] = 2;

	mask[24*p->maskcolumns + 0] = 1;
	mask[24*p->maskcolumns + 1] = 3;

	mask[25*p->maskcolumns + 0] = 0;
	mask[25*p->maskcolumns + 1] = 3;

	mask[26*p->maskcolumns + 0] = -1;
	mask[26*p->maskcolumns + 1] = 3;
}

/*
 * Runs benchmark.
 */
int main(int argc, char **argv){
	int i, bar = 0;              /* Loop index.            */
	int *mask;       	/* Mask.                  */
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

	if(tid == 0)
		timer_init();

	srandnum(seed);

	/* Benchmark initialization. */
	if (verbose && (tid == 0))
		printf("initializing...\n");

	MPI_Barrier(MPI_COMM_WORLD);

	if(tid == 0)
		start = timer_get();

	img = smalloc(p->imgsize*p->imgsize*sizeof(char));

	for (i = 0; i < p->imgsize*p->imgsize; i++){
		char val = randnum() & 0xff;
		img[i] = (val>0) ? val : val*(-1);
	}

	mask = smalloc(p->maskrows*p->maskcolumns*sizeof(int));
	generate_mask(mask);

	MPI_Barrier(MPI_COMM_WORLD);

	if(tid == 0)
		end = timer_get();

	if (verbose && (tid == 0))
		printf("  time spent: %f\n", timer_diff(start, end)*MICROSEC);

	/* Detect corners. */
	if (verbose && (tid == 0))
		printf("detecting corners...\n");


	MPI_Barrier(MPI_COMM_WORLD);

	if(tid == 0)
		start = timer_get();

	numcorners = fast(img, p->imgsize, mask);

	MPI_Reduce(&numcorners, &totalcorners, 1, MPI_INT, MPI_SUM, 0,
             MPI_COMM_WORLD);

	if(tid == 0){
		end = timer_get();
		printf("timing statistics:\n");
		printf("  total time:       %f\n", timer_diff(start, end)*MICROSEC);

		printf("  corners detected: %d\n", totalcorners);
	}

	/* House keeping. */
	free(mask);
	free(img);

	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
	return (0);
}
