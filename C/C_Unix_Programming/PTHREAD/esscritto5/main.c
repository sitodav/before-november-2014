/*
Si realizzi un programma C e Posix in ambiente Linux che, impiegando la libreria Pthread, crei un vettore
di interi di dimensione n, dove n>=21 e multiplo di 3, è un argomento passato da riga di comando.
Calcolato il valore medio degli elementi contenuti nel vettore,
il programma deve determinare,
impiegando tre thread, il numero di elementi maggiori del valore medio, rispettivamente nei tre
sottovettori di dimensioni n/3.
*/


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <time.h>

//var glob in CS
int totElem=0;

pthread_mutex_t mutex1=PTHREAD_MUTEX_INITIALIZER;

//var glob non in CS
int *array;
int n,porzioni;
int valMedio=0;


void * thread_start(void * arg);
int main(int argc,char *argv[]){
if(argc!=2){
	printf("\nUSO: n >=21 e multiplo di 3\n");
	exit(0);
}

sscanf(argv[1],"%d",&n);
if(n<21 || n%3!=0){
	printf("\nUSO: n non e' multiplo di 3 o non e' >=21\n");
	exit(0);
}

srand((unsigned int)time(NULL));
array=(int *)calloc(n,sizeof(int));
int i;
for(i=0;i<n;i++){
	array[i]=1+rand()%25;
	valMedio+=array[i];
	printf("%d ",array[i]);
}
valMedio/=n;




pthread_t tid[3];
int indStart[3],ret;
porzioni=n/3;

for(i=0;i<3;i++){
	indStart[i]=i*porzioni;
	if((ret=pthread_create(tid+i,NULL,thread_start,(void *)(indStart+i)))!=0){
		printf("\n%s",strerror(ret));
		exit(0);	
	}
}

for(i=0;i<3;i++)
	if((ret=pthread_join(tid[i],NULL))!=0){
		printf("%s",strerror(ret));
		exit(0);	
	}
printf("\nIl numero di valori nell'array superiore alla media %d e' %d\n",valMedio,totElem);
fflush(stdout);

exit(0);
}


void * thread_start(void * arg){
int start,end;
start=*((int *)arg);
end=start+porzioni;
int i;
for(i=start;i<end;i++){
	if(array[i]>valMedio){
		pthread_mutex_lock(&mutex1);
		totElem++;
		pthread_mutex_unlock(&mutex1);	
	}
}

pthread_exit(NULL);
}
