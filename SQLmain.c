#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>



int main(void)
{
    MYSQL mysql;
    mysql_init(&mysql);
    mysql_options(&mysql,MYSQL_READ_DEFAULT_GROUP,"option");

    if(mysql_real_connect(&mysql,"localhost","root","notsecure","MPI",0,NULL,0))
    {
		//Connection success
	    printf("Connection success... requesting\n");
		
		 //Requête qui sélectionne tout dans ma table scores
        mysql_query(&mysql, "SELECT * FROM TestMpi_tbl");

        //Déclaration des objets
        MYSQL_RES *result = NULL;
        MYSQL_ROW row;

        unsigned int i = 0;
        unsigned int num_champs = 0;

        //On met le jeu de résultat dans le pointeur result
        result = mysql_use_result(&mysql);

        //On récupère le nombre de champs
        num_champs = mysql_num_fields(result);

        //Tant qu'il y a encore un résultat ...
        while ((row = mysql_fetch_row(result)))
        {
            //On déclare un pointeur long non signé pour y stocker la taille des valeurs
            unsigned long *lengths;

            //On stocke ces tailles dans le pointeur
            lengths = mysql_fetch_lengths(result);

            //On fait une boucle pour avoir la valeur de chaque champs
            for(i = 0; i < num_champs; i++)
            {
               //On ecrit toutes les valeurs
               printf("[%.*s] ", (int) lengths[i], row[i] ? row[i] : "NULL");
            }
            printf("\n");
        }

        //Libération du jeu de résultat
        mysql_free_result(result);
		
		
		
        mysql_close(&mysql);
    }
    else
    {
        printf("Une erreur s'est produite lors de la connexion à la BDD!");
    }

    return 0;
}

