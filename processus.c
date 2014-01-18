#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
int main ()
{
 int tube[4];
 char c;
 if(pipe(tube))
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
	 case  0:
		{
		close(tube[1]);
		while(read(tube[0],&c,1))
		write(1,&c,1);
		printf("\n");
		exit(0);
		}
	case  1:
		{
		close(tube[0]);
		close(tube[1]);
		while(read(tube[1],&c,1))
		write(1,&c,1);
		printf("\n");
		exit(1);
		}
	case  2:
		{
		close(tube[0]);
		close(tube[1]);
		close(tube[1]);
		while(read(tube[2],&c,1))
		write(1,&c,1);
		printf("\n");
		exit(2);
		}
	 default:
		{
                int cr;
		close(tube[0]);
		close(tube[1]);
		close(tube[2]);
		while((c=getchar())!=EOF)
		{
		   if((c<='z')&&(c>='a'))
		    {
		      write(tube[1],&c,1);
		    }
		}
		close(tube[1]);
		wait(&cr);
		}
	}
	exit(0);
}
 
