/*****************************************************************
 * File name	: seq.c
 * Project 		: PARALLEL
 * Version 		: 1.0
 * Creation Date: 23/03/2013
 * Tabsize		: 4
 * Author  		: Group PHP                  
 * Copyright	: (c) 2013
 * License		: Free.
 * Comments		: 
 *		MODEL:
 *			SEQUENTIAL CUMPUTING
 * 
 ****************************************************************/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "defineN.h"


double timestamp(){
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return ((double) (tv.tv_sec + tv.tv_usec / 1000000.0));
}

long 	A[N][N], 
		B[N][N],
		C[N][N];

int main(int argc, char **argv){  
	int i, j, k;
	double start, end;
	for (i = 0; i < N; i++)
		for(j = 0; j < N; j++){
			A[i][j] = 1;
			B[i][j] = 1;
			C[i][j] = 0;
		}

	start = timestamp();

	for(i = 0; i < N; i++)
		for(j = 0; j < N; j++)
			for(k = 0; k < N; k++)
				C[i][j] += A[i][k] * B[k][j];

	end = timestamp();

	printf("%f\n", end - start);

	return 0;
}
