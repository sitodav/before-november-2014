#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <errno.h>

struct found{
	int i;
	int j;
}
	*trovati;


int **matrix;

int m,n,x;
int iLast=0;
pthread_mutex_t mutex1=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond1=PTHREAD_COND_INITIALIZER;


void *thread_start(void *arg);
void *thread_start2(void *arg);

int main(int argc,char *argv[]){
trovati=(struct found *)calloc(n*m,sizeof(struct found));
matrix=(int **)calloc(m,sizeof(int *));

if(argc!=3){
	printf("\nUso : passami m && n\n"); 
	exit(1);

}

sscanf(argv[1],"%d",&m);
sscanf(argv[2],"%d",&n);



srand((unsigned int)time(NULL));

int i,j;


for(i=0;i<m;i++){
	*(matrix+i)=(int *)calloc(n,sizeof(int));
	for(j=0;j<n;j++){
		*(*(matrix+i)+j)=rand()%6;	
		printf("%d ",matrix[i][j]);	
	}
	printf("\n");

}


printf("\nInserisci x\n");
scanf("%d",&x);
while(getchar()!='\n');


pthread_t *tid=(pthread_t *)calloc(m,sizeof(pthread_t));
pthread_t tid2	;
int ret,*indici=(int *)calloc(m,sizeof(int));

if((ret=pthread_create(&tid2,NULL,thread_start2,NULL))!=0){
	printf("%s",strerror(ret));
	exit(1);
	

}



for(i=0;i<m;i++){
	indici[i]=i;
	if((ret=pthread_create(tid+i,NULL,thread_start,(void *)(indici+i)  ))!=0){
		printf("%s",strerror(ret));
		exit(1);	
	}
}

for(i=0;i<m;i++)
	if((ret=pthread_join(tid[i],NULL))!=0){
		printf("%s",strerror(ret));
		exit(1);	
	}


sleep(5);





}


void *thread_start(void *arg){
int myLine=*((int *)(arg));
int j=0;
while(j<n){
	if( *(*(matrix+myLine)+j)==x){
		pthread_mutex_lock(&mutex1);
		trovati[iLast].i=myLine;
		trovati[iLast].j=j;
		iLast++;
		pthread_mutex_unlock(&mutex1);
		pthread_cond_signal(&cond1);
				
	}
	
	j++;
}


}




void *thread_start2(void *arg){


while(1){
	pthread_mutex_lock(&mutex1);
	while(iLast==0)
		pthread_cond_wait(&cond1,&mutex1);
	while(iLast>0){
		iLast--;
		printf("CONTROLLORE TROVA i:%d j:%d\n",trovati[iLast].i,trovati	[iLast].j);
		fflush(stdout);
			
	}
	pthread_mutex_unlock(&mutex1);

}


}
