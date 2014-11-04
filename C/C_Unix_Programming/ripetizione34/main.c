#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <time.h>


pthread_mutex_t mutex1=PTHREAD_MUTEX_INITIALIZER;
struct node {
	int dato;
	struct node *pnext;

} sentry={0,NULL};


int eff=0;
int nMax;
void *thread_start(void *arg);
int main(int argc,char *argv[]){

if(argc!=2){
	printf("\npassami NMax\n");
	exit(0);
}

sscanf(argv[1],"%d",&nMax);



pthread_t *tid=(pthread_t *)calloc(3,sizeof(pthread_t));
int i,ret,*ind=(int *)calloc(3,sizeof(int));

for(i=0;i<3;i++){
	ind[i]=i;
	if((ret=pthread_create(tid+i,NULL,thread_start,(void *)(ind+i)))!=0){
		printf("%s",strerror(ret));
		exit(1);	
	}
}


for(i=0;i<3;i++){
	if((ret=pthread_join(tid[i],NULL))!=0){
		printf("%s",strerror(ret));
		exit(1);	
	}
}


struct node *pwork=NULL;

pwork=&sentry;
while((pwork=pwork->pnext)!=NULL)
	printf("\n%d",pwork->dato);


}

void *thread_start(void *arg){

int myId=*((int *)(arg));

int tot=0;
struct node *pwork=NULL;

pwork=&sentry;

while(1){
	tot=0;
	pwork=&sentry;
	pthread_mutex_lock(&mutex1);
	while((pwork=pwork->pnext)!=NULL)
		tot++;
	
	fflush(stdout);
	
	if(tot<nMax){
		eff++;
		//printf("\nAGGIUNGO%d %d",myId,tot);
		printf("\n%d\n",eff);
		fflush(stdout);
		pwork=(struct node *)calloc(1,sizeof(struct node));
		pwork->dato=eff;
		pwork->pnext=sentry.pnext;
		sentry.pnext=pwork;
		pthread_mutex_unlock(&mutex1);	
	}	
	else{
		pthread_mutex_unlock(&mutex1);
		pthread_exit(NULL);	
	}
	
	

}


pthread_exit(NULL);

}
