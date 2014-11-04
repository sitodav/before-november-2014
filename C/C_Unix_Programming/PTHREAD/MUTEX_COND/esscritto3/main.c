/*Si realizzi un programma in C e Posix sotto Linux che, con l’ausilio della libreria Pthread, lancia n thread
per calcolare la somma dei reciproci degli elementi di ciascuna riga di una matrice nxn di interi generati
casualmente in un intervallo [0,255]. Il calcolo della somma dei reciproci degli elementi di ciascuna riga
deve essere effettuato concorrentemente su tutte le righe. Individuati i valori cercati, si provvederà a
ricercarne il minimo ed a stamparlo a video. La dimensione della matrice può essere fornita in input al
programma in fase di esecuzione o da riga di comando.
*/

//Si generano n thread, ogni thread stampa somma reciproci della sua riga assegnata, e si accede sotto mutua esclusione (per evitare problema di race condition) alla variabile che conterra' il minimo
//quando tutti i thread terminano, il main thread li aspetta e poi stampa il valore della variabile

#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

//glob da sincronizzare 
float min=51;
//glob da non sincronizzare
int **matrix;
int n;

pthread_mutex_t mutex1=PTHREAD_MUTEX_INITIALIZER;

void * thread_start(void * arg);

int main(int argc,char *argv[]){
srand((unsigned int)time(NULL));

if(argc!=2){
	printf("\nPassami n\n"); 
	exit(0);
}

int i,j;

sscanf(argv[1],"%d",&n);



matrix=(int **)calloc(n,sizeof(int *));
for(i=0;i<n;i++){
	matrix[i]=(int *)calloc(n,sizeof(int));
	for(j=0;j<n;j++){
		matrix[i][j]=1+rand()%50;
		printf("%d ",matrix[i][j]);	
	}
	printf("\n");	
}

pthread_t *tid=(pthread_t *)calloc(n,sizeof(pthread_t));
//e per gli indici riga di lavoro da passare ognuno come argomento ai thread
int *indici=(int *)calloc(n,sizeof(int));
int ret;

//creo thread
for(i=0;i<n;i++){
	indici[i]=i;
	if((ret=pthread_create(tid+i,NULL,thread_start,(void *)(indici+i)))!=0){
		printf("\n%s",strerror(ret));
		exit(1);	
	}
}

//e aspetto che terminino
for(i=0;i<n;i++){
	if((ret=pthread_join(tid[i],NULL))!=0){
		printf("\n%s",strerror(ret));
		exit(1);	
	}
}

printf("\nIL MINIMO E' %f\n",min);
exit(0);

}

void * thread_start(void * arg){
int miaRiga=*((int *)arg);
float sum=0;
int j;

for(j=0;j<n;j++){
	sum+=(1.0f/matrix[miaRiga][j]);
}

printf("\nSono thread per riga %d e la mia somma e' %f\n",miaRiga,sum);

pthread_mutex_lock(&mutex1);
if(sum<min)
	min=sum;
pthread_mutex_unlock(&mutex1);

pthread_exit(NULL);




}
