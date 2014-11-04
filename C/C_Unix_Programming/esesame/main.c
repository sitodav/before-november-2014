/*Scrivere un programma in C e Posix sotto Linux che resta sempre in attesa di un segnale:
- se riceve SIGINT stampa a video il fattoriale di un numero, compreso tra 1 e 20, passato come
argomento da linea di commando;
- se riceve SIGUSR1 legge il contenuto del file prova.txt e lo conserva nell’heap (memoria allocate
dinamicamente);
- se riceve SIGUSR2 crea un file prova1.txt con diritti di lettura e scrittura per tutti tranne per gli “altri” e
vi scrive, a ritroso, il contenuto dell’heap.
*/


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

//var glob
int n;
char *buff;

void SIGINThandler(int sign);
void SIGUSR1handler(int sign);
void SIGUSR2handler(int sign);
int main(int argc,char *argv[]){

if(argc!=2){
	printf("\nUso: <this> <n>\n"); exit(0);
}


sscanf(argv[1],"%d",&n);

if(signal(SIGINT,SIGINThandler)==SIG_ERR || signal(SIGUSR1,SIGUSR1handler)==SIG_ERR || signal(SIGUSR2,SIGUSR2handler)==SIG_ERR){
	perror("\n");
	exit(1);
}

while(1){
	printf("\nSono in attesa di un segnale\n"); fflush(stdout);
	pause();
}

return 0;
}


//handlers
void SIGINThandler(int sign){
int i,fatt=1;

for(i=2;i<=n;i++)
	fatt*=i;

printf("\nIl fattoriale di %d e' %d\n",n,fatt);
fflush(stdout);
//return

}


void SIGUSR1handler(int sign){
int fdFile,ret;
buff=calloc(1000,sizeof(char));
if(buff==NULL){
	perror("\n");
	exit(0);
}

if((fdFile=open("prova.txt",O_RDONLY))<0){
	perror("\n");
	exit(1);
}

printf("\nFile aperto, leggo...\n"); fflush(stdout);

do{
	ret=read(fdFile,buff,100);
	if(ret<0 && errno==EINTR)
		continue;
	if(ret<0){
		perror("\n"); exit(1);	
	}
		
}
	while(ret>0);
printf("\nContenuto di prova.txt letto!\n"); fflush(stdout);
close(fdFile);
//anche ritornando al main dal signal handler, il contenuto allocato dinamicamente
//di buff dovrebbe mantenersi perche' appunto e' allocato dinamicamente 

//return

}

void SIGUSR2handler(int sign){
int fdFile,ret;
int i=0;

if( (fdFile=open("prova1.txt",O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP))<0 ){
	perror("\n");
	exit(1);

}

printf("\nprova1.txt aperto.Inizio trascrizione dal buffer\n");

while(i<strlen(buff)){
	ret=write(fdFile,&buff[i++],1);
	if(ret<0 && errno==EINTR)
		continue;
	if(ret<0){
		perror("\n"); exit(1);
	}
}

printf("\nFile trascritto\n");
close(fdFile);


}
