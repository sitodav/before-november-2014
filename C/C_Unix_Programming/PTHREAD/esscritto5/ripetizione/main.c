#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <pthread.h>



int n;
int media;

int sharedCont=0;
pthread_mutex_t mutex1=PTHREAD_MUTEX_INITIALIZER;


void * thread_start(void * arg);
int main(int argc,char *argv[]){

srand((unsigned int)time(NULL));
if(argc!=2){
	printf("\nuso:size array multiplo di 3\n");
	exit(1);
}

sscanf(argv[1],"%d",&n);

int *array=(int *)calloc(n,sizeof(int));
int i,sumParz=0;
printf("\n");
for(i=0;i<n;i++){
	array[i]=rand()%10;
	printf("%d ",array[i]);
	sumParz+=array[i];
}



printf("\nLa media degli elementi dell'array e' %d\n",(media=sumParz/n));


int ret;
pthread_t tid[3];

for(i=0;i<3;i++)
	if((ret=pthread_create(tid+i,NULL,thread_start,  (void *)(array+(i*(n/3)) )   ))!=0){
		printf("\n41:%s\n",strerror(ret));
		exit(1);
	}

for(i=0;i<3;i++){
	if((ret=pthread_join(tid[i],NULL))!=0 ){
		printf("%s",strerror(ret));
			
	}
}

printf("\nSono main thread: i 3 thread hanno terminato ed il numero di elementi maggiori del valor medio (%d) e' %d\n",media,sharedCont);
fflush(stdout);
return 0;


}

void * thread_start(void * arg){

int *mySlice=((int *)(arg));
int myParz=0;
int i;
for(i=0;i<n/3;i++)
	if(mySlice[i]>media)
		myParz++;

pthread_mutex_lock(&mutex1);
sharedCont+=myParz;
pthread_mutex_unlock(&mutex1);

pthread_exit(NULL);
}

