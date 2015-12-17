#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>



int main(void)
{
    MYSQL mysql;
    mysql_init(&mysql);
    mysql_options(&mysql,MYSQL_READ_DEFAULT_GROUP,"option");

    if(mysql_real_connect(&mysql,"www.goldzoneweb.info","mon_pseudo","******","ma_base",0,NULL,0))
    {
        mysql_close(&mysql);
    }
    else
    {
        printf("Une erreur s'est produite lors de la connexion à la BDD!");
    }

    return 0;
}

