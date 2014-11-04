#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>




//var glob da sincr all'accesso
struct finale{
	int max;
	pthread_t threadAssegnato;
} finalMax={0,0};


pthread_mutex_t mutex1;


void * start_thread(void *);

int main(int argc,char *argv[]){


int nMatrici,i,j;
if(argc!=2){
	printf("\nUso n matrici!\n");
	exit(0);
}

srand((unsigned int)time(NULL));

sscanf(argv[1],"%d",&nMatrici);

//STO USANDO UNA STRUTTURA CUBICA DI MATRICI 2D IN PILA UNA SULL'ALTRA

int ***listaMatrici;//puntatore ad array di puntatori a puntatori
listaMatrici=(int ***)calloc(nMatrici,sizeof(int **));
for(i=0;i<nMatrici;i++){
	*(listaMatrici+i)=(int **)calloc(3,sizeof(int *));
	for(j=0;j<3;j++)
		*(*(listaMatrici+i)+j)=(int *)calloc(3,sizeof(int));
	
}

int k;

//assegno valore
for(i=0;i<nMatrici;i++){
	for(j=0;j<3;j++)
		for(k=0;k<3;k++)
			 listaMatrici[i][j][k]=1+rand()%100;
}

//stampo
for(i=0;i<nMatrici;i++){
	printf("\n\n\nNUOVA MATRICE->\n");
	for(j=0;j<3;j++){
		for(k=0;k<3;k++)
			 printf("%d ",listaMatrici[i][j][k]);
		printf("\n");
        }
	
}


int ret;
pthread_t *tid;
tid=(pthread_t *)calloc(nMatrici,sizeof(pthread_t));
for(i=0;i<nMatrici;i++){
	if((ret=pthread_create(tid+i,NULL,start_thread,(void *)*(listaMatrici+i)))!=0){
		printf("\n%s\n",strerror(ret));
		exit(0);	
	}

}


for(i=0;i<nMatrici;i++){
	if((ret=pthread_join(*(tid+i),NULL))!=0){
		printf("%s",strerror(ret));
		exit(1);	
	}
}

printf("\nA quanto pare il massimo valore sulla mediana e' %d e stava nella matrice assegnata al thread %u\n",finalMax.max,(unsigned int)finalMax.threadAssegnato);
fflush(stdout);




}

void * start_thread(void * arg){
int **matrix=(int **)arg;
int i,j;

printf("\nSono thread con tid:%u al quale e' arrivata la matrice->\n",(unsigned int)pthread_self());

for(i=0;i<3;i++){
	for(j=0;j<3;j++)
		printf("%d ",matrix[i][j]);
	printf("\n");
}

int mymax=0;

for(i=0,j=0;i<3 && j<3;i++,j++){
	if(matrix[i][j]>mymax)
		mymax=matrix[i][j];
}

pthread_mutex_lock(&mutex1);
if(mymax>finalMax.max){
	finalMax.max=mymax;
	finalMax.threadAssegnato=pthread_self();
}
pthread_mutex_unlock(&mutex1);
pthread_exit((void *)NULL);



}

