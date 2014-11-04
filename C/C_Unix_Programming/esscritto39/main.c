/*Calcolare il prodotto di due matrici (di dimensioni mxn e nxp) di interi, allocate dinamicamente, facendo
uso di m thread concorrenti
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <time.h>


//non richiede uso di mutex/var di condizioni questo esercizio





int **matrix1,**matrix2,**matrixRis;
int m,n,p;

void * thread_start(void *arg);

int main(int argc,char *argv[]){
if(argc!=4){
	printf("\nPassami m,n,p\n");
	exit(1);
}

srand((unsigned int)time(NULL));


int *indici;

sscanf(argv[1],"%d",&m);
sscanf(argv[2],"%d",&n);
sscanf(argv[3],"%d",&p);
int i,j;

printf("m%d   n%d   p%d\n",m,n,p);
//matrice mXn
matrix1=(int **)calloc(m,sizeof(int *));
for(i=0;i<m;i++){
	matrix1[i]=(int *)calloc(n,sizeof(int));
	for(j=0;j<n;j++){
		matrix1[i][j]=1+rand()%4;
		printf("%d ",matrix1[i][j]);	
	}
	printf("\n");

}

printf("\n\n");
//matrice nXp
matrix2=(int **)calloc(n,sizeof(int *));
for(i=0;i<n;i++){
	matrix2[i]=(int *)calloc(p,sizeof(int));
	for(j=0;j<p;j++){
		matrix2[i][j]=1+rand()%4;
		printf("%d ",matrix2[i][j]);	
	}
	printf("\n");

}

printf("\n\n");

//matrice mXp
matrixRis=(int **)calloc(m,sizeof(int *));


matrixRis=(int **)calloc(m,sizeof(int *));

for(i=0;i<m;i++)
	matrixRis[i]=(int *)calloc(p,sizeof(int));

pthread_t *tid=(pthread_t *)calloc(m,sizeof(pthread_t));
indici=calloc(m,sizeof(sizeof(int)));

int ret;

for(i=0;i<m;i++){
	indici[i]=i;
	if((ret=pthread_create(tid+i,NULL,thread_start,(void *)(indici+i) ))!=0){
		printf("%s",strerror(ret));
		exit(1);	
	}
}
	
for(i=0;i<m;i++){
	ret=pthread_join(tid[i],NULL);
	if(ret!=0){
		printf("%s",strerror(ret));
		exit(1);	
	}
}	


printf("\nMATRICE RISULTANTE:\n");

for(i=0;i<m;i++){
	for(j=0;j<p;j++)
		printf("%d ",matrixRis[i][j]);		
	printf("\n");		
}

exit(0);

}


void * thread_start(void *arg){
int myLine=*((int *)arg);
printf("\nSono %u e lavoro su riga %d\n",(unsigned int)pthread_self(),myLine);

int i,l,k,finale;

for(l=0;l<p;l++){
	finale=0;
	for(k=0;k<n;k++){
		printf("\nID THREAD %d:>>Matrix1[%d][%d] * Matrix2[%d][%d]\n",myLine,myLine,k,k,l);
		fflush(stdout);
		finale+=matrix1[myLine][k]*matrix2[k][l];
	}
	printf("\nVa in matrixRis[%d][%d]\n",myLine,l);
	fflush(stdout);
	//matrixRis[myLine][l]=finale;

}

pthread_exit((void *)NULL);



}
