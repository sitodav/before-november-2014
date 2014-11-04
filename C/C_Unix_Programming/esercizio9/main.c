#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int a,b; //globali

void sigUsr1Handler(int sig);
void sigUsr2Handler(int sig);
void sigIntHandler(int sig); //in realta' l'esercizio chiede che il processo si interrompa quando riceve sigInt, quindi
//e' inutile implementare un sigHandler che stampa che abbiamo ricevuto siGint ed esce, ma bastava lasciare il default per il SIGINT...

int main(){


printf("\nInserisci primo numero\n");
scanf("%d",&a);
printf("\nInserisci secondo numero\n");
scanf("%d",&b);

//installo i 3 handler 
if(signal(SIGUSR1,sigUsr1Handler)==SIG_ERR)
	perror("\n");
if(signal(SIGUSR2,sigUsr2Handler)==SIG_ERR)
	perror("\n");
if(signal(SIGINT,sigIntHandler)==SIG_ERR)
	perror("\n");

printf("\nSono in attesa di ricevere segnale\n");

//non va bene un singolo pause, perche' altrimenti dopo il primo segnale terminerebbe
while(1);


return 0;
}




void sigUsr1Handler(int sig){


printf("\nla differenza tra i due numeri e': %d",a-b);
fflush(stdout);



}

void sigUsr2Handler(int sig){

printf("\nLa somma tra i due numeri e': %d",a+b);
fflush(stdout);
}


void sigIntHandler(int sig){ 
	printf("\nRicevuto SIGINT, TERMINO NORMALMENTE\n");
	fflush(stdout);
	exit(1);
}
