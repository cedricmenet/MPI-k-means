#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mysql.h>
#include <mpi.h>

#include "kmeans.h"

#define PARTICLE_NUMBER 50
#define PARTICLE_DIM 2

#define CLUSTER_OFFSET 10


int main(int argc, char** argv)
{
	double* particle;
	particle = (double*) malloc(sizeof(double)*PARTICLE_NUMBER*PARTICLE_DIM);
	
	
	
	int rank, size;
	MPI_Init (&argc, &argv);      					/* starts MPI */
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);        /* get current process id */
	MPI_Comm_size (MPI_COMM_WORLD, &size);        /* get number of processes */
	
	
	
	MYSQL mysql;
	mysql_init(&mysql);
	mysql_options(&mysql,MYSQL_READ_DEFAULT_GROUP,"option");
	
	unsigned int i = 0;
	unsigned int j = 0;

	if(mysql_real_connect(&mysql,"localhost","root","notsecure","MPI",0,NULL,0))
	{
		printf("Connection success...from process %d of %d\n", rank, size );
	}
	else
	{
		printf("Une erreur s'est produite lors de la connexion à la BDD from process %d of %d\n", rank, size );
		return -1;
	}
	
	while(1)
	{
	
		if(rank == 0)
		{
		
			//Connection success
			printf("Connection success... requesting\n");
			
			//vide la bdd de résultat
			if (mysql_query(&mysql, "DELETE FROM `MpiResult_tbl`") != 0) {
				printf(mysql_error(&mysql));
				return -1;
			}
			
			
			
			//Requête qui sélectionne tout dans ma table scores
			mysql_query(&mysql, "SELECT * FROM TestMpi_tbl ORDER BY `TestMpi_id` DESC LIMIT 50");

			//Déclaration des objets
			MYSQL_RES *result = NULL;
			MYSQL_ROW row;

			
			unsigned int num_champs = 0;
			unsigned int l = 0;

			//On met le jeu de résultat dans le pointeur result
			result = mysql_use_result(&mysql);

			//On récupère le nombre de champs
			num_champs = mysql_num_fields(result);

			
			l = 0;
			
			//Tant qu'il y a encore un résultat ...
			while ((row = mysql_fetch_row(result)))
			{
				//on récupère le nombre de lignes
				//printf("%d \n", mysql_num_rows(result) );
				
				//On déclare un pointeur long non signé pour y stocker la taille des valeurs
				unsigned long *lengths;

				//On stocke ces tailles dans le pointeur
				lengths = mysql_fetch_lengths(result);

				//On fait une boucle pour avoir la valeur de chaque champs
				for(i = 0; i < num_champs; i++)
				{
				//On ecrit toutes les valeurs
				//printf("[%.*s] ", (int) lengths[i], row[i] ? row[i] : "NULL");
				}
				//printf("\n");
				particle[l*2] = atof(row[1]);
				particle[l*2+1] = atof(row[2]);
				
				++l;
			}

			//Libération du jeu de résultat
			mysql_free_result(result);
			
			
					
		}
		//a partir d'ici on a les particules
		
		//Broadcast des particules
		MPI_Bcast (particle,PARTICLE_NUMBER*PARTICLE_DIM,MPI_DOUBLE,0,MPI_COMM_WORLD);
		
		//build datapoint
		
		double** data_points;
		data_points = malloc(sizeof(double*)*PARTICLE_NUMBER);
		
		
		for(i = 0 ; i < PARTICLE_NUMBER; i++ )
		{	
			data_points[i] = malloc(sizeof(double)*PARTICLE_DIM);
			for(j = 0 ; j < PARTICLE_DIM; j++ )
				data_points[i][j] = particle[i*PARTICLE_DIM+j];
		}
		//build centroids
		double** centroids = malloc(sizeof(double*)*(rank + CLUSTER_OFFSET));
		for(i = 0; i < rank+ CLUSTER_OFFSET;i++)
		{
			centroids[i] =  malloc(sizeof(double)*PARTICLE_DIM);
		}
		
		//application du K-means
		
		int *c = k_means(data_points, PARTICLE_NUMBER, PARTICLE_DIM, rank+CLUSTER_OFFSET, 1e-3, centroids);
		
		
		/*for (i = 0; i < PARTICLE_NUMBER; i++) {
			printf("data point %d is in cluster %d\n", i, c[i]);
		}*/ 
		
		
		for(i = 0; i < rank+CLUSTER_OFFSET;i++)
		{
			//add result in bdd
			char str[1024];
	// 		sprintf(buf, 1024, "INSERT INTO `MpiResult_tbl` (MpiResult_x,MpiResult_y,MpiResult_k)  VALUES('%f','%f','%d')",  rank+2);
			sprintf(str, "INSERT INTO `MpiResult_tbl` (MpiResult_x,MpiResult_y,MpiResult_k)  VALUES('%f','%f','%d')", centroids[i][0],centroids[i][1],rank+CLUSTER_OFFSET);
			
			
			if (mysql_query(&mysql, str) != 0) {
				printf(mysql_error(&mysql));
			return -1;
			}
			
			
			
			printf("centroids %d from process %d of %d\n", i, rank, size);
			for(j = 0; j < PARTICLE_DIM;j++)
			{
				printf("[%f]",centroids[i][j] );
			}
			printf("\n");
		}
		
		sleep(15);
	}
    //update BDD avec le résultat
    
    
    
    mysql_close(&mysql);
    MPI_Finalize();
	return 0;
}

