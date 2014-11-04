#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#define M_SIZE 3


pthread_mutex_t mutex1=PTHREAD_MUTEX_INITIALIZER;
int max=0;


void *thread_start(void *arg);
int main(int argc,char *argv[]){
if(argc!=2){
	printf("\nPassami n\n"); 
	exit(0);
}

int n;
sscanf(argv[1],"%d",&n);
int ***matrix=(int ***)calloc(n,sizeof(int **));
int i,j,k;
for(k=0;k<n;k++){
	printf("\n\n");
	*(matrix+k)=(int **)calloc(M_SIZE,sizeof(int *));
	for(i=0;i<M_SIZE;i++){
		*(*(matrix+k)+i)=(int *)calloc(M_SIZE,sizeof(int));
		for(j=0;j<M_SIZE;j++){
			*(*(*(matrix+k)+i)+j)=i*j+j*k;
			printf("%d ",matrix[k][i][j]);
			fflush(stdout);
		}
		printf("\n");	
	}


}


pthread_t *tid=(pthread_t *)calloc(n,sizeof(pthread_t));
int ret;
for(i=0;i<n;i++){
	if((ret=pthread_create(tid+i,NULL,thread_start,(void *)(*(matrix+i))  ))!=0){
		printf("%s",strerror(ret));
		exit(1);	
	}


}

int *p;


for(i=0;i<n;i++){
	if((ret=pthread_join(tid[i],(void **)(&p)))!=0){
		printf("%s",strerror(ret));
		exit(1);	
	}
	printf("\nthread %u ritorna: %d\n",(unsigned int)(pthread_self()),*p);
	fflush(stdout);
}

printf("\n il max e' %d \n",max);
fflush(stdout);



}


void *thread_start(void *arg){
printf("\nCIAO\n"); fflush(stdout);


int **a=(int **)(arg);


int *myDiag=(int *)calloc(1,sizeof(int)),i,j;


for(i=0;i<M_SIZE;i++)
	*myDiag+= *(*(a+i)+i);





pthread_mutex_lock(&mutex1);
if(*myDiag>max)
	max=*myDiag;
pthread_mutex_unlock(&mutex1);	
pthread_exit((void *)myDiag);




}

