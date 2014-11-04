/*Scrivere un programma in C e Posix sotto Linux che crea un processo figlio e dopo 5 secondi gli invia il segnale
SIGUSR2. Il figlio attende il segnale SIGUSR2 inserisce il contenuto del file password.txt, dell'esercizio precedente, in
un buffer e termina. Non appena il figlio termina, il padre manda a se stesso un segnale SIGUSR1 e quando intercetta il
segnale legge il contenuto del buffer e lo scrive sullo standard output, quindi attende per un numero di secondi pari alla
lunghezza della stringa contenuta nel buffer e termina.
*/


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#define file "password.txt"

//pipe globale per essere usata dall'handler
int pipe1[2];

void SIGUSR2handler(int sign);
void SIGUSR1handler(int sign);


int main(int argc,char *argv[]){

if(pipe(pipe1)<0){
	perror("\n");
	exit(1);
}

int childpid;

if((childpid=fork())<0){
	perror("\n");
	exit(1);
}

if(childpid!=0){
	if(signal(SIGUSR1,SIGUSR1handler)<0){
		perror("\n");
		exit(1);	
	}
	//padre
	close(pipe1[1]);
	if(signal(SIGUSR1,SIGUSR1handler)<0){
		perror("\n");
		exit(1);	
	}
	sleep(5);
	//mando segnale al figlio
	if(kill(childpid,SIGUSR2)<0){
		perror("\n");
		exit(1);	
	}
	//e aspetto che termini
	if(wait(NULL)<0){
		perror("\n");
		exit(1);	
	}
	//ora sulla pipe c'e' il contenuto del file password.txt
	//mi lancio da solo SIGUSR1handler
	if(kill(getpid(),SIGUSR1)<0){
		perror("\n");
		exit(1);	
	}
	


}

else{
	//figlio
	close(pipe1[0]);
	if(signal(SIGUSR2,SIGUSR2handler)<0){
		perror("\n");
		exit(1);	
	}
	sleep(10); //se non attendesse, terminerebbe prima di aver ricevuto il segnale dal padre
}

}



void SIGUSR2handler(int sign){
int fdFileR;

	printf("\nSono figlio, parto!: \n");
	fflush(stdout);

if((fdFileR=open(file,O_RDONLY))<0){
	perror("\n");
	exit(1);
}



char toRead[FILENAME_MAX];
while(read(fdFileR,toRead,FILENAME_MAX)>0){

	//mettilo nella pipe
	if(write(pipe1[1],toRead,strlen(toRead))<0){
		perror("\n"),
		exit(1);

	}
}


}


void SIGUSR1handler(int sign){
printf("\nSono padre e ho ricevuto SIGUSR1\n");
printf("\nSono padre, il figlio ha terminato e leggo quello che mi ha lasciato sulla pipe\n");
	fflush(stdout);
	char toRead[FILENAME_MAX];
	while(read(pipe1[0],toRead,FILENAME_MAX)>0){
		printf("%s",toRead);
		fflush(stdout);	
	}


}
