#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
	// initialise particle data
	int* particle;
	
	int* recvbuf;
	int* subParticle;
	
	
	
	particle = (int*) malloc(sizeof(int)*PARTICLE_NUMBER*PARTICLE_DIM);
	
	
		//init random
		srand((unsigned) time(&t));
		
		
		//build particle coordinate for random
		for(i = 0 ; i < PARTICLE_NUMBER; i++ ) 
			for(j = 0 ; j < PARTICLE_DIM; j++ )
			{
				int r = rand() % MAP_MAX_SIZE;
				particle[i*PARTICLE_DIM+j] = r;
				printf("%d\n",r);
			}
	
	//now broadcast
	
	
	//subParticle = malloc(sizeof(int) *(PARTICLE_DIM*PARTICLE_NUMBER/size));
	//MPI_Scatter(&particle,PARTICLE_NUMBER/size*PARTICLE_DIM,MPI_INT,subParticle,PARTICLE_NUMBER/size*PARTICLE_DIM,MPI_INT,0,MPI_COMM_WORLD);
	
	
	
	
	
	data_points = malloc(sizeof(double*)*PARTICLE_NUMBER);
	//build data_points
	for(i = 0 ; i < PARTICLE_NUMBER; i++ )
	{	
		data_points[i] = malloc(sizeof(double)*PARTICLE_DIM);
		for(j = 0 ; j < PARTICLE_DIM; j++ )
			data_points[i][j] = particle[i*PARTICLE_DIM+j];
	}
	
	double** centroids = malloc(sizeof(double*)*10);
	for(i = 0; i < 10;i++)
	{
		centroids[i] =  malloc(sizeof(double)*PARTICLE_DIM);
		
		
	}
	int *c = k_means(data_points, PARTICLE_NUMBER, PARTICLE_DIM, 10, 1e-3, centroids);
	for (i = 0; i < PARTICLE_NUMBER; i++) {
		printf("data point %d is in cluster %d\n", i, c[i]);
	}
	for(i = 0; i < 10;i++)
	{
		printf("centroids %d ",i);
		for(j = 0; j < PARTICLE_DIM;j++)
		{
			printf("[%f]",centroids[i][j] );
		}
		printf("\n");
	}
	
  return 0;
} 
