/*
Si realizzi un programma C in ambiente Linux, che implementi la seguente interazione di processi:
- il sistema consiste di 4 processi: un processo padre (P1) che provvede alla creazione di 3 processi figli
(P11, P12 e P13 ) e di una pipe Pa;
- il padre P1 attiva ciascun figlio attraverso un segnale SIGUSR2;
- P1 attende il completamento dei figli;
- P11 provvede a generare 50 numeri casuali positivi e un messaggio di terminazione (-1), scrivendoli
sulla pipe;
- P12 e P13 prelevano in modo competitivo i numeri dalla pipe e li trascrivono ciascuno su un proprio file.
*/

//l'esercizio non specifica se ogni proc figlio deve leggere max 1 numero e poi far leggere il successivo all'altro proc figlio lettore. Quindi ci limitiamo a farli leggere in maniera competitiva. Se avessimo voluto rispettare un ordine, avremmo dovuto farli alternare con pause e signal ulteriori

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>



void SIGUSR2handler(int sign);
int main(int argc,char *argv[]){
//lettura competitiva, quindi una sola pipe condivisa tra tutti i figli
int fdPipe[2],childpid[3],i,j,fdFile;

if(pipe(fdPipe)<0){
	perror("\n");
	exit(1);
}

if(signal(SIGUSR2,SIGUSR2handler)==SIG_ERR){
	perror("\n");
	exit(1);
}

for(i=0;i<3;i++){
	childpid[i]=fork();
	if(childpid[i]==0)
		break;
}

if(i==0){
	pause();
	int nBuff;
	while(read(fdPipe[0],&nBuff,sizeof(nBuff))>0){
		printf("\nSono primo figlio e ho letto %d\n",nBuff);
		fflush(stdout);	
	}
	
	
}

else if(i==1){
	pause();
	int nBuff;
	while(read(fdPipe[0],&nBuff,sizeof(nBuff))>0){
		printf("\nSono secondo figlio e ho letto %d\n",nBuff);
		fflush(stdout);	
	}
	
}

else if(i==2){
	pause();
	for(j=0;j<50;j++)
		if(write(fdPipe[1],&j,sizeof(j))<0){
			perror("\n");
			exit(1);	
		}
	j=-1;
	if(write(fdPipe[1],&j,sizeof(j))<0){
			perror("\n");
			exit(1);	
		}
	close(fdPipe[1]); 
	printf("\nSono il terzo figlio, ho scritto tutti i 50+1 numeri, e termino\n"); fflush(stdout);
	

	
}

else if(i==3){
	printf("\nSono il padre,attendo 5 sec\n");fflush(stdout);
	sleep(5);
	for(j=0;j<3;j++)
		if(kill(childpid[j],SIGUSR2)<0){
			perror("\n");
			exit(1);		
		}
	printf("\nSono il padre, attivati i figli, attendo loro terminazione\n");
	for(j=0;j<3;j++)
		wait(NULL);
	printf("\nSono padre.Tutti i figli sono terminati\nTermino\n"); fflush(stdout);
	
	



}






}


void SIGUSR2handler(int sign){

printf("\nSono stato sbloccato\n");
fflush(stdout);

}
