/*
Scrivere un programma C che riceve 2 nomi di file a riga di comando, e genera un processo figlio. Il
processo padre legge dal primo file e scrive quello che legge in una pipe. Il processo figlio legge dalla
pipe e scrive quello che legge nel secondo file. Il tutto termina quando il padre non ha altro da leggere. Il
secondo file deve essere eventualmente creato, se non esiste, con permessi di lettura e scrittura per il
proprietario e per il gruppo
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

int main(int argc,char *argv[]){
if(argc!=3){
	printf("\nUso: nomefile1 nomefile2\n");
	exit(0);
}

int fdFile,childpid,i,ret;
char *toRead,*toWrite;
int fdPipe[2];

if(pipe(fdPipe)<0){
	perror("\n"); exit(1);
}

childpid=fork();
if(childpid==0){
	close(fdPipe[1]);
	if((fdFile=open(argv[2],O_WRONLY | O_CREAT,00660))<0){
		perror("\n");
		exit(1);	
	}
	printf("\nSono il figlio, aperto in scrittura il file %s\n",argv[2]);
	toWrite=calloc(100,sizeof(char));
	while((ret=read(fdPipe[0],toWrite,100))>0){ //ritornera' 0 quando non ci sara' piu' niente sulla pipe e il padre avra' chiuso la sua estremita' in scrittura
		if(write(fdFile,toWrite,strlen(toWrite))<0){
			perror("\n");
			exit(1);		
		}
		free(toWrite);
		toWrite=calloc(100,sizeof(char));
	}
	close(fdPipe[0]);
	close(fdFile);
	
	

}

else if(childpid>0){
	close(fdPipe[0]);
	if((fdFile=open(argv[1],O_RDONLY))<0){
		perror("\n");
		exit(1);	
	}
	printf("\nSono il padre, aperto in lettura il file %s\n",argv[1]);
	toRead=calloc(100,sizeof(char));
	while((ret=read(fdFile,toRead,100))>0){
		if(write(fdPipe[1],toRead,strlen(toRead))<0){	
			perror("\n");
			exit(1);		
		}
		free(toRead);
		toRead=calloc(100,sizeof(char));	
	}
	//appena non c'e' piu' niente da leggere sul file, il padre chiude l'estremita' in scrittura dela pipe, cosi' il figlio non rimarra' bloccato in lettura sulla pipe
	close(fdPipe[1]);
	close(fdFile);

}

else{
	perror("\n");
	exit(1);
}

return 0;
}
