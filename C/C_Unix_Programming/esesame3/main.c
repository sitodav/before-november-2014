/*Progettare un programma in C e Posix per realizzare un meccanismo di comunicazione bidirezionale,
tramite pipe, tra un processo padre ed un processo figlio, in modo che il processo padre legga un
messaggio da tastiera e lo comunichi al processo figlio mentre il processo figlio stampa il messaggio
ricevuto dal padre e gli comunichi lo stesso messaggio all'inverso.
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#define FIFO1 "fifo"
#define FIFO2 "fifo2"
#include <sys/select.h>
#include <time.h>
#include <string.h>


int main(int argc,char *argv[]){

int childpid,fdFifo1,fdFifo2,ret;
fd_set set1;
FD_ZERO(&set1);
struct timeval tempo1;
char *buff;
char *buff2;



if(mkfifo(FIFO1,00777)<0){
	perror("\n");
	exit(1);
}

if(mkfifo(FIFO2,00777)<0){
	perror("\n");
	exit(1);
}


childpid=fork();
if(childpid==0){
	
	if( (fdFifo1=open(FIFO1,O_RDONLY))<0 ){		
		perror("\n");
		exit(1);
	}
	if( (fdFifo2=open(FIFO2,O_WRONLY))<0 ){
		perror("\n");
		exit(1);
	}
	printf("\nSono figlio: aperta FIFO1 (R) su %d e FIFO2(W) su %d\n",fdFifo1,fdFifo2);
	fflush(stdout);
	while(1){
		buff=calloc(100,sizeof(char));
		ret=read(fdFifo1,buff,100);
		
		printf("\nSono figlio e ho ricevuto messaggio %s: Lo rimando indietro al contrario\n",buff);
		fflush(stdout);
		int i;
		buff2=calloc(100,sizeof(char));
		for(i=strlen(buff)-1;i>=0;i--)
			buff2[strlen(buff)-1-i]=buff[i];
		buff2[strlen(buff2)]='\0';
		printf("%s",buff2);
		fflush(stdout);
		write(fdFifo2,buff2,strlen(buff2)+1);
		free(buff2);
		free(buff);
		
	}


}


else if(childpid>0){

	if( (fdFifo1=open(FIFO1,O_WRONLY))<0 ){
		perror("\n");
		exit(1);
	}
	if( (fdFifo2=open(FIFO2,O_RDONLY))<0 ){
		perror("\n");
		exit(1);
	}
	printf("\nSono padre: aperta FIFO1 (W) su %d e FIFO2(R) su %d\n",fdFifo1,fdFifo2);
	fflush(stdout);
	
	printf("\nInserisci testo\n");
fflush(stdout);
while(1){
	FD_SET(fileno(stdin),&set1);
	tempo1.tv_sec=5;
	if(select(1,&set1,NULL,NULL,&tempo1)==0){
		printf("\nIn attesa dell'inserimento dell'input da tastiera\n");
		fflush(stdout);	
		continue;
	}
	
	else if(FD_ISSET(fileno(stdin),&set1)){
		buff=calloc(100,sizeof(char));
		fgets(buff,100,stdin);
		ret=write(fdFifo1,buff,strlen(buff)+1);
		if(ret<0){
			perror("\n");
			exit(1);
		}
		printf("\nInviato: %s\n",buff); fflush(stdout);
		buff2=calloc(100,sizeof(char));
		ret=read(fdFifo2,buff2,100);		
		if(ret<0){
			perror("\n");
			exit(1);		
		}
	
		printf("\nSono padre: ricevuto %d:\n%s",ret,buff2);
		fflush(stdout);	
		free(buff2);
		free(buff);
	}


}


}

else{
	perror("\nFork fallita\n");
	exit(1);
}




}
