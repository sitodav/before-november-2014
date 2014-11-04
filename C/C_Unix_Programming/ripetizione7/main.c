#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <time.h>


int fdPipe[2];

void signal_handler(int sign);
int main(int argc,char *argv[]){

if(signal(SIGUSR1,signal_handler)==SIG_ERR || signal(SIGUSR2,signal_handler)==SIG_ERR){
	perror("\nerrore 16\n");
	exit(1);
}


if(pipe(fdPipe)!=0){
	perror("\nerrore 16\n");
	exit(1);
}

int childpid,toWait,sign;
srand((unsigned int)time(NULL));

childpid=fork();
if(childpid!=0){
	close(fdPipe[0]);
	printf("\nSono padre!\n");
	toWait=rand()%11;
	sleep(toWait);
	sign=1+rand()%2;
	
	if(sign==1){
		printf("\nSono padre e invio SIGUSR1\n");
		fflush(stdout);
		if(kill(childpid,SIGUSR1)!=0){
			perror("\nerrore 36\n");
			exit(1);		
		}	
	}
	else{
		printf("\nSono padre e invio SIGUSR2\n");
		fflush(stdout);
		if(kill(childpid,SIGUSR2)!=0){
			perror("\nerrore 44\n");
			exit(1);		
		}		
	}
	if(write(fdPipe[1],&sign,sizeof(sign))<=0){
		perror("\nerrore 48\n");
		exit(1);
	}
	printf("\nSono padre: inviato, termino!\n");
	return 0;
	
	
}

else{
	close(fdPipe[1]);
	printf("\nSono figlio!Attendo segnale\n");
	pause();
	printf("\nSono figlio! Termino!\n");
	return;
	
}



}


void signal_handler(int sign){

printf("\nRicevuto segnale!\n");
int toRead;
if(read(fdPipe[0],&toRead,sizeof(toRead))<0){
	perror("\nerrore 75\n");
	fflush(stdout);
}

if(sign==SIGUSR1)
	printf("\nRicevuto SIGUSR1 dal padre!\n");
else if(sign==SIGUSR2)
	printf("\nRicevuto SIGUSR2 dal padre!\n");
fflush(stdout);

printf("\nIl padre mi ha scritto %d\n",toRead);


}
