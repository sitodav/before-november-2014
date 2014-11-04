/*Realizzare un programma in C e Posix sotto Linux che, mediante l'uso di pipe, sommi concorrentemente
le N righe di una matrice NxN di interi definita dall'utente.
*/



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/types.h>
#include <time.h>
#define fifo1 "fifo"



int main(int argc,char *argv[]){

srand((unsigned int)time(NULL));
int n;
printf("\nInserisci il numero n (matrice nXn):\n");
scanf("%d",&n); while(getchar()!='\n');
int i,j,**matrix;
matrix=(int **)calloc(n,sizeof(int *));
for(i=0;i<n;i++)
	matrix[i]=calloc(n,sizeof(int));

printf("\nLa matrice %dX%d generata randomicamente e' \n",n,n); fflush(stdout);

for(i=0;i<n;i++){
	for(j=0;j<n;j++)
		printf("%d ",(matrix[i][j]=rand()%101));
	printf("\n");
}

int *childpid;
childpid=calloc(n,sizeof(int));

for(i=0;i<n-1;i++){
	childpid[i]=fork();
	if(childpid[i]==0)
		break;
}

if(i==n-1){
//padre
	if(access(fifo1,F_OK)<0 && errno==ENOENT){
		printf("\nSono il padre: LA FIFO NON ESISTE E LA GENERO\n"); fflush(stdout);
		if(mkfifo(fifo1,00777)<0){
			perror("\n"); 
			exit(1);		
		}
		printf("\nFifo creata!\n"); fflush(stdout);

	}

	int fdFifoR;
	if((fdFifoR=open(fifo1,O_RDONLY))<0){
		perror("\n"); 
		exit(1);
	}
	
	int sumtot=0,buff;
	for(j=0;j<n;j++)
		sumtot+=matrix[i][j]; //la riga iesima sara' la n-1 (padre)
	printf("\nSono padre: la mia riga %d e' -> %d\n",i,sumtot);
	
	for(j=0;j<n-1;j++){
		if(read(fdFifoR,&buff,sizeof(int))<0){
			perror("\n");
			exit(1);		
		}
		printf("\nsono padre: ricevuto da figlio la somma %d\n",buff);
		sumtot+=buff;	
	}
	printf("\nSono padre: IL TOTALE DEGLI ELEMENTI DELLA MATRICE E' %d\n",sumtot);
	fflush(stdout),
	exit(0);


}

else{
//figlio iesimo
	int fdFifoW;	
	if((fdFifoW=open(fifo1,O_WRONLY))<0){
		perror("\n");
		exit(1);	
	}
	int sumRiga=0;
	for(j=0;j<n;j++)
		sumRiga+=matrix[i][j]; //la riga iesima sara' quella col valore col quale il figlio e' uscito dal for della fork()
	printf("\nSono figlio %d: la somma della riga %d e' %d\n",i,i,sumRiga);
	fflush(stdout);
	if(write(fdFifoW,&sumRiga,sizeof(sumRiga))<0){
		perror("\n");
		exit(1);	
	}
	close(fdFifoW);
	exit(0);
}


}
