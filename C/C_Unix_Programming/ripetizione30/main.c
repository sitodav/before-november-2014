#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <time.h>



pthread_mutex_t mutex1=PTHREAD_MUTEX_INITIALIZER;
int max=0;


int n;
void *thread_start(void *arg);
int main(int argc,char *argv[]){

srand((unsigned int)time(NULL));
printf("\nInserisci n\n");
scanf("%d",&n);
while(getchar()!='\n');

int i,j,**matrix;

matrix=(int **)calloc(n,sizeof(int *));
for(i=0;i<n;i++){
	*(matrix+i)=(int *)calloc(n,sizeof(int));
	for(j=0;j<n;j++){
		*(*(matrix+i)+j)=1+rand()%10;
		printf(" %d",matrix[i][j]);
		
	}	
	printf("\n");
	fflush(stdout);
}

pthread_t *tid=(pthread_t *)calloc(n,sizeof(pthread_t));
int ret;
for(i=0;i<n;i++){
	if((ret=pthread_create(tid+i,NULL,thread_start,(void *)*(matrix+i)    ))!=0){
		printf("%s",strerror(ret));
		exit(1);	
	}

}

int **res=(int **)calloc(n,sizeof(int *));

for(i=0;i<n;i++){
	
	if((ret=pthread_join(tid[i],(void **)(res+i) ))!=0){
		printf("%s",strerror(ret));
		exit(1);	
	}
	printf("\nthread terminato con %d\n",**(res+i));
	fflush(stdout);
}


printf("\nIl massimo e' %d\n",max);
fflush(stdout);
return;

}




void *thread_start(void *arg){
printf("\nsono thread e parto!\n"); fflush(stdout);
int *vect=(int *)arg;

int i,prod=1;

for(i=0;i<n;i++)
	prod*=*(vect+i);

pthread_mutex_lock(&mutex1);
if(prod>max)
	max=prod;
pthread_mutex_unlock(&mutex1);


int *toRet=(int *)calloc(1,sizeof(int));
*toRet=prod;

pthread_exit( (void *)toRet );




}
