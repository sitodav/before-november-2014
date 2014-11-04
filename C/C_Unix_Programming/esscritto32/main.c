#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <time.h>




pthread_mutex_t mutex1=PTHREAD_MUTEX_INITIALIZER;
int maxValue=0;

int n;
int **matrix;

void * thread_start(void *arg);
int main(int argc,char *argv[]){
if(argc!=2){
	printf("\nUso: passami n\n");
	exit(1);
}
sscanf(argv[1],"%d",&n);

srand((unsigned int)time(NULL));

int i,j,ret;

matrix=(int **)calloc(n,sizeof(int *));
for(i=0;i<n;i++){ //allochiamo, riempiamo e stampiamo le righe della matrice nxn
	matrix[i]=(int *)calloc(n,sizeof(int));
	for(j=0;j<n;j++){
		matrix[i][j]=rand()%10;
		printf("%d ",matrix[i][j]);	
	}
	printf("\n");
}

pthread_t *tid=(pthread_t *)calloc(n,sizeof(pthread_t));
int *workInd=(int *)calloc(n,sizeof(int));
for(i=0;i<n;i++)
	workInd[i]=i;

for(j=0;j<n;j++)
	if((ret=pthread_create(tid+j,NULL,thread_start,(void *)(workInd+j)))!=0){
		printf("%s",strerror(ret));
		exit(1);	
	}
for(j=0;j<n;j++)
	if((ret=pthread_join(tid[j],NULL))!=0){
		printf("%s",strerror(ret));
		exit(1);	
	}


}


void * thread_start(void *arg){
int myCol=*((int *)arg);
int prod=1,i;

for(i=0;i<n;i++)
	prod*=matrix[i][myCol];

pthread_mutex_lock(&mutex1);
if(prod>maxValue){
	printf("\nSono thread %d e aggiorno il max value con %d\n",myCol,prod);
	fflush(stdout);
	maxValue=prod;
}
else{
	printf("\nSono thread %d e NON aggiorno il max value perche' io ho %d",myCol,prod);
	fflush(stdout);
}
pthread_mutex_unlock(&mutex1);
pthread_exit(NULL);
}
