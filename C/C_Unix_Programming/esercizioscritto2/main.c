/*Scrivere un programma C e Posix in ambiente Linux che generi due processi figli. Il padre ed i due figli si
sincronizzano in modo da scrivere in maniera alternata, nella prima posizione di un file temporaneo
creato opportunamente, i caratteri che di volta in volta leggono da tre file (uno per ogni processo) forniti
da riga di comando. Utilizzare i segnali per la sincronizzazione.
*/



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#define FileW "FileW.txt"


void SIGUSR1handler(int sign);
int main(int argc,char *argv[]){

if(argc!=4){
	printf("\nuso : nome 3 file\n"); 
	exit(0);
}

int childpid[2],fdFileR,fdFileW,i;

if(signal(SIGUSR1,SIGUSR1handler)==SIG_ERR){
	perror("\n");
	exit(1);

}


if((fdFileW=open(FileW,O_CREAT | O_WRONLY,0777))<0){
	perror("\n");
	exit(1);
}

printf("\nFile su cui scrivere, aperto\n");
fflush(stdout);

for(i=0;i<2;i++){
	childpid[i]=fork();
	if(childpid[i]==0)
		break;
}

if(i==0){
	printf("\nSono il primo figlio\n");
	fflush(stdout);
	if((fdFileR=open(argv[1],O_RDONLY))<0){
		perror("\n"); exit(1);	
	}
	else printf("\nSono primo figlio, aperto il file lettura %s\n",argv[1]);
	int ret;
	char buff[100];
	while((ret=read(fdFileR,buff,100))>0){
		printf("\nSono primo figlio, leggo %s",buff);
		fflush(stdout);	
		//scrivo sul fileW, sveglio il padre, e vado in pausa
		printf("\nSono primo figlio, scrivo:\n"); fflush(stdout);
		if(write(fdFileW,buff,strlen(buff))<=0){
			perror("\n");
			exit(1);		
		}
		if(kill(getppid(),SIGUSR1)<0){
			perror("\n");
			exit(1);		
		}
		printf("\nSono primo figlio:vado in pausa\n"); fflush(stdout);
		pause();

	}
	

}

else if(i==1){
	int ret;
	char buff[100];
	printf("\nSono il secondo figlio\n");
	fflush(stdout);
	if((fdFileR=open(argv[2],O_RDONLY))<0){
		perror("\n"); exit(1);	
	}
	else printf("\nSono secondo figlio, aperto il file lettura %s\n",argv[2]);
	while((ret=read(fdFileR,buff,100))>0){
		printf("\nSono secondo figlio, leggo %s",buff);
		fflush(stdout);	
		//vado in pausa finche' non mi sveglia il padre, scrivo, segnalo primo figlio (posso) e vado in pausa
		pause();
		if(write(fdFileW,buff,strlen(buff))<=0){
			perror("\n");
			exit(1);		
		}
		if(kill(childpid[0],SIGUSR1)<0){
			perror("\n");
			exit(1);		
		}
		printf("\nSono secondo figlio:vado in pausa\n"); fflush(stdout);
		
	}
	

}

else{	
	int ret;
	char buff[100];
	printf("\nSono il padre\n");
	fflush(stdout);
	if((fdFileR=open(argv[3],O_RDONLY))<0){
		perror("\n"); exit(1);	
	}
	else printf("\nSono padre, aperto il file lettura%s\n",argv[3]);
	while((ret=read(fdFileR,buff,100))>0){
		printf("\nSono padre, leggo %s",buff);
		fflush(stdout);	
		//vado in pausa, finche' non mi sveglia primo figlio, scrivo, segnalo secondo figlio e vado in apusa
		pause();
		if(write(fdFileW,buff,strlen(buff))<=0){
			perror("\n");
			exit(1);		
		}
		if(kill(childpid[1],SIGUSR1)<0){
			perror("\n");
			exit(1);		
		}
		printf("\nSono padre:vado in pausa\n"); fflush(stdout);
				

	}

}





}

void SIGUSR1handler(int sign){
printf("\nSono stato sbloccato\n");
fflush(stdout);

}
