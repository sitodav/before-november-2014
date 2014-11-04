#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <time.h>

int n;



int **matrix;

void * thread_start(void *arg);

int main(int argc,char *argv[]){

srand((unsigned int)time(NULL));

if(argc!=2){
	printf("\npassami n\n");
	exit(0);
}


sscanf(argv[1],"%d",&n);


int *ind=calloc(n,sizeof(int));

pthread_t *tid=calloc(n,sizeof(pthread_t));

matrix=calloc(n,sizeof(int *));
int i,j;
for(i=0;i<n;i++){
	*(matrix+i)=calloc(n,sizeof(int));
	for(j=0;j<n;j++){
		*(*(matrix+i)+j)=rand()%3;
		printf("%d ",matrix[i][j]);	
	}
	printf("\n");
	fflush(stdout);
}

int ret;
int max=0;

for(i=0;i<n;i++){
	ind[i]=i;
	if((ret=pthread_create(tid+i,NULL,thread_start, (void *)(ind+i)   ))!=0){
		printf("errore 55,%s\n",strerror(ret));
		exit(1);	
	}	

}

int *p;

for(i=0;i<n;i++){
	if( (ret=pthread_join(tid[i],(void **)&p))!=0 ){
		printf("%s\n",strerror(ret));		
		exit(1);	
	}
	printf("ritornato %d\n",*p);
	if(*p>max)
		max=*p;
	

}
printf("\nIl max e' %d\n",max);
fflush(stdout);


	



}


void *thread_start(void *arg){
int myInd=*((int *)arg);


int *mySum=calloc(1,sizeof(int));
*mySum=0;
int j;

for(j=0;j<n;j++){
	
	*mySum+=matrix[myInd][j];

}
printf("??%d",*mySum);
pthread_exit((void *)mySum);




}
