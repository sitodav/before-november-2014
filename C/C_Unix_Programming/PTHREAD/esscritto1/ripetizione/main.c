#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>




pthread_mutex_t mutex1;
int sharedBuff=-1;

void *thread_produttore(void *);
void *thread_consumatore(void *);

int main(int argc,char *argv[]){

if(argc!=2){
	printf("uso: n\n"); 
	exit(1);
}

int n;
sscanf(argv[1],"%d",&n);

pthread_t idProduttore,idConsumatore;

int ret;
if((ret=pthread_create(&idProduttore,NULL,thread_produttore,(void *)&n))!=0){
	printf("\n25: %s\n",strerror(ret));
	exit(1);
}

if((ret=pthread_create(&idConsumatore,NULL,thread_consumatore,(void *)&n))!=0){
	printf("\n30: %s\n",strerror(ret));
	exit(1);
}

printf("\nEntrambi i thread creati, li aspetto!\n");
fflush(stdout);

if((ret=pthread_join(idProduttore,NULL))!=0){
	printf("%s",strerror(ret));
	exit(1);	
}

if((ret=pthread_join(idConsumatore,NULL))!=0){
	printf("%s",strerror(ret));
	exit(1);
}

printf("\nI 2 thread hanno terminato. Termino (main thread)!\n"); 
fflush(stdout);
return 0;


}




void *thread_produttore(void *arg){
srand((unsigned int)time(NULL));

int n=*((int *)(arg));

int i,toWrite;
for(i=0;i<n;i++){
	toWrite=rand()%10;
	
	while(1){
		pthread_mutex_lock(&mutex1);
		if(sharedBuff<0){
			sharedBuff=toWrite;
			pthread_mutex_unlock(&mutex1);
			printf("\nTHREAD PRODUTTORE: SCRITTO %d\n",toWrite); fflush(stdout);
			
			break;
		}			
		pthread_mutex_unlock(&mutex1);
	}
}

printf("\nTHREAD PRODUTTORE: TERMINATO, ESCO!\n");
pthread_exit((void *)NULL);

}

void *thread_consumatore(void *arg){
int n=*((int *)(arg));
int i;
int letto;

for(i=0;i<n;i++){
	while(1){
		pthread_mutex_lock(&mutex1);
		if(sharedBuff>=0){
			letto=sharedBuff;
			sharedBuff=-1;
			pthread_mutex_unlock(&mutex1);
			printf("\nThread CONSUMATORE: LETTO %d",letto);
			break;		
		}
		pthread_mutex_unlock(&mutex1);	
	}

}

printf("\nSono thread consumatore: terminato\n");
fflush(stdout);
pthread_exit((void *)NULL);
}
