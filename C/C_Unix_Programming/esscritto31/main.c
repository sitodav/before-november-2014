/*Realizzare un programma in C e Posix sotto Linux che, mediante l'uso di pipe, sommi concorrentemente
le N righe di una matrice NxN di interi definita dall'utente.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>


void SIGUSR1handler(int sign);

int main(int argc,char *argv[]){

int pipe1[2];
srand((unsigned int)time(NULL));

if(pipe(pipe1)<0){
	perror("\n");
	exit(1);
}

if(signal(SIGUSR1,SIGUSR1handler)<0){
	perror("\n");
	exit(1);
}

int n;

int sumTot=0;
printf("\nInserire n righe/processi\n");
scanf("%d",&n);
while(getchar()!='\n');

int **matrix,i,l;
matrix=(int **)calloc(n,sizeof(int *));
for(i=0;i<n;i++){
	matrix[i]=calloc(n,sizeof(int));
	for(l=0;l<n;l++)
		matrix[i][l]=rand()%10;
}

int *childpid;
childpid=(int *)calloc(n,sizeof(int));
for(i=0;i<n;i++){
	childpid[i]=fork();
	if(childpid[i]==0)
		break;

}

if(i==n){
	//padre
	close(pipe1[1]);
	int k,t;
	for(k=0;k<n;k++){
		if(kill(childpid[k],SIGUSR1)<0){
			perror("\n");
			exit(1);		
		}
		//sbloccato il figlio iesimo
		//e aspetto che termina
		if(waitpid(childpid[k],NULL,0)<0){
			perror("\n");
			exit(1);		
		}	
		if(read(pipe1[0],&t,sizeof(int))<0){
			perror("\n");
			exit(1);		
		}
		printf("\nIl figlio %d mi ha inviato %d\n",k,t);
		fflush(stdout);
		sumTot+=t;
	}
	printf("\nTutti i figli sono terminati\n");

	printf("\n\nL'array e'\n");
	fflush(stdout);
	int j;
	for(i=0;i<n;i++){
		for(j=0;j<n;j++)
			printf("%d ",matrix[i][j]);
		printf("\n");	
	}

	printf("\nLa somma risulta essere %d\n",sumTot);
}


else{	
	printf("\nSono figlio %d \n",i);
	fflush(stdout);
	//figli
	close(pipe1[0]);
	sleep(200);
	int j,sumParz=0;
	for(j=0;j<n;j++)
		sumParz+=matrix[i][j];
	if(write(pipe1[1],&sumParz,sizeof(int))<0){
		perror("\n");
		exit(1);	
	}
	//il figlio iesimo termina
}



}


void SIGUSR1handler(int sign){
printf("\nSbloccatomi\n");


}
