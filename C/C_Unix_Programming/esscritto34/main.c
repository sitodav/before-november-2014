#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#define FIFO1 "fifo1"
#define FIFO2 "fifo2"



int main(int argc,char *argv[]){

int childpid[2],i;
int myFifoFd,myFifoFd2;
int pipe1[2],pipe2[2];

if(pipe(pipe1)<0 || pipe(pipe2)<0){
	perror("\n");
	exit(1);
}

if(access(FIFO1,F_OK)!=0){
		printf("\nFifo non esiste gia', la creo!\n");
		fflush(stdout);	
		if(mkfifo(FIFO1,00777)<0){
			perror("\n");
			exit(1);		
		}	
}

if(access(FIFO2,F_OK)!=0){
		printf("\nFifo non esiste gia', la creo!\n");
		fflush(stdout);	
		if(mkfifo(FIFO2,00777)<0){
			perror("\n");
			exit(1);		
		}	
}

for(i=0;i<2;i++){
	childpid[i]=fork();
	if(childpid[i]==0){
		printf("\nNasco (sono figlio %d con pid %d)",i,getpid());	
		fflush(stdout);	
		break;
	}
}

if(i!=2){
	//figli
	printf("\nSono figlio %d\n",i);
	fflush(stdout);
	if(i==0){
		close(pipe2[0]);
		close(pipe2[1]);
		close(pipe1[0]);
		
		if((myFifoFd=open(FIFO2,O_RDONLY))<0){
			perror("\n");
			exit(1);	
		}
	}
		
	
	else {
		close(pipe1[0]);
		close(pipe1[1]);
		close(pipe2[0]);
		
		if((myFifoFd=open(FIFO1,O_RDONLY))<0){
			perror("\n");
			exit(1);	
		}
	}
	
	printf("\nSono figlio%d , aperta fifo in lettura!\n",i);
	fflush(stdout);
	
	int t,parz=0;
	while(read(myFifoFd,&t,sizeof(int))>0){
		printf("Sono figlio %d e ho ricevuto %d\n",i,t);
		fflush(stdout);
		parz+=t;
	}
	printf("\nSono figlio %d : invio parziale..\n",i); fflush(stdout);
	
	int myPipe;
	if(i==0)
		myPipe=pipe1[1];
	else myPipe=pipe2[1];

	if(write(myPipe,&parz,sizeof(int))<0){
		perror("\n");
		exit(1);	
	}
}


else if(i==2){
	//padre		
	close(pipe1[1]);
	close(pipe2[1]);
	if((myFifoFd=open(FIFO1,O_WRONLY))<0){
		perror("\n");
		exit(1);	
	}
	printf("\nAperta prima fifo!\n");
	fflush(stdout);
	if((myFifoFd2=open(FIFO2,O_WRONLY))<0){
		perror("\n");
		exit(1);	
	}
	
	printf("\nSono padre, aperte fifo in scrittura!\n");
	fflush(stdout);
	
	printf("\nInserisci numero...\n");
	fflush(stdout);
	int n;
	while(scanf("%d",&n) && n>0){
		if(n%2==0){
			if(write(myFifoFd2,&n,sizeof(int))<0){
				perror("\n");
				exit(1);			
			}		
		}
		else{
			if(write(myFifoFd,&n,sizeof(int))<0){
				perror("\n");
				exit(1);			
			}
		}	
	}
	close(myFifoFd2);
	close(myFifoFd); //questo fara' ritornare anche i figli bloccati in lettura sulla fifo
	//quindi i figli sono ritornati, e allora aspetto che mi mandino i parziali pipe
	int tot=0,parz;	
	if(read(pipe1[0],&parz,sizeof(int))<0){
		perror("\n");
		exit(1);	
	}
	printf("\nIl totale dei pari e' : %d",parz);
	tot+=parz;
	

	if(read(pipe2[0],&parz,sizeof(int))<0){
		perror("\n");
		exit(1);	
	}
	printf("\nIl totale dei dispari e' : %d",parz);
	tot+=parz;

	printf("\nLa somma totale finale e' %d",tot);
	fflush(stdout);
}



}
