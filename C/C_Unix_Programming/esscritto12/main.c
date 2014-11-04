/*
4. 9 punti (PER CFU 12 e 10)
Progettare un programma in C e Posix per realizzare un meccanismo di comunicazione bidirezionale,
tramite pipe, tra un processo padre ed un processo figlio, in modo che il processo padre legga un
messaggio da tastiera e lo comunichi al processo figlio mentre il processo figlio stampa il messaggio
ricevuto dal padre e gli comunichi lo stesso messaggio all'inverso.
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(int argc,char *argv[]){

int fdPipe1[2];
int fdPipe2[2];
char *toWrite,*toRead;
int childpid;

if(pipe(fdPipe1)<0 || pipe(fdPipe2)<0){
	perror("\n");
	exit(1);
}


childpid=fork();


if(childpid!=0){//padre
	close(fdPipe1[0]);
	close(fdPipe2[1]);

	while(1){
		printf("\nInserisci messaggio da tastiera\n"); fflush(stdout);
		toWrite=calloc(100,sizeof(char));
		fgets(toWrite,100,stdin);
		if(write(fdPipe1[1],toWrite,strlen(toWrite)+1)<0){
			perror("\n");
			exit(1);
		}
		toRead=calloc(100,sizeof(char));
		if(read(fdPipe2[0],toRead,strlen(toWrite)+1)<0){
			perror("\n");
			exit(1);
		}
		printf("\nRicevuto in echo:\n%s\n",toRead); fflush(stdout);
		free(toWrite); free(toRead);

	}
}

else{ //figlio
	close(fdPipe1[1]);
	close(fdPipe2[0]);
	toRead=calloc(100,sizeof(char));
	while(read(fdPipe1[0],toRead,100)>0){
		printf("\nSono figlio, ricevuto:\n%s\n",toRead); fflush(stdout);
		int j,k;
		toWrite=calloc(100,sizeof(char));
		for(j=strlen(toRead)-1,k=0;j>=0;j--,k++)
			toWrite[k]=toRead[j];
		printf("\nIl messaggio al contrario e':\n%s\n",toWrite);
		fflush(stdout);
		if(write(fdPipe2[1],toWrite,strlen(toWrite)+1)<0){
			perror("\n"); exit(1);
		}
		free(toRead); free(toWrite);
		toRead=calloc(100,sizeof(char));

	}




}










}

