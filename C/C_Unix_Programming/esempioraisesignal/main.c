#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void mioHandler(int signo);
int main(int argc,char *argv[]){

signal(SIGINT,mioHandler); //installiamo la disposizione per la gestione di SIGINT con handler mio
while(1)
	raise(SIGINT); //lanciamo a noi stesso il segnale che verra' gestito esplicitamente dal mioHandler





return 0;
}


void mioHandler(int signo){
if(signo==SIGINT)
	printf("\nricevuto segnale di chiusura! Non voglio chiudere!\n");


}
