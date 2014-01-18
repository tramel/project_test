//-----------------------------------------------------------------------------------------------------------------------------------------------------------

//Application client serveur permettant au serveur de transférer des données au client et de mesurer la durée de transmission

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SERVER_PORT 1500
#define MAX_MSG 100
char BUFFER[16];// espace necessaire pour stocker le message recu( les nombres aléatoires)
FILE *fd;
int p=0,i=0;
char buffer[30];
typedef struct valeurs = {int temperature, int pression};
int tube_1[2],tube_2[2];
//int c = 10;
char c;
char p; // lecture du tube

int main (int argc, char *argv[])
{
	int sd; // descripteurs de socket
	int rc,n;
	struct sockaddr_in localAddr, servAddr;
	struct hostent *h;

  // verification du nombre d'arguments sur la ligne de commande
	if (argc<3)
	{	
		printf("usage :%s, <server> <data1> <data2>...<dataN>\n", argv[0]);
		exit(1);
	}
	
	h=gethostbyname(argv[1]);
	
	if(h==NULL)
	{
		printf("%s: unknown host '%s'\n",argv[0],argv[1]);
		exit(1);
	}
// initialisation de la structure d'adresse du serveur
	servAddr.sin_family=h->h_addrtype;
	memcpy((char*) &servAddr.sin_addr.s_addr,h->h_addr_list[0],h->h_length);
	// recuperation du port du serveur
	servAddr.sin_port=htons(SERVER_PORT);

//---------------------------------------------------------------------------------
//				|CREATE SOCKET|
//				---------------
	
	sd=socket(AF_INET, SOCK_STREAM,0);
	
	if(sd<0)
	{
		perror("cannot open socket ");
		exit(1);
	}
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------	
//			       |BIND ANY PORT NUMBER|
//			       ----------------------
// initialisation de la structure d'adresse locale
	// famille d'adresse
	localAddr.sin_family= AF_INET;
	localAddr.sin_addr.s_addr= htonl(INADDR_ANY);
	localAddr.sin_port =htons(0);

	
	rc=bind (sd,(struct sockaddr *) &localAddr, sizeof(localAddr));

	if (rc<0)
	{
		printf("%s: cannot bind port TCP %u\n",argv[0],SERVER_PORT);
		perror("error");
		exit(1);
	}
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
//
//			       |CONNECT TO SERVEUR|
//			       --------------------

	rc=connect (sd ,(struct sockaddr *) &servAddr, sizeof (servAddr)); 

	if(rc<0)
	{
		perror("cannot connect");
		exit(1);
	}
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------

if(pipe(tube_1))
	{
	perror("Pb_sur-creation_du_tube");
	exit(-1);
	}
 switch(fork())
	{
	 case -1:
		{
		perror("pb_fork");
		exit(-2);
		}
	 case  0: // dans le premier fils
		{
		close(tube_1[0]);
		write(tube_1[1],&c,sizeof(int));
		exit(0);
		}
	case  1: // on est dans le pere
		{
		
		if(pipe(tube_2))
		{
		perror("Pb_sur-creation_du_tube");
		exit(-1);
		}
		switch(fork())
			{
			 case -1:
				{
				perror("pb_fork");
				exit(-2);
				}
			 case  0:// dans le deuxieme fils
				{
				close(tube_2[0]);
				write(tube_2[1],&c,sizeof(int));
				exit(0);
				}
			case  1: // on est dans le pere
				{
					close(tube_1[1]);
					close(tube_2[1]);
					while(read(tube_1[0],&p,sizeof(char))
					valeurs.temperature = atoi(p);
					while(read(tube_2[0],&p,sizeof(char))
					valeurs.pression = atoi(p);
					send(sd,valeurs,sizeof(valeurs)+1,0);
// initialisation mémoire du buffer de réception des données(entiers)
memset(BUFFER,0x0,1024);
// réception à partir du même numero de socket d'envoie des données provenant du serveur 
// dans le buffer et affichage des données reçues
recv(sd,BUFFER,1024,0);
printf("%s\n\n",BUFFER);
					
		
				}
	}


return(0);
}
	
		
































