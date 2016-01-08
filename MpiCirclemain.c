#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#include "kmeans.h"

#define PARTICLE_NUMBER 100
#define PARTICLE_DIM 2

#define MAP_MAX_SIZE 100


inline double hypot(double a, double b)
{
	return sqrt(a*a+b*b);
}
// is particle on circle (center MAP_MAX_SIZE/2)
inline int onCircle(int particle[PARTICLE_DIM],int circleSize)
{
	if(	hypot(	particle[0] - MAP_MAX_SIZE/2.,
				particle[1] - MAP_MAX_SIZE/2.)
		< circleSize)
		return 1;
	return 0;
}


int main (int argc, char* argv[])
{
	int rank, size;
	int i, j;
	time_t t;
	int nb_in;
	
	double** data_points;
	
	MPI_Datatype rtype;

	// initialise particle data
	int* particle;
	
	int* recvbuf;
	int* subParticle;
	
	
	MPI_Init (&argc, &argv);      /* starts MPI */
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);        /* get current process id */
	MPI_Comm_size (MPI_COMM_WORLD, &size);        /* get number of processes */
	printf( "Hello world from process %d of %d\n", rank, size );
	
	
	particle = (int*) malloc(sizeof(int)*PARTICLE_NUMBER*PARTICLE_DIM);
	
	if(rank == 0)
	{
		//init random
		srand((unsigned) time(&t));
		
		
		//build particle coordinate for random
		for(i = 0 ; i < PARTICLE_NUMBER; i++ )
			for(j = 0 ; j < PARTICLE_DIM; j++ )
				particle[i*PARTICLE_DIM+j] = rand() % MAP_MAX_SIZE;

	}
	//now broadcast
	MPI_Bcast (particle,PARTICLE_NUMBER*PARTICLE_DIM,MPI_INT,0,MPI_COMM_WORLD);
	
	
	//subParticle = malloc(sizeof(int) *(PARTICLE_DIM*PARTICLE_NUMBER/size));
	//MPI_Scatter(&particle,PARTICLE_NUMBER/size*PARTICLE_DIM,MPI_INT,subParticle,PARTICLE_NUMBER/size*PARTICLE_DIM,MPI_INT,0,MPI_COMM_WORLD);
	
	
	
	nb_in = 0;
	for( i = 0 ; i < PARTICLE_NUMBER ; i++ )
	{
		nb_in+=onCircle(particle+(i*PARTICLE_DIM),50);
	}
	printf( "from %d recu nb_in %d\n",rank, nb_in );
	
	
	data_points = malloc(sizeof(double*)*PARTICLE_NUMBER);
	//build data_points
	for(i = 0 ; i < PARTICLE_NUMBER; i++ )
	{	
		data_points[i] = malloc(sizeof(double)*PARTICLE_DIM);
		for(j = 0 ; j < PARTICLE_DIM; j++ )
			data_points[i][j] = particle[i*PARTICLE_DIM+j];
	}
	
	double** centroids = malloc(sizeof(double*)*(rank+2));
	for(i = 0; i < rank+2;i++)
	{
		centroids[i] =  malloc(sizeof(double)*PARTICLE_DIM);
		
		
	}
	int *c = k_means(data_points, PARTICLE_NUMBER, PARTICLE_DIM, rank+2, 1e-3, centroids);
	for (i = 0; i < PARTICLE_NUMBER; i++) {
		printf("data point %d is in cluster %d\n", i, c[i]);
	}
	for(i = 0; i < rank+2;i++)
	{
		printf("centroids %d ",i);
		for(j = 0; j < PARTICLE_DIM;j++)
		{
			printf("[%f]",centroids[i][j] );
		}
		printf("\n");
	}
	
	//gather pour recup
	
	if(rank == 0)
	{
		recvbuf = malloc(sizeof(int) *(size));
		recvbuf[0] = 1;
		printf("recvbuf %d\n",recvbuf[0]);
	}
	
	MPI_Gather (&nb_in,1,MPI_INT,recvbuf,1,MPI_INT,0,MPI_COMM_WORLD);
	
	if(rank == 0){
	printf("sizeof recvbuf %d\n", sizeof(recvbuf));	
	printf("sizeof int %d\n", sizeof(int));	
	printf("sizeof MPI_INT %d\n", sizeof(MPI_INT));
	
		for(i = 0 ; i < size;i++)
			printf("[%d] recvbuf %d\n",i,recvbuf[i]);
	}
  
  
  MPI_Finalize();
  return 0;
} 
