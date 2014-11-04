#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <time.h>


struct sync{
	pthread_mutex_t mutex1;
	int *array;	
	int iMax;
	int Max;
	int sizePerThread;

} syncr;

void * thread_start(void *);

int main(int argc,char *argv[]){

srand((unsigned int)time(NULL));

if(argc!=2){
	printf("\nInserire size array!\n");
	exit(0);
}

int size;
sscanf(argv[1],"%d",&size);


syncr.Max=0;
syncr.sizePerThread=size/4;
int i,ret;

syncr.array=(int *)calloc(size,sizeof(int));
for(i=0;i<size;i++)
	syncr.array[i]=rand()%4;

pthread_t *tid=(pthread_t *)calloc(4,sizeof(pthread_t));


int *indici=(int *)calloc(4,sizeof(int));
for(i=0;i<4;i++){
	indici[i]=i;
	if( (ret=pthread_create(tid+i,NULL,thread_start,(void *)(indici+i)))!=0 ){
		printf("%s",strerror(ret));
		exit(1);	
	}
}
	
printf("\nThread creati...Li attendo\n");
for(i=0;i<4;i++){
	if( (ret=pthread_join(tid[i],NULL))!=0 )
		printf("%s",strerror(ret));
}
	
printf("\nArray\n");
for(i=0;i<size;i++)
	printf("%d ",syncr.array[i]);
	
printf("La somma parziale maggiore e' quella per thread con indice %d",syncr.iMax);
return 0;		


}



void * thread_start(void *arg){

int myInd=*((int *)(arg));
int i;
int myParz=0;

printf("\nSono thread indice :%d\n",myInd);
fflush(stdout);

for(i=0;i<syncr.sizePerThread;i++)
	myParz+=*(syncr.array+syncr.sizePerThread*myInd+i);	

pthread_mutex_lock(&(syncr.mutex1));
if(myParz>syncr.Max){
	syncr.Max=myParz;
	syncr.iMax=myInd;
}
pthread_mutex_unlock( &(syncr.mutex1)  );
pthread_exit((void *)NULL);

}
