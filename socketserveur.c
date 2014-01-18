#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h> /*close*/

#define SUCCESS 0
#define ERROR 1

#define END_LINE 0x0
#define SERVER_PORT 1500
#define MAX_MSG 100

/*fonction readline*/
int read_line();

int main(int argc, char *argv[])
{
   int sd,newsd,clilen;

   struct sockaddr_in cliAddr, servAddr;
   char line[MAX_MSG];

   /*create socket*/
   sd=socket(AF_INET, SOCK_STREAM, 0);
   if(sd<0)
   {
	perror("cannot open socket");
        return ERROR;
   }

   /*bind server port*/
   servAddr.sin_family=AF_INET;
   servAddr.sin_addr.s_addr=htonl(INADDR_ANY);
   servAddr.sin_port=htons(SERVER_PORT);

   if(bind(sd, (struct sockaddr *) &servAddr, sizeof(servAddr))<0)
   {
	perror("cannot accept connection");
	return ERROR;
   }

   listen(sd,5);

   while(1)
   {
	printf("%s:waiting for data on port TCP %u\n",argv[0],SERVER_PORT);

	clilen=sizeof(cliAddr);
	newsd=accept(sd, (struct sockaddr *) &cliAddr, &clilen);
 	if(newsd<0)
	{
	    perror("cannot accept connection");
	    return ERROR;
    }

	/*init line*/
   memset(line,0x0,MAX_MSG);
   /*recieve segments*/
   while(read_line(newsd,line)!=ERROR)
   {
       printf("%s:recieved from %s:TCP%d: %s\n",argv[0],inet_ntoa(cliAddr.sin_addr),ntohs(cliAddr.sin_port),line);
       /*init line*/
       memset(line,0x0,MAX_MSG);
   }/*while(readline)*/
   }/*while(1)*/
}

int read_line(int newsd,char *line_to_return)
{
    static int rcv_prt=0;
    static char rcv_msg[MAX_MSG];
    static int n;
    int offset;
    offset=0;
    while(1)
    {
        if(rcv_prt==0)
        {
            /*read data from socket*/
            memset(rcv_msg,0x0,MAX_MSG);/*init buffer*/
            n=recv(newsd,rcv_msg,MAX_MSG,0);/*wait data*/
            if(n<0)
            {
                perror("cannot recieve data");
                return ERROR;
            }
            else if(n==0)
            {
                printf("connection closed by client\n");
                close(newsd);
                return ERROR;
            }
        }
        /*if new data read on socket*/
        /*OR*/
        /*if another line is still in buffer*/

        /*copy line into 'line_to_return*/
        while(*(rcv_msg+rcv_prt)!=END_LINE && rcv_prt<n)
        {
            memcpy(line_to_return+offset,rcv_msg+rcv_prt,1);
            offset++;
            rcv_prt++;
        }
        /*end of line + end of buffer => return line*/
        if(rcv_prt==n-1)
        {
            /*set last byte to END_LINE*/
            *(line_to_return+offset)=END_LINE;
            rcv_prt=0;
            return ++offset;
        }

        /*end of line but still some data in buffer => return line */
        if(rcv_prt<n-1)
        {
            /*set last byte to END_LINE*/
            *(line_to_return+offset)=END_LINE;
            rcv_prt++;
            return ++offset;
        }

        /*end of buffer but line is not ended=>*/
        /*wait for more data to arrive on socket*/
        if(rcv_prt==n)
        {
            rcv_prt=0;
        }
    }/*while*/
}
