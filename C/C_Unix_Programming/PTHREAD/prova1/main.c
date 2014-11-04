#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

int n=0;

void *thread_start(void *arg);
int main(int argc,char *argv[]){

pthread_t tid[2];
int i,ret;
int *retValue;

for(i=0;i<2;i++){
	printf("\nSono il thread principale: creo un thread\n");
	fflush(stdout);
	if((ret=pthread_create(tid+i,NULL,thread_start,NULL))<0){
		printf("\nerrore:%s\n",strerror(ret));	
		exit(1);
	}
}

for(i=0;i<2;i++){
	if((ret=pthread_join(tid[i],(void *)&retValue))<0){
		printf("\nerrore:%s\n",strerror(ret));
		exit(1);	
	}
	printf("\nIl thread n %d con TID %u e' terminato con valore %d\n",i,(unsigned int)tid[i],*retValue);
	fflush(stdout);
}


printf("\nSono thread principale nell'unico processo: Ho creato i 2 thread e ho terminato\n");
fflush(stdout);



}


void *thread_start(void *arg){

int *retValue=calloc(1,sizeof(int)); //lo mettiamo nell'heap per evitare problema etc...
pthread_t myTid=pthread_self();
printf("\nSONO THREAD n%d e ho tid=%u\n",n,(unsigned int)myTid);
*retValue=n;

return (void *)retValue; //ogni thread termina perche' la funzione a loro attribuita ritorna con la return ma potevamo fare anche pthread_exit((void *)retValue)
}
