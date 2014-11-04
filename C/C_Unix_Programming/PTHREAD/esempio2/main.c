#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

//programma dove il main thread crea tot thread e ad ognuno gli passa il valore dell'indice
//al quale e' stato generato. 

void * thread_start(void * arg);
int main(int argc,char *argv[]){
pthread_t tid[3];
int i,ret;
for(i=0;i<3;i++){
	ret=pthread_create(tid+i,NULL,thread_start,(void *)&i);
	if(ret)
		printf("\nerrore: %s\n",strerror(ret));
	sleep(2);
}


}


void * thread_start(void * arg){
int *myid;
myid=(int *)(arg);
printf("\nSono il thread che era stato generato in indice %d,TID %u\n",*myid,(unsigned int)pthread_self());
fflush(stdout);
pthread_exit((void *)1);


}
