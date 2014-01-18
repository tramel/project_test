#include <stdio.h>
#include <signal.h>
void hand(){
	printf("envoyer le signal SIGKILL pour me tuer\n");
	   }
main(){
	printf("installation du handler\n");
	signal(SIGINT,hand);
	signal(SIGQUIT,hand);
	for(;;);
	}
