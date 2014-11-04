#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>

int pidDest; //perche' dall'handler per SIGUSR dobbiamo sapere pid del destinatario del nuovo SIGUSR
	     //ma a noi la lista di pid dei vari called, ci viene passata come arg del main
 		//quindi dal main dobbiamo assegnarlo a questa variabile globale



//ogni called si occupa di inviare SIGUSR al solo called successivo, che e' quello
//che si trova in argv[1] mentre tutti gli altri pid non gli interessano
//per come sono creati i called dai figli forkati, accadra' che l'ultimo called della catena, quello che non conosce pid di altri figli (in realta' proprio il prim ofiglio creato in ordine cronologico) non avra' niente in argv[1] e quindi quello sara' il caso basee che mandera' SIGUSR
//direttamente al padre

void sigUsrHandler(int sig);

int main(int argc,char *argv[]){
int i;

//installo handler per sigusr
if(signal(SIGUSR1,sigUsrHandler)==SIG_ERR){
	perror("\n");
	exit(1);
}

printf("\nSono processo \"called\" e la lista dei miei parametri in input(nome escluso) e'\n");
fflush(stdout);
for(i=1;i<argc;i++)
	printf("----> %s\n",argv[i]);


if(argv[1]==NULL) 
	pidDest=getppid();//caso base, siamo il primo called spawnato, e dobbiamo mandare SIGUSR 	direttamente al padre, non ad un called successivo, quindi settiamo i lvalore 	di pidDest per il padre
else 	//altrimenti il nostro unico destinatario di interesse si trova in argv[1]
	sscanf(argv[1],"%d",pidDest);

while(1);




return 0;
}

void sigUsrHandler(int sig){ //nell'handler per SIGUSR manda SIGUSR o al padre (caso base) o al prossimo called
printf("\nSono figlio con pid %d, e mando SIGUSR a %d\n",getpid(),pidDest);
fflush(stdout);
if(kill(pidDest,SIGUSR1)<0){
	perror("\n");
	exit(1);
}
exit(1);
}
