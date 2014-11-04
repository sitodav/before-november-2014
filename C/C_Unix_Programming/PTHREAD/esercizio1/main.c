/*scrivere un programma che accetta un intero n da riga di comando, crea n thread e poi aspetta la loro terminazione...ciascun thread aspetta u nnumero di secondi pari all'indice dell'ordine di generazione e poi incrementa una variabile globale intera e infine ne stampa il valore
*/

#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int glob=0;
void * start_thread(void * arg);
int main(int argc,char *argv[]){
if(argc!=2){
	printf("\nUso: n\n");
	exit(1);
}

int i,n,ret;
pthread_t *tid; //non sappiamo quanto dovra' essere grande l'array per contenere i TID, quindi usiamo alloc dinamica piu' avanti

sscanf(argv[1],"%d",&n);
tid=calloc(n,sizeof(pthread_t));

for(i=0;i<n;i++){
	ret=pthread_create(tid+i,NULL,start_thread,(void *)&i);
	if(ret){
		printf("%s",strerror(ret));
		exit(1);	
	}
	printf("\nSono padre: creato thread %d-esimo con TID:%u\n",i,(unsigned int)tid[i]);
	fflush(stdout);
}

for(i=0;i<n;i++)
	if((ret=pthread_join(tid[i],NULL))<0){
		printf("%s",strerror(ret));
		exit(1);	
	
	}
	else printf("\nTerminato thread %d-esimo con TID:%u\n",i,(unsigned int)tid[i]);






}

void * start_thread(void *arg){
int *p;
p=(int *)arg;
// *p e' il valore dell'indice di generazione

sleep(*p); //evita la race su glob
printf("\nSono thread %d-esimo: sveglio!\n",*p);
fflush(stdout);
glob++; //rischio race...
printf("\nglob e' :%d",glob);
fflush(stdout);


}
