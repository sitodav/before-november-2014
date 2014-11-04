#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <string.h>




int n;


void *thread_start(void *arg);

int main(int argc,char *argv[]){

if(argc!=2){
	printf("\npassami n\n");
	exit(1);
}

sscanf(argv[1],"%d",&n);

int **matrix=(int **)calloc(n,sizeof(int *));
pthread_t *tid=(pthread_t *)calloc(n,sizeof(pthread_t));
int i,ret,j;

for(i=0;i<n;i++){
	if((ret=pthread_create(tid+i,NULL,thread_start,(void *)(matrix+i) ))!=0){
		printf("%s",strerror(ret));
		exit(1);	
	}
	
}


for(i=0;i<n;i++){
	if((ret=pthread_join(tid[i],NULL))!=0){
		printf("\n%s\n",strerror(ret));
		exit(1);	
	}

}


for(i=0;i<n;i++){
	for(j=0;j<n;j++)
		printf("%d ",matrix[i][j]);
	printf("\n");
}



return;


}


void *thread_start(void *arg){
printf("\nPARTO!\n");
fflush(stdout);

int **vector=(int **)(arg);
*vector=(int *)calloc(n,sizeof(int));
int i,j;
for(j=0;j<n;j++)
	*(*vector+j)=1+rand()%9;







}
