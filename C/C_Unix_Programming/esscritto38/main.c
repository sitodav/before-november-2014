#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <time.h>

/*Si realizzi un programma in C e Posix sotto Linux che, con l’ausilio della libreria Pthread, lancia n thread
per calcolare il prodotto degli elementi di ciascuna riga di una matrice nxn di interi generati casualmente
in un intervallo [0,255]. Il calcolo del prodotto degli elementi di ciascuna riga deve essere effettuato
concorrentemente su tutte le righe. Individuati i valori cercati, si provvederà a ricercarne il massimo ed a
stamparlo a video. La dimensione della matrice può essere fornita in input al programma in fase di
esecuzione o da riga di comando.
*/

int n;

int max=0;
pthread_mutex_t mutex1=PTHREAD_MUTEX_INITIALIZER;

void * thread_start(void * arg);
int main(int argc,char *argv[]){
if(argc!=2){
	printf("\nPassami n\n");
	exit(1);
}

srand((unsigned int)time(NULL));


sscanf(argv[1],"%d",&n);

int **matrix,i,j,ret;
matrix=(int **)calloc(n,sizeof(n));
for(i=0;i<n;i++){
	matrix[i]=(int *)calloc(n,sizeof(n));
	for(j=0;j<n;j++){
		matrix[i][j]=0+rand()%256;
		printf("%d ",matrix[i][j]);
	}
	printf("\n");
}


pthread_t *tid;
tid=(pthread_t *)calloc(n,sizeof(pthread_t));

for(i=0;i<n;i++)
	if((ret=pthread_create(tid+i,NULL,thread_start,(void *)(matrix[i])))!=0){
		printf("%s",strerror(ret));
		exit(1);	
	}

for(i=0;i<n;i++)
	if((ret=pthread_join(tid[i],NULL))!=0){
		printf("%s",strerror(ret));
		exit(1);	
	}

printf("\nTutti i thread hanno calcolato ognuno il suo prodotto riga e confrontato in MUTUA ESCLUSIONE il massimo!.\nIl massimo e' %d\n",max);

exit(0);
	


}

void * thread_start(void *arg){

int *myVector=((int *)arg);
int i,prod=1;
for(i=0;i<n;i++)
	prod*=myVector[i];

pthread_mutex_lock(&mutex1);
printf("\nSono %u e il mio prod e' %d\n",(unsigned int)pthread_self(),prod);
if(prod>max)
	max=prod;
pthread_mutex_unlock(&mutex1);

pthread_exit((void *)NULL);



}
