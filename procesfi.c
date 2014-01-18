#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main (int argc, char**argv){
 	FILE*f;
	int tube[3];
        char cmd_1[10],cmd_2[10],cmd_3[10];
  if((f=fopen("./commandes.txt","r"))==NULL)
	{
	printf("erreur d'ouverture fichier\n");
	exit(-1);
	}
  else
	{
    	while(fscanf(f,"%s %s %s",cmd_1,cmd_2,cmd_3)!=-1)
          printf("cmd_1=%s,cmd_2=%s,cmd_3=%s\n",cmd_1,cmd_2,cmd_3);
	  execl(cmd_1,cmd_2,cmd_3, NULL);
	}
	
 	fclose(f);
 	return(0);
 }

//execl(const char *path, const char *arg,  ...);
//execl(cmd_1, NULL);
   
/*fprintf(f, "%s\n", "/bin ls -lisa");*/
