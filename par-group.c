#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "defineN.h"

double 	A[N*N],
		B[N*N],
		C[N*N];

int main( int argc, char* argv[] ){
	long i, j, k, count ;
	int rank, size;
	double t;
	
	MPI_Status status;
	
	MPI_Init( &argc, &argv );
	MPI_Comm_size( MPI_COMM_WORLD, &size );
	MPI_Comm_rank( MPI_COMM_WORLD, &rank );

	
	/********************* initiation *********************************/
	if (rank == 0){
		for (i=0; i< N; i++)
			for (j=0; j< N; j++){
				A[i*N + j] = 1;
				B[i*N + j] = 1;
				C[i*N + j] = 0;
			}
		count = N/size;
	}
	else {
		for (i=0; i<N; i++)
			for (j=0; j<N; j++){
				C[i*N + j] = 0;
			}
	}
	
	t = MPI_Wtime();
	
	/***************** communicate ************************************/
	MPI_Bcast(&count, 1, MPI_LONG, 0, MPI_COMM_WORLD);
	MPI_Bcast(&B[0], N*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	

	MPI_Scatter(&A[0], count*N, MPI_DOUBLE, &A[0], count*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	
	/******************** commpute ************************************/
	for (k=0; k<N; k++)
		for (i=0; i<count; i++)
			for (j=0; j<N; j++)
				C[i*N + k] += A[i*N + j] * B[j*N + k];

	
	MPI_Gather(&C[0], count*N, MPI_DOUBLE, &C[0], count*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	
	/*********************** compute the last *************************/
	if (rank == 0)
		for (k=0; k<N; k++)
			for (i = count*size ; i< N; i++)
				for (j=0; j<N; j++)
					C[i*N + k] += A[i*N + j] * B[j*N + k];
	
	
	/************************ print result ****************************/
	/*if (rank == 0){
		printf("Answer is:\n");
		for (i=0; i<N; i++){
			for (j=0; j<N; j++)
				printf("%f ",C[i*N + j]);
			printf("\n");
		}
	}*/

	if (rank==0) printf("%f\n",MPI_Wtime()-t);

	MPI_Finalize();
	return 0;
}
