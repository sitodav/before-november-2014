/*Realizzare un programma in C e Posix che realizza il seguente comportamento: un processo padre apre
un file ricevuto come argomento da riga di comando e crea due figli P1 e P2. P1 stampa il numero delle
lettere 'a' nel testo mentre P2 stampa il numero delle lettere 'b' nel testo. Il padre alla ricezione del
segnale SIGINT stampa la somma delle lettere contate dai processi figli e termina.
*/

//ogni figlio apre separatamente dopo la fork il file, in modo tale che non ci sia competitivita' e race condition sulla
//lettura del file, visto che non e' richiesta da traccia


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>


int main(int argc,char *argv[]){
if(argc!=2){
	printf("\nuso: nomefile\n");
	exit(0);
}

int childpid[2],i;

for(i=0;i<2;i++){
	childpid[i]=fork();
	if(childpid[i]==0)
		break;	
}

if(i==2){
	//padre
	int atot,btot;
	waitpid(childpid[0],&atot,0);
	waitpid(childpid[1],&btot,0);
	
	printf("\nSono padre, entrambi i miei figli sono terminati, e la somma del numero di a e del numero di b e' %d",WEXITSTATUS(atot)+WEXITSTATUS(btot)); fflush(stdout);
	return 0;
}

else if(i==0){
	//primo figlio
	int fdFile,aCont=0;
	char buff;	
	if((fdFile=open(argv[1],O_RDONLY))<0){
		perror("\n"); exit(1);	
	}
	while(read(fdFile,&buff,1)>0)
		if(buff=='a')
			aCont++;
	printf("\nSono il primo figlio...ci sono %d a nel testo!\n",aCont);
	fflush(stdout);
	exit(aCont);

}

else if(i==1){
	//secondo figlio
	int fdFile,bCont=0;
	char buff;
	if((fdFile=open(argv[1],O_RDONLY))<0){
		perror("\n"); exit(1);	
	}
	while(read(fdFile,&buff,1)>0)
		if(buff=='b')
			bCont++;
	printf("\nSono il primo figlio...ci sono %d b nel testo!\n",bCont);
	fflush(stdout);
	exit(bCont);

}










}
