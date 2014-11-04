#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#define VERO 1
#define FALSO 0


int iB=0;
int temp;
int *mA,*mB;

pthread_mutex_t mutex1=PTHREAD_MUTEX_INITIALIZER;

int n;

void * threadA(void *arg);
void * threadB(void *arg);
int main(int argc,char *argv[]){
srand((unsigned int)time(NULL));


printf("\nInserisci n\n"); 
scanf("%d",&n);
while(getchar()!='\n');

int i,ret,j;
pthread_t *tid=(pthread_t *)calloc(2,sizeof(pthread_t));

printf("\n");
mA=calloc(n,sizeof(int));
mB=calloc(n,sizeof(int));


for(i=0;i<n;i++){
	mA[i]=rand()%10;
	mB[i]=rand()%10;
}

for(i=0;i<n;i++){
	printf("%d ",mA[i]);
}
printf("\n\n");
for(i=0;i<n;i++){
	printf("%d ",mB[i]);
}

fflush(stdout);

if((ret=pthread_create(tid,NULL,threadA,NULL))!=0){
	printf("%s",strerror(ret));
	exit(1);
	
}

if((ret=pthread_create(tid+1,NULL,threadB,NULL))!=0){
	printf("%s",strerror(ret));
	exit(1);
	
}


for(i=0;i<2;i++){
	if((ret=pthread_join(tid[i],NULL))!=0){
		printf("%s",strerror(ret));
		exit(1);	
	}

}

printf("\nArrays:\n");

for(i=0;i<n;i++){
	printf("%d ",mA[i]);
}
printf("\n\n");
for(i=0;i<n;i++){
	printf("%d ",mB[i]);
}

return;


}




void *threadA(void *arg){
int i;

for(i=0;i<n;i++){
	printf("\nA: lavoro con i=%d",i);
	fflush(stdout);
	pthread_mutex_lock(&mutex1);
	iB=i;
	temp=mA[i];
	mA[i]=-1;
	pthread_mutex_unlock(&mutex1);
	
	while(1){
		pthread_mutex_lock(&mutex1);
		if(mB[i]!=-1){
			pthread_mutex_unlock(&mutex1);
			continue;		
		}		
		else{
			mB[i]=temp;
			pthread_mutex_unlock(&mutex1);
			break;		
		}
		
	}

}

pthread_mutex_lock(&mutex1);
iB=n;
pthread_mutex_unlock(&mutex1);

pthread_exit(NULL);


}


void *threadB(void *arg){

while(1){
	
	pthread_mutex_lock(&mutex1);
	if(iB==n){
		pthread_mutex_unlock(&mutex1);
		break;	
	}
	if(mA[iB]!=-1){
		
		pthread_mutex_unlock(&mutex1);
		continue;	
	}
	else{
		
		mA[iB]=mB[iB];
		mB[iB]=-1;
		pthread_mutex_unlock(&mutex1);	
	}

}

}
