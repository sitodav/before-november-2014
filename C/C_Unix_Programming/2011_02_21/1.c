//VERSIONE 1: IL SEGNALATORE DELLA VAR DI CONDIZIONE E' LO SCRITTORE ED IL SEGNALATO IL CONTROLLORE


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#define file "file1.txt"

//var glob per le quali sincronizzare l'accesso
int fdFileRW;
int actualN=0;



pthread_mutex_t mutex1=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond1=PTHREAD_COND_INITIALIZER;


void * thread_scrittore(void * arg);
void * thread_lettore(void * arg);
void * thread_controllore(void *arg);

int main(int argc,char *argv[]){
if(argc!=2){
	printf("\nPassami n\n");
	exit(1);
}

int nMax; //il massimo numero a cui arrivare lo passiamo come parametro alle funzioni dei thread tanto non deve essere modificato, quindi per esercitarmi lo passo, e sara' lo stesso passato per riferimento allo scrittore ed al controllore

sscanf(argv[1],"%d",&nMax);
if(nMax<7 || nMax>29){
	printf("\nN deve essere >=7 e <=29\n");
	exit(1);
}
//apro file
if((fdFileRW=open(file,O_RDWR | O_CREAT,00777))<0){
	perror("\n");
	exit(1);
}

int ret;
pthread_t *tid=(pthread_t *)calloc(3,sizeof(pthread_t));

if((ret=pthread_create(tid,NULL,thread_scrittore,(void *)&nMax))!=0){
	printf("%s",strerror(ret));
	exit(1);
}

if((ret=pthread_create(tid+1,NULL,thread_lettore,NULL))!=0){
	printf("%s",strerror(ret));
	exit(1);
}

if((ret=pthread_create(tid+2,NULL,thread_controllore,(void *)&nMax))!=0){
	printf("%s",strerror(ret));
	exit(1);
}

//e aspetto solo il controllore
if((ret=pthread_join(tid[2],NULL))!=0){
	printf("%s\n",strerror(ret));
	exit(1);
}

exit(0); //termina main thread e tutti i thread assieme al processo
}


void *thread_scrittore(void * arg){ //il thread scrittore usa nel ruolo di segnalatore la variabile di condizione,quindi c'e' mutex2 per evitare race condition sull'accesso alla variabile del predicato(actualN) della var di condizione, e il mutex1 per evitare race condition col lettore nell'accesso dal file. Il controllore invece non accede al file, ma solo alla variabile "actualN" per vedere qual e' stato l'ultimo numero trascritto dallo scrittore..quindi per il controllore actualN sara' la variabile contenuta nel predicato, che quindi sara' sotto mutex2 per avere mutua esclusione

int nMax=*((int *)arg);

while(1){
	//CS RISPETTO AL CONTROLLORE
	pthread_mutex_lock(&mutex2); //lock mutex associato alla condizione nel predicato della variabile di condizione
	actualN++; 
	if(actualN>nMax){ //se il predicato e' vero, cioe' se deve intervenire il controllore (problema di segnalazione)...
		pthread_cond_signal(&cond1); //fai signal e sblocca mutex associato alla variabile di condizione
		pthread_mutex_unlock(&mutex2);
		break;	
	}
	// sblocca mutex2, blocca mutex1 associato all'accesso al file, trascrivi ActualN sul file, sblocca mutex1
	
	//Fine sezione critica per l'accesso alla variabile contenuta nel predicato della variabile di condizione
	pthread_mutex_unlock(&mutex2);
	//CS RISPETTO AL LETTORE
	pthread_mutex_lock(&mutex1);

	lseek(fdFileRW,0,SEEK_SET);

	if(write(fdFileRW,&actualN,sizeof(int))<0){
		perror("\n");
		exit(1);	
	}
	
	printf("\nSono scrittore e ho scritto %d\n",actualN);
	fflush(stdout);

	pthread_mutex_unlock(&mutex1);
	
}


}




void * thread_lettore(void * arg){
//non ci interessa il massimo, noi dobbiamo solo leggere in mutua esclusione dal file
int t;
while(1){ //il lettore non si ferma mai
	pthread_mutex_lock(&mutex1);
	
	lseek(fdFileRW,0,SEEK_SET);

	if(read(fdFileRW,&t,sizeof(int))<0){
		perror("\n");
		exit(1);	
	}
	printf("\nSono lettore e ho letto %d\n",t);
	fflush(stdout);
	pthread_mutex_unlock(&mutex1);
}

}


void * thread_controllore(void * arg){
int nMax=*((int *)arg);
//accesso in CS con mutua esclusione (accediamo alla variabile contenuta nel predicato della variabile di condizione, sotto mutua esclusione garantita dal mutex2)
pthread_mutex_lock(&mutex2);
while(actualN<=nMax)
	pthread_cond_wait(&cond1,&mutex2);
printf("\nSONO IL CONTROLLORE: ACTUALN E' %d\nTERMINIAMO\n",actualN);
fflush(stdout);
pthread_exit(NULL);



}
