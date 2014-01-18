#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main ()
{
 FILE*f;
 char cmd_1[10],cmd_2[10],cmd_3[10];
 int pid,i,u;

 if((f=fopen("./commandes.txt","r"))==NULL)
    {
     printf("erreur d'ouverture fichier\n");
     exit(-1);
    }
else
{
	for(i=0;i<3;i++){
	pid=fork();
	switch(pid){
      		case -1:
	        	perror("erreur de processus");
			exit(2);
       		case 0 :
			printf("%d :je suis le fils %d de père %d\n",getpid(),i+1,getppid());
			while(fscanf(f,"%s %s %s",cmd_1,cmd_2,cmd_3)!=-1)
          		printf("cmd_1=%s,cmd_2=%s,cmd_3=%s\n",cmd_1,cmd_2,cmd_3);
			execl(cmd_1,cmd_2,cmd_3,NULL);
			exit(0);
       		default:
 			if(i==2){
			printf("%d: je suis le pere %d des 3 fils \n",getpid(),getpid());
			wait(&u);
			}
		   }
 	}
}
 fclose(f);
return(0);
}