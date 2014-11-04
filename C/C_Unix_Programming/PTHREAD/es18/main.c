/*Realizzare un programma C e Posix sotto Linux che, mediante l'ausilio della libreria pthread, decomponga
la somma degli elementi di un vettore di interi di dimensione 1000 in quattro somme locali effettuate
concorrentemente da altrettanti thread. Si contempli l'uso di una variabile mutex per regolare l'accesso
alla variabile globale che conterrà la somma degli elementi del vettore.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#define SIZE 20

//var glob da sinc
int sumTot=0;

pthread_mutex_t mutex1;

//per esercitarmi l'array non lo faccio globale, ma ne passo il riferimento alle funzioni thread, assieme all'indice
//di spawn del thread affinche' sappia su quale porzione lavorare



	


void * thread_start(void *);
int main(int argc,char *argv[]){

int array[SIZE];
int i;
srand((unsigned int)time(NULL));
for(i=0;i<SIZE;i++)
	array[i]=rand()%10;

pthread_t tid[4];
int ret;
for(i=0;i<4;i++){
	if((ret=pthread_create(tid+i,NULL,thread_start,(void *)(array+(SIZE/4)*i)))!=0){
		printf("%s",strerror(ret));
		exit(1);	
	}
}

for(i=0;i<4;i++){
	if((ret=pthread_join(tid[i],NULL))!=0){
		printf("%s",strerror(ret));
		exit(1);
	}
}

printf("\nTutti i thread sono terminati, sono il main thread, e la somma tot e' %d\n",sumTot);
for(i=0;i<SIZE;i++){
	printf("\n%d",array[i]);
	fflush(stdout);
}
fflush(stdout);


}



void * thread_start(void *arg){
int *array;
array=(int *)arg;
int i;
for(i=0;i<SIZE/4;i++){
	pthread_mutex_lock(&mutex1);
	sumTot+=array[i];
	pthread_mutex_unlock(&mutex1);
}

printf("sono un thread. Ho terminato\n"); fflush(stdout);
pthread_exit((void *)NULL);




}
