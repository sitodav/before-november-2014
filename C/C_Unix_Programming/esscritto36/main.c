/*Scrivere un programma C che riceve 2 nom i di file a riga di comand o, e genera un processo figlio . Il
processo padre legge dal primo file, un carattere alla volta ogni 3 secondi e scrive quello che legge in una
pipe . Il processo figlio legge dalla pipe , un caratte re alla volta e scr ive quello che legge nel secondo file,
alla ricez ione de l segnale SIGUSRl. Il tutto termina quando il padre non ha altro da leggere,
comunicando a video l'imminente t erm inazione che avverrà non pri ma di aver t erm inato il processo
figlio.
*/



#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>


void SIGUSR1handler(int sign);
int main(int argc,char *argv[]){


if(argc!=3){
	printf("\nPassami 2 nomi file\n");
	exit(1);
}

if(signal(SIGUSR1,SIGUSR1handler)<0){
	perror("\n");
	exit(1);
}

int fdFileR,fdFileW,pipe1[2];
if((fdFileR=open(argv[1],O_RDONLY))<0){
	perror("\n");
	exit(1);
}

if(pipe(pipe1)<0){
	perror("\n");
	exit(1);
}

int childpid;
char c;

childpid=fork();
if(childpid==0){
	//figlio
	close(fdFileR);
	close(pipe1[1]);
	if((fdFileW=open(argv[2],O_WRONLY,00777))<0){
		perror("\n");
		exit(1);
	}
int ret,i=0;
while(1){
	
	while(sleep(5)==0); //finche' aspetta 2 secondi e ritorna 0, quindi ritorna dalla sleep xke' son opassati i 2 secondi, allora riaspetta. Quando verra' interrotta da un segnale ritornera' dalla sleep con >0 e quindi esce da busy wait	
	printf("\n--%d\n",++i);
	fflush(stdout);
	ret=read(pipe1[0],&c,sizeof(char));
	if(ret==0)
		break;
	printf("\nSono figlio, ho letto %c\n",c);
	fflush(stdout);
	
	if(write(fdFileW,&c,sizeof(char))<0){
		perror("\n");
		exit(1);
	}


}
	printf("\nSono figlio, il padre ha chiuso e quindi esco anche io!\n");
	fflush(stdout);

}

else{
	//padre
	close(pipe1[0]);
	
	while(read(fdFileR,&c,sizeof(char))>0){
		
		printf("\nSono padre: ho letto %c\n",c);
		fflush(stdout);
		if(write(pipe1[1],&c,sizeof(c))<0){
			perror("\n");
			exit(1);		
		}
		if(kill(childpid,SIGUSR1)<0){
			perror("\n");
			exit(1);		
		}		
		
		
		sleep(3);	
	}
	
	if(kill(childpid,SIGUSR1)<0){ 
		perror("\n");
		exit(1);	
	}	
	close(pipe1[1]); 
	
}



}



void SIGUSR1handler(int sign){
printf("\nSono figlio, sbloccato\n");
}
