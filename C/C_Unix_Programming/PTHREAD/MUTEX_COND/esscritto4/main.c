/*Si realizzi un programma in C e Posix sotto Linux che, con l’ausilio della libreria Pthread, lancia n thread
per calcolare il prodotto degli elementi di ciascuna riga di una matrice nxn di interi generati casualmente
in un intervallo [0,255]. Il calcolo del prodotto degli elementi di ciascuna riga deve essere effettuato
concorrentemente su tutte le righe. Individuati i valori cercati, si provvederà a ricercarne il massimo ed a
stamparlo a video. La dimensione della matrice può essere fornita in input al programma in fase di
esecuzione o da riga di comando.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <time.h>

struct arg{
	int indice;
	int n;
};


//var glob da sinc in cs
int max=0;

pthread_mutex_t mutex1=PTHREAD_MUTEX_INITIALIZER;

//var glob non da sinc
int **matrix;

void * thread_start(void * arg);

int main(int argc,char *argv[]){
int n,i,j,ret;
srand((unsigned int)time(NULL));
printf("\nInserisci valore n\n"); 
fscanf(stdin,"%d",&n);
while(getchar()!='\n');

matrix=(int **)calloc(n,sizeof(int *));
for(i=0;i<n;i++){
	matrix[i]=(int *)calloc(n,sizeof(int));
	for(j=0;j<n;j++){
		matrix[i][j]=rand()%4;
		printf("%d ",matrix[i][j]);
	}
	printf("\n");
}

pthread_t *tid=calloc(n,sizeof(pthread_t));
struct arg *argomenti=calloc(n,sizeof(struct arg));


for(i=0;i<n;i++){
	argomenti[i].n=n;
	argomenti[i].indice=i;
	if((ret=pthread_create(tid+i,NULL,thread_start,(void *)(argomenti+i)))!=0){
		printf("err 48: %s\n",strerror(ret));
		exit(1);	
	}

}

for(i=0;i<n;i++){
	if((ret=pthread_join(tid[i],NULL))!=0){
		printf("%s\n",strerror(ret));
		exit(1);
	}

}

printf("\nIl valore massimo e' %d\n",max);
fflush(stdout);




exit(0);

}


void * thread_start(void * arg){
int n=((struct arg*)arg)->n;
int myLine=((struct arg*)arg)->indice;
int j,myTot=1;
for(j=0;j<n;j++)
	myTot*=matrix[myLine][j];

pthread_mutex_lock(&mutex1);
if(max<myTot)
	max=myTot;
pthread_mutex_unlock(&mutex1);
pthread_exit(NULL);


}



