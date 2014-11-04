/*Si realizzi un programma in C e Posix sotto Linux che, con l’ausilio della libreria Pthread, lancia n thread
per cercare la mediana di valore massimo tra le n righe di una matrice nxn di interi. Individuato il valore
cercato, si provvederà a stamparlo a video. La dimensione della matrice può essere fornita in input al
programma in fase di esecuzione o da riga di comando.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <time.h>

//var glob da sincronizzare
int maxMediana=0;

pthread_mutex_t mutex1=PTHREAD_MUTEX_INITIALIZER;

//var glob non da sincronizzare
int **matrix;
int n;

void * thread_cercatore(void * arg);

int main(int argc,char *argv[]){
int i,j,ret,*indici;
if(argc!=2){
	printf("\nUso : size n\n");
	exit(1);

}

srand((unsigned int)time(NULL));
sscanf(argv[1],"%d",&n);

matrix=(int **)calloc(n,sizeof(int *));
for(i=0;i<n;i++){
	matrix[i]=(int *)calloc(n,sizeof(int));
	for(j=0;j<n;j++){
		matrix[i][j]=1+rand()%20;
		printf("%d ",matrix[i][j]);	
	}
	printf("\n");
}

indici=(int *)calloc(n,sizeof(int));

pthread_t *tid=(pthread_t *)calloc(n,sizeof(pthread_t));
for(i=0;i<n;i++){
	indici[i]=i;
	if((ret=pthread_create(tid+i,NULL,thread_cercatore,(void *)(indici+i)))!=0){
		printf("%s",strerror(ret));
		exit(1);	
	}
}

for(i=0;i<n;i++){
	ret=pthread_join(tid[i],NULL);
	if(ret!=0){
		printf("\n%s",strerror(ret));
		exit(1);	
	}

}

printf("\nIl massimo mediano e' %d\n",maxMediana);
fflush(stdout);
exit(0);

}



void * thread_cercatore(void * arg){
int myLine=*((int *)arg);
int j,sum=0,myMediana;
if(n%2==0){
	for(j=0;j<n;j++)
		sum+=matrix[myLine][j];
	myMediana=sum/n;
}

else
	myMediana=matrix[myLine][n/2];
pthread_mutex_lock(&mutex1);
if(myMediana>maxMediana)
	maxMediana=myMediana;
pthread_mutex_unlock(&mutex1);

return NULL;


}
