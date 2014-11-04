#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <time.h>



pthread_mutex_t mutex1;
int min=-1;
int n;



void * thread_start(void *);
int main(int argc,char *argv[]){
srand((unsigned int)time(NULL));

printf("\nInserisci n\n"); fflush(stdout);
scanf("%d",&n);

int **matrix;
int i,j;
matrix=(int **)calloc(n,sizeof(int *));
for(i=0;i<n;i++){
	matrix[i]=(int *)calloc(n,sizeof(int));
	for(j=0;j<n;j++){
		matrix[i][j]=rand()%2;
		printf("%d ",matrix[i][j]);
	}
	printf("\n");
}


pthread_t *tid=(pthread_t *)calloc(n,sizeof(pthread_t));
int ret;

for(i=0;i<n;i++){
	
	if( (ret=pthread_create(tid+i,NULL,thread_start,(void *)(*(matrix+i)) ))!=0){
		printf("%s",strerror(ret));
		exit(1);	
	}
}

printf("\nThread creati\n");

for(i=0;i<n;i++)
	if((ret=pthread_join(tid[i],NULL))!=0){
		printf("",strerror(ret));
		exit(1);
	}

printf("\nI thread hanno completato, il min e' %d\n",min);
fflush(stdout);
return 0;

}

void * thread_start(void *arg){
int *myMonoArray=(int *)arg;

int i,sumElementi=0;

for(i=0;i<n;i++){
	sumElementi+=*(myMonoArray+i);
}

pthread_mutex_lock(&mutex1);
if(min==-1 || sumElementi<min){
	printf("\nSono thread %u e la mia riga e' la minima con %d come somma elem\n",(unsigned int)pthread_self,sumElementi);
	fflush(stdout);
	min=sumElementi;
}
pthread_mutex_unlock(&mutex1);
	
pthread_exit(NULL);



}
