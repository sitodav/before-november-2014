#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>

void handler(int sign);
int main(int argc,char *argv[]){

int childpid[2],fdPipe[2];

srand((unsigned int)time(NULL));
if(pipe(fdPipe)){
	perror("\nerrore 16\n");
	exit(1);
}

int i,ret;
for(i=0;i<2;i++){
	childpid[i]=fork();
	if(childpid[i]==0)
		break;
}

if(signal(SIGUSR1,handler)==SIG_ERR){
	perror("\n");
	exit(1);
}

if(i==2){
	printf("\nSono padre!\n"); fflush(stdout);
	sleep(2);
	close(fdPipe[0]);
	int *vector=(int *)calloc(20,sizeof(int));
	int j;
	for(j=0;j<20;j++){
		*(vector+j)=50+rand()%51;
		printf("%d-",vector[j]);
		fflush(stdout);	
	}
	for(j=0;j<20;j++)
		if(write(fdPipe[1],vector+j,sizeof( *(vector+j) ))<0){
			perror("\nerrore 45\n");
			fflush(stdout);		
		}
	printf("\nTerminato di scrivere, li segnalo e li attendo!\n"); fflush(stdout);
	for(j=0;j<2;j++)
		if(kill(childpid[j],SIGUSR1)!=0){
			perror("\nerrore 51\n");
			exit(1);		
		}
	for(j=0;j<2;j++)
		if(wait(NULL)<0){
			perror("\nerrore 56\n");
			fflush(stdout);
			exit(1);		
		}
}

else if(i==0){
	printf("\nSono primo figlio!\n"); fflush(stdout);
	close(fdPipe[1]);
	int j;
	int *letti=calloc(10,sizeof(int));
	printf("\nAttendo start!\n");
	fflush(stdout);
	pause();
	for(j=0;j<10;j++){
		
		if(read(fdPipe[0],letti+j,sizeof(int))<0){
			perror("errore 55\n");
			exit(1);		
		}
		printf("\nSono primo figlio, letto %d\n",letti[j]);
		fflush(stdout);
		sleep(2);
	
	}
	printf("\nSono figlio 1, ho terminato...trascrivo su file ed esco!\n");
	for(j=0;j<10;j++)
		printf("%d-",letti[j]);
	fflush(stdout);
	int fdFileW;
	
	if((fdFileW=open("miofile.txt",O_WRONLY | O_CREAT,00777))<=0){
		perror("errore 116\n");
		exit(1);	
	}
	
	for(j=0;j<10;j++)
		if(write(fdFileW,letti+j,sizeof(letti[j]))<=0){
			perror("errore 122\n");
			exit(1);		
		}
	close(fdFileW);
	return 0;
	
}

else if(i==1){
	close(fdPipe[1]);
	printf("\nSono secondo figlio!\n"); fflush(stdout);
	int j;
	int *letti=calloc(10,sizeof(int));
	printf("\nAttendo start!\n");
	pause();
	for(j=0;j<10;j++){
			
			sleep(2);
			if(read(fdPipe[0],letti+j,sizeof(int))<0){
				perror("\nerrore 73\n");
				exit(1);			
			}
			printf("\nSono secondo figlio, letti %d\n",letti[j]);
		fflush(stdout);
	}
	printf("\nSono figlio 2 e ho terminato! Trascrivo sul file ed esco!\n");
	for(j=0;j<10;j++)
		printf("%d-",letti[j]);
	fflush(stdout);
	int fdFileW;
	
	if((fdFileW=open("miofile2.txt",O_WRONLY | O_CREAT,00777))<=0){
		perror("errore 116\n");
		exit(1);	
	}
	
	for(j=0;j<10;j++)
		if(write(fdFileW,letti+j,sizeof(letti[j]))<=0){
			perror("errore 122\n");
			exit(1);		
		}
	close(fdFileW);
	return 0;

}



}


void handler(int sign){

if(sign==SIGUSR1)
	printf("\nPARTO!\n");
return;

}
