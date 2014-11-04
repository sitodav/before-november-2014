#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <time.h>



pthread_mutex_t mutex1=PTHREAD_MUTEX_INITIALIZER;
int max=0;



int n;
void *thread_start(void *arg);
int main(int argc,char *argv[]){


srand((unsigned int)time(NULL));
printf("\npassami n dispari\n");

scanf("%d",&n);
while(getchar()!='\n');

int i,j;
int **array=(int **)calloc(n,sizeof(int *));

for(i=0;i<n;i++){
	*(array+i)=(int *)calloc(n,sizeof(int));
	for(j=0;j<n;j++){
		*(*(array+i)+j)=rand()%10;
		printf("%d ",array[i][j]);
			
	}
	printf("\n");

}


int **parts=(int **)calloc(4,sizeof(int *));
for(i=0;i<4;i++)
	*(parts+i)=(int *)calloc(n,sizeof(int));
	
for(j=0;j<n;j++)
	*(*(parts)+j)=*(*(array+(int)(n/2))+j);
for(j=0;j<n;j++)
	*(*(parts+1)+j)=*(*(array+j)+(int)(n/2));

for(j=0;j<n;j++)
	*(*(parts+2)+j)=*(*(array+j)+j);

for(j=0;j<n;j++)
	*(*(parts+3)+j)= *(*(array+j)+n-j-1);

printf("\n\n");
for(i=0;i<4;i++){
	for(j=0;j<n;j++)
		printf("%d ",parts[i][j]);
	printf("\n");
		
}

int ret;
pthread_t *tid=(pthread_t *)calloc(4,sizeof(pthread_t));

for(i=0;i<4;i++){
	if((ret=pthread_create(tid+i,NULL,thread_start, (void *)(*(parts+i)) ))!=0){
		printf("\n%s\n",strerror(ret));
		exit(1);	
	}

}


for(i=0;i<4;i++)
	if((ret=pthread_join(tid[i],NULL))!=0){
		printf("%s",strerror(ret));
		exit(1);	
	}

printf("\n-->%d",max);

}



void *thread_start(void *arg){

int *mine=(int *)(arg);
int i;
int mySum=0;
for(i=0;i<n;i++)
	mySum+=mine[i];

pthread_mutex_lock(&mutex1);
if(mySum>max)
	max=mySum;
pthread_mutex_unlock(&mutex1);

pthread_exit(NULL);




}
