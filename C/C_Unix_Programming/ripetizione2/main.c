#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>


int massimo;
pthread_mutex_t mutex1=PTHREAD_MUTEX_INITIALIZER;


void *thread_start(void *arg);
int main(int argc,char *argv[]){
srand((unsigned int)time(NULL));
if(argc!=2){
	printf("\nuso: numero di matrici 3x3\n"); 
	exit(0);
}

int n;
sscanf(argv[1],"%d",&n);
int ***arrayMatrici=(int ***)calloc(n,sizeof(int **));

int i=n,j,k;
while(i-->0){
	printf("\nMatrice %d-esima\n",i);
	fflush(stdout);
	arrayMatrici[i]=(int **)calloc(3,sizeof(int *));
	for(j=0;j<3;j++){
		*(arrayMatrici[i]+j)=(int *)calloc(3,sizeof(int));
		for(k=0;k<3;k++){
			*( *( *(arrayMatrici+i) +j) +k)=rand()%10;
			printf("%d ",*( *(*(arrayMatrici+i)+j) +k));
		}
		printf("\n");			
	}
}

pthread_t *tid=(pthread_t *)calloc(n,sizeof(pthread_t));

int ret;
for(i=0;i<n;i++)
	if( (ret=pthread_create(tid+i,NULL,thread_start,(void *)( *(arrayMatrici+i) ) ))!=0 ){
		printf("45,%s",strerror(ret));
		exit(1);	
	}


printf("\nThread creati, aspetto\n"); 
for(i=0;i<n;i++)
	if((ret=pthread_join(tid[i],NULL))!=0){
		printf("%s",strerror(ret));
		exit(1);	
	}

printf("\nIl massimo dei massimi delle diagonali e' %d\n",massimo);
fflush(stdout);
return 0;

}

void *thread_start(void *arg){
int **myMatrix=(int **)arg;
int myMax=0;

int i;
//printf("\nSono un thread figlio e la mia diagonale di lavoro e' :\n");
for(i=0;i<3;i++){
	//printf("%d ",myMatrix[i][i]);
	fflush(stdout);
	if( *(*(myMatrix+i)+i)>myMax  )
		myMax=myMatrix[i][i];
}
pthread_mutex_lock(&mutex1);
if(myMax>massimo)
	massimo=myMax;
pthread_mutex_unlock(&mutex1);

pthread_exit((void *)NULL);
   
}

