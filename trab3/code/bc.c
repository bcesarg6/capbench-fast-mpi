/*
 * Copyright(C) 2014 Alyson D. Pereira <alyson.deives@outlook.com>,
 *                   Pedro H. Penna <pedrohenriquepenna@gmail.com>
 *
 * fast.c - FAST corner detection kernel.
 */

#include "lib/include/global.h"
#include <math.h>
#include <omp.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "fast.h"

typedef struct{
  int tid;
  int n_lines;
  int init;
  int total;
  int imgsize;
  int res;
  char* img;
} pthread_arg;

/*
 * FAST corner detection.
 */

int mask[] = {-1, -3, 0, -3, 1, -3, 2, -2, 3, -1, 3, 0, 3, 1, 2, 2, 1, 3, 0, 3, -1, 3, -2, 2, -3, 1,
 	-3, 0, -3, -1, -2, -2, -1, -3, 0, -3, 1, -3, 2, -2, 3, -1, 3, 0, 3, 1, 2, 2, 1, 3, 0, 3, -1, 3};


void* fast_thread(void* data){
  pthread_arg* args = (pthread_arg*) data;
  int init, imgsize, total, n_lines, tid;
  int i,j,k,r,z,x,y;
	char accumBrighter, accumDarker;
	char imagePixel,centralPixel;
	int corners[1] = {0};
	int numcorners = 0;
  char* img;

  init = args->init;
  imgsize = args->imgsize;
  total = args->total;
  n_lines = args->n_lines;
  tid = args->tid;
  img = args->img;
  printf("init: %d, tid: %d, pid: %d, n_lines: %d, img_as: %c, img_s: %c, img_f: %c, total: %d\n",
          init, tid, pid, n_lines, img[init], img[init + (tid*n_lines)], img[init + ((tid+1)*n_lines)], total);
  {
		for (j = init + (tid*n_lines); j < init + ((tid+1)*n_lines); j++){
      if(j > 510)
        printf("j: %d, tid: %d\n",j, tid);
			for (i = 0; i < imgsize; i++){

				centralPixel = img[j*imgsize + i];
				z = 0;

				while(z < 16){
					accumBrighter = 0;
					accumDarker = 0;

					for(r = 0; r < 12; r++){
						x = i + mask[((r+z) * 2) + 0];
						y = j + mask[((r+z) * 2) + 1];

						if(x >=0 && y>=0 && ((y * imgsize + x) < total)){
							imagePixel = img[y * imgsize + x];

							if(imagePixel >= (centralPixel + THRESHOLD)){
								if(accumBrighter == 0){
									accumDarker++;
								}

								else{
									z += r - 1;
									goto not_a_corner;
								}
							}

							else if (imagePixel <= (centralPixel - THRESHOLD)){
								if(accumDarker == 0){
									accumBrighter++;
								}

								else{
									z += r - 1;
									goto not_a_corner;
								}
							}

							else{
								z += r;
								goto not_a_corner;
							}
						}
					}

					if(accumBrighter == 12 || accumDarker == 12){
						corners[0]++;
						z = 16;
					}
not_a_corner:			z++;
				}
			}
		}
	}

	numcorners += corners[0];
  args->res = numcorners;
}

int fast(char *img, int imgsize, int total, int init){
	int n_lines, i, numcorners = 0;
  pthread_t* threads;
  pthread_arg* args;

  threads = (pthread_t*)malloc((nthreads-1) * sizeof(pthread_t));
  args = (pthread_arg*)malloc(nthreads * sizeof(pthread_arg));

	n_lines = imgsize/(nprocs-1)/nthreads;
  printf("nlines: %d\n", n_lines);

  for(i = 0; i < nthreads; i++){
    args[i].tid = i;
    args[i].n_lines = n_lines;
    args[i].init = init;
    args[i].total = total;
    args[i].imgsize = imgsize;
    args[i].res = 0;
    args[i].img = img;
    if(i == (nthreads-1))
      break;
    pthread_create(threads+i, NULL, fast_thread, args+i);
  }

  fast_thread(args+i);

  for(i = 0; i < (nthreads-1); i++){
    pthread_join(threads[i], NULL);
  }

  for(i = 0; i < nthreads; i++){
    numcorners += args->res;
  }

  free(threads);
  free(args);

	return numcorners;
}
