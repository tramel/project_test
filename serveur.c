//-----------------------------------------------------------------------------------------------------------------------------------------------------------

//Application client serveur permettant au serveur de transférer des données au client et de mesurer la durée de transmission

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h> /*close*/
#include <time.h>
#include <stdlib.h>
#define SUCCESS 0
#define ERROR 1

#define END_LINE 0x0
#define SERVER_PORT 1500
#define MAX_MSG 100

typedef struct valeurs = {int temperature, int pression};// espace necessaire pour stocker le message recu(requete client)
valeurs.temperature = malloc(sizeof(int));
valeurs.pression = malloc(sizeof(int));

char BUFFERS[512];// espace necessaire pour stocker le message à envoyer
int m,n,i=0,j=0;
FILE *f=NULL,*fd=NULL;// descripteurs de fichiers
char buffer[30];

int main(int argc, char *argv[])
{
	   int sd,newsd; // descripteurs de socket
	   int clilen;//taille de l'adresse du client
	   
	   struct sockaddr_in cliAddr; // structure d'adresse qui contiendra les param reseaux du client
	   struct sockaddr_in servAddr;// structure d'adresse qui contiendra les param reseaux du serveur
	   srand(time(NULL));
//---------------------------------------------------------------------------------
//				|CREATE SOCKET|
//				---------------
	
	sd=socket(AF_INET, SOCK_STREAM, 0);

	   if(sd<0)
	   {
		perror("cannot open socket");
		return ERROR;
	   }
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------	
//			       |BIND SERVER PORT |
//			       -------------------
 // initialisation de la structure d'adresse du serveur 

	// famille d'adresse
	   servAddr.sin_family=AF_INET;
	  // adresse IPv4 du serveur
	   servAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	   // recuperation du port du serveur
	   servAddr.sin_port=htons(SERVER_PORT);

 // association de la socket et des param reseaux du serveur
	   if(bind(sd, (struct sockaddr *) &servAddr, sizeof(servAddr))<0)
	   {
		perror("cannot bind port");
		return ERROR;
	   }
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//
//				|ENVOI DE 10 ENTIERS ALEATOIRES ET DE L'HEURE D'ENVOI|
//				------------------------------------------------------

   listen(sd,5);

	
 while(1)
	   {
		printf("%s: waiting for data on port TCP %u\n",argv[0],SERVER_PORT);
		clilen= sizeof(cliAddr);
		//connexion entre client et serveur
		newsd= accept(sd,(struct sockaddr *) &cliAddr,(socklen_t*) &clilen);
		if(newsd<0)
		{
			perror("cannot accept connection");
			return ERROR;
		}
		//initialisation mémoire du buffer de réception de la requête du client(juste un mot)
		//memset(nomfichier,0x0,100);
		
		// réception de la requete à partir du descripteur de socket newsd crée lors de 
		//l'établissement de la connexion entre client et serveur 
		recv(newsd,valeurs,100,0); 
		printf("%d\n",valeurs.temperature);
		printf("%d\n",valeurs.pression);
		
		//initialisation mémoire du buffer d'envoie des données par le serveur au client
		memset(BUFFERS,0x0,1024);

		//ouverture en ecriture d'un fichier quelcquonque dans lequel on va écrire les entiers
		//aléatoires et envoyer au client qui fera juste la lecture
		f=fopen(nomfichier,"w+");
	
		for(i=0;i<10;i++)
			{
				//stockage dans le tableau d'entiers aléatoires compris entre 0 et 49
				
				T[i]=(int)(rand()%50);

				//écriture des entiers générés dans un fichier local qui sera ensuite envoyé au client

				fprintf(f," %d ",T[i]);
				
			}
		//fermeture du fichier local	
		fclose(f);
		//ouverture du fichier contenant les entiers aléatoires en lecture
		f=fopen(nomfichier,"r");

		//lecture des données du fichier( les entiers) et envoi du buffer au client
		//et obtention de l'heure d'envoi au client
		if (f != NULL)
		{

			 fread(BUFFERS,1024,1,f);
			 printf("%s\n",BUFFERS);
			 send(newsd,BUFFERS,strlen(BUFFERS)+1,0);

			 //obtenir l'heure de l'envoi des données
			 gettimeofday(&tv, NULL); 
			 curtime=tv.tv_sec;
			 strftime(buffer,30,"%m-%d-%Y  %T.",localtime(&curtime));
			 
			 fclose(f);
		}
		//ecriture de l'heure d'envoi dans un fichier et envoi du fichier au client qui 
		//fera la lecture

		fd=fopen("temps.txt","w+");
		fprintf(fd," %ld ",tv.tv_usec);
		if (fd != NULL)
			{

			fread(buffer,1024,1,fd);
			printf("%s\n",buffer);
			send(newsd,buffer,strlen(buffer)+1,0);
	
			 printf("\n\nheure d envoi %s%ld\n",buffer,tv.tv_usec);
			fclose(fd);

			}


}

}

