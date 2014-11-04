
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

/*Scrivere un programma in C e Posix sotto Linux che gestisca la seguente situazione:
Un processo P1 crea una pipe ed un figlio F1. Un secondo processo P2 comunicherà con P1 tramite un file
TEMP. P2 ogni secondo genera un numero casuale da 1 a 100 e lo scrive in TEMP seguito dal proprio pid.
P1 dopo 20 secondi dalla creazione del figlio scrive nella pipe il pid di P2, seguito dal numero -1, poi
stampa un messaggio sullo schermo e termina la sua esecuzione. Durante questi 20 secondi P1 leggerà i
numeri dal file TEMP e scriverà sulla pipe il suo pid con il numero che ha letto.
F1 leggerà dalla pipe i pid seguiti dal numero. Se il numero è -1 ucciderà P2 e poi terminerà; altrimenti
stamperà al terminale il proprio pid seguito dal numero che ha letto.
*/

int p2pid,fdPipe[2];

void SIGALRMhandler(int sign);
int main(int argc,char *argv[]){

int fdFile,childpid,ret,n;
char buff[20];
char buff2[20],buff3[20];

if(signal(SIGALRM,SIGALRMhandler)==SIG_ERR){
	perror("\n");
	exit(1);
}
pipe(fdPipe);

while(1) //sorta di polling, ovvero busy wait finche' p2 non gli ha creato il file temp che i 2 processi utilizzeranno come mezzo di comunicazione
	if( (fdFile=open("temp.txt",O_RDONLY))<0 && errno==ENOENT ) 
		continue;
	else break; //se lo apre o non ci riesce ma non perche' non esiste, esce

if(fdFile<0){
	perror("\n");
	exit(1);
}
	

childpid=fork();
if(childpid!=0){
	close(fdPipe[0]);
	alarm(20);
	while(1){
		do{
			ret=read(fdFile,buff,20);
			if(ret<0 && errno==EINTR)
				continue;
		}
		while(ret>0);
		if(ret<0){
			perror("\n");
			exit(1);	
		}
		sscanf(buff,"%d,%d\n",&p2pid,&n);
		printf("\nSono p1:Il processo p2(pid=%d) ha scritto sul file il numero %d\n",p2pid,n);
		fflush(stdout);
		printf("Lo mando a mio figlio f1 ma col mio pid\n");fflush(stdout);
		sprintf(buff3,"%d,%d\n",getpid(),n);
		ret=write(fdPipe[1],buff3,strlen(buff3)+1);
		if(ret<0){
			perror("\n");
			exit(1);	
		}
	}	
	
	

}

else if(childpid==0){
	int rcvpid,rcvnum=0;
	close(fdFile);
	close(fdPipe[1]);
	while(1){
		ret=read(fdPipe[0],buff,20);
		if(ret<0){
			perror("\n");
			exit(1);		
		}
		sscanf(buff,"%d,%d\n",&rcvpid,&rcvnum);
		if(rcvnum==-1) 
			break;
		//printf("\nSono figlio: ricevuto da mio padre sulla pipe: rcvpid:%d  rcvnum %d\n",rcvpid,rcvnum);

	}
	printf("\nSONO F1:Ricevuto rcvnum=%d, uccido p2 che ha pid=%d\n",rcvnum,rcvpid); fflush(stdout);
	if(kill(rcvpid,SIGKILL)<0){
		perror("\n"); exit(1);	
	}
	printf("\nSIG KILL INVIATA A P2, termino anche io figlio\n"); fflush(stdout);
	exit(0);
}

else{

 perror("\nFork non riuscita:"); fflush(stdout);
}





return 0;
}



void SIGALRMhandler(int sign){
int ret,n=-1;
char buff[20];

printf("\nSono padre:Passati i 20 secondi, mando -1 e p2pid a mio figlio\n"); fflush(stdout);
sprintf(buff,"%d,%d\n",p2pid,n);
ret=write(fdPipe[1],buff,strlen(buff)+1);
if(ret<0){
	perror("\n");
	exit(1);
}
printf("\nSono padre: termino\n");
exit(0);




}
