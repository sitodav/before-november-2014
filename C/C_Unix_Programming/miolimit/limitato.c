#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

void sigAlrmHandler(int sig);
int main(int argc,char *argv[]){
int timeToGo;
if(argc==1){
	printf("\nNon e' stato passato il parametro opzionale che indicasse per quanto tempo devo andare...\nVado indefinitamente fino a che mio \"padre\" non mi blocca\n");
	fflush(stdout);
}

else if(argc==2){
	printf("\nMi e' stato indicato di andare in esecuzione per %s secondi, dopo i quali mi blocchero' da solo \nse non mi avra' gia' bloccato mio padre\n"); fflush(stdout);
}

//per avere un timer che ci blocchi dopo un tot tempo senza pero' rimanere bloccati nel frattempo, usiamo la alarm()

if(argc==2){ 
	signal(SIGALRM,sigAlrmHandler);
	sscanf(argv[1],"%d",&timeToGo);
	alarm(timeToGo);
}
//altrimenti se non abbiamo passato il parametro di tempo, non settiamo nessun allarme

while(1)
	printf("CIAO!\n");


}

void sigAlrmHandler(int sig){
	printf("\nRicevuto allarme da me stesso che indica che devo autoterminarmi! MI UCCIDO\n");
	exit(123);



}
