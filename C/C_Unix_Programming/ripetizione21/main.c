#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <time.h>

int n;
int max=0;
pthread_mutex_t mutex1;

void * thread_start(void *arg);
int main(int argc,char *argv[]){
srand((unsigned int)time(NULL));


printf("\nInserisci size matrice!\n");
scanf("%d",&n);

int **matrix=(int **)calloc(n,sizeof(int *));
int i,j;

for(i=0;i<n;i++){
	*(matrix+i)=(int *)calloc(n,sizeof(int));
	for(j=0;j<n;j++){
		*(*(matrix+i)+j)=rand()%256;	
		printf("%d ",matrix[i][j]);	
	}
	printf("\n");
}

pthread_t *tid=(pthread_t *)calloc(n,sizeof(pthread_t));
int ret;

for(i=0;i<n;i++){
	if((ret=pthread_create(tid+i,NULL,thread_start,(void *) (*(matrix+i)) ))!=0){
		printf("%s",strerror(ret));
		exit(1);
	}

}

for(i=0;i<n;i++){
	if((ret=pthread_join(*(tid+i),NULL))!=0){
		printf("%s",strerror(ret));
		exit(1);	
	}

}


printf("\nThread figli hannoetrminato, il massimo prodotto di riga e' %d\n",max);
fflush(stdout);


}	



void * thread_start(void *arg){

int *myVector=(int *)arg;

int myProd=1;
int i;
for(i=0;i<n;i++)
	myProd*=*(myVector+i);

printf("\nSono thread ed il mio prodotto e' %d\n",myProd);

pthread_mutex_lock(&mutex1);
if(myProd>max)
	max=myProd;
pthread_mutex_unlock(&mutex1);


pthread_exit(NULL);


}
