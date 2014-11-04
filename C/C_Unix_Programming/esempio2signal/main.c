#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>


void handlerGenerico(int sigInput);

int main(int argc,char *argv[]){

//installiamo lo stesso handler per diversi segnali (2 in questo caso ma si puo' fare n volte)
if(signal(SIGINT,handlerGenerico)==SIG_ERR){
	perror("\n"); exit(1);
}

if(signal(SIGTSTP,handlerGenerico)==SIG_ERR){
	perror("\n"); exit(1);
}

//cicliamo all'infinito
while(1);



return 0;

}

void handlerGenerico(int sigInput){

if(sigInput==SIGTSTP)
	printf("\nIl segnale ricevuto e' di stop! SIGSTP!Non mi fermo!\n");
else if(sigInput==SIGINT)
	printf("\nRicevuto segnale di interruzione! SIGINT!\nNon mi voglio fermare!");


}
