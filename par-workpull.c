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
 *			WORKPULL
 * 
 ****************************************************************/

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int master( int );
int slave(void);

/*int minFunc(int a, int b){
	return (a<b)? a:b;
}*/

#define minFunc(a,b) (((a)<(b))?a:b)


#define N 10

int main(int argc, char ** argv){
    int rank,size;
    double time;
    
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    
    time = MPI_Wtime();
    
    if(rank == 0)
      master(size);
    else
      slave();
    
    time = MPI_Wtime()- time;
    
    if (rank == 0)
		//printf("\nTime run = %f\n",time);
		printf("%f\n", time);
    
    MPI_Finalize();
    return 0;
}

int master(int procs){
	long matrixA[N][N], matrixC[N][N];
	long i,j, sender, row, numsent=0;
	long dotp[N];
	MPI_Status status;

	/* Initialize data */
	for(i=0; i < N; i++)
		for(j=0; j < N; j++)
			matrixA[i][j] = 1;
	
	/* distribute data to slave */
	for(i=1; i < minFunc(procs, N); i++){
		MPI_Send(&matrixA[i-1][0], N, MPI_LONG, i, i, MPI_COMM_WORLD );
		numsent++;
	}
	
	/* receive result and distribute data */
	for(i=0; i < N; i++){
		MPI_Recv(dotp, N, MPI_LONG, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		/* SV xac dinh process gui ket qua ve va gui tiep du lieu cho no ??? */
		sender = status.MPI_SOURCE;
		row     = status.MPI_TAG - 1;
		for (j=0; j<N; j++)
			matrixC[row][j] = dotp[j];
		
		if(numsent < N) {
			MPI_Send(&matrixA[numsent][0], N, MPI_LONG, sender, numsent+1, MPI_COMM_WORLD);
			numsent++;
		}
		else {
			/* SV gui thong diep thong bao ket thuc cong viec */
			MPI_Send(MPI_BOTTOM, 0, MPI_LONG, sender, 0, MPI_COMM_WORLD);
		}
		
	}
	
	/* In ket qua de xac dinh tinh dung dan cua chuong trinh */
	for(i = 0; i < N; i++){
		for (j=0; j<N; j++){
			printf("%ld ",matrixC[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	
	return 0;
}

int slave(void){
	MPI_Status status;
	long rev[N];
	long matrixB[N][N];
	
	long ans[N];
	int i, j, row;
	
	for(i=0; i < N; i++){
		for(j=0; j < N; j++)
			matrixB[i][j] = 1;
		ans[i] = 0;
	}
	do {
		MPI_Recv(rev, N, MPI_LONG, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		if (status.MPI_TAG == 0)
			break;
		
		row = status.MPI_TAG - 1 ;
		
		for (i = 0; i<N; i++){
			ans[i] = 0;
			for (j=0; j<N; j++){
				ans[i] += rev[j]*matrixB[i][j];
			}
		}
		
		MPI_Send(ans, N, MPI_LONG, 0, status.MPI_TAG, MPI_COMM_WORLD);
	}
	while (1);
	
	return 0;
}

