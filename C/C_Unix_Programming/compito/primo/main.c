#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <time.h>


struct buffer{
	int indJ;
	int indI;
	int value;

} buf={0,0,-1};

pthread_cond_t cond1=PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex1=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond2=PTHREAD_COND_INITIALIZER;

int **matrix1,**matrix2,**matrixRis;
int n,m,p;


void * thread_produttore(void * arg);
void * thread_consumatore(void * arg);

int main(int argc,char *argv[]){
if(argc!=4){
	printf("\nUso: m n p\n");
	exit(1);
}
srand((unsigned int)time(NULL));

sscanf(argv[1],"%d",&m);
sscanf(argv[2],"%d",&n);
sscanf(argv[3],"%d",&p);

int i,j;

matrix1=(int **)calloc(m,sizeof(int *));
matrix2=(int **)calloc(n,sizeof(int *));
matrixRis=(int **)calloc(m,sizeof(int *));

for(i=0;i<m;i++){
	matrix1[i]=(int *)calloc(n,sizeof(int));
	for(j=0;j<n;j++){
		matrix1[i][j]=1+rand()%3;	
		printf("%d ",matrix1[i][j]);	
	}	
	printf("\n");

}

printf("\n\n");

for(i=0;i<n;i++){
	matrix2[i]=(int *)calloc(p,sizeof(int));
	for(j=0;j<p;j++){
		matrix2[i][j]=1+rand()%3;	
		printf("%d ",matrix2[i][j]);	
	}	
	printf("\n");

}

for(i=0;i<m;i++)
	matrixRis[i]=(int *)calloc(p,sizeof(int));


pthread_t *produttori=(pthread_t *)calloc(m,sizeof(pthread_t));
int *indici=calloc(m,sizeof(int)),ret;

for(i=0;i<m;i++){
	indici[i]=i;
	if((ret=pthread_create(produttori+i,NULL,thread_produttore,(void *)(indici+i)))!=0){
		printf("%s",strerror(ret));
		exit(1);	
	}

}
pthread_t consumatore;
if((ret=pthread_create(&consumatore,NULL,thread_consumatore,NULL))!=0){
	printf("%s",strerror(ret));
	exit(1);
}

if((ret=pthread_join(consumatore,NULL))!=0){
	printf("%s",strerror(ret));
	exit(1);

}

printf("\nRISULTATO:\n\n");
for(i=0;i<m;i++){
	for(j=0;j<p;j++)
		printf("%d ",matrixRis[i][j]);	
	printf("\n");		
}

}



void * thread_produttore(void *arg){
int myLine=*((int *)arg);
int prod;


fflush(stdout);
int i,j,k;
for(i=0;i<p;i++){
	prod=0;
	for(j=0;j<n;j++)
		prod+=matrix1[myLine][j]*matrix2[j][i];
	
	pthread_mutex_lock(&mutex2);
	while(buf.value!=-1){
		pthread_cond_signal(&cond1);	
		pthread_cond_wait(&cond2,&mutex2);
	
	}	
	pthread_mutex_lock(&mutex1);
	printf("\nTrovato valore per %d%d -> %d\n",myLine,i,prod);
	fflush(stdout);
	
	buf.indI=myLine;
	buf.indJ=i;
	buf.value=prod;

	pthread_cond_signal(&cond1);
	pthread_mutex_unlock(&mutex1);	


	pthread_mutex_unlock(&mutex2);
	
	
	
}


}


void * thread_consumatore(void * arg){
int i,j,k;

for(i=0;i<m*p;i++){	
	printf("\nFACCIO\n");
	fflush(stdout);	
	pthread_mutex_lock(&mutex1);
	while(buf.value==-1){
		pthread_cond_signal(&cond2);
		pthread_cond_wait(&cond1,&mutex1);
	}
	matrixRis[buf.indI][buf.indJ]=buf.value;
	buf.value=-1;
	pthread_cond_signal(&cond2);
	pthread_mutex_unlock(&mutex1);	
	

}

pthread_exit(NULL);



}



