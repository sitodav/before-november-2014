/*Scrivere un programma C in ambiente Linux che facendo uso della libreria Pthread realizzi il seguente
comportamento: preso in input un intero n, il nome di un file di testo ed un carattere x, il programma ha
il compito di contare le occorrenze del carattere x nel file di testo. Il programma esegue tale compito
creando n thread, ognuno dei quali esamina una porzione diversa del file di testo (Esempio: se il file è
lungo 1000 byte ed n=4, il primo thread esaminerà i primi 250 byte, il secondo thread esaminerà i 250
byte successivi, e così via).
*/

//QUESTO E' UN PROBLEMA DI RACE CONDITION SULLA VAR CONDIVISA CHE CONTIENE IL CONTEGGIO TOTALE, E SUL FILE CHE ESSENDO CONDIVISO, OGNI VOLTA CHE UN THREAD DIVERSO CI LEGGE, SI SPOSTA IL PUNTATORE DELL'OFFSET,QUINDI OGNI THREAD PRIMA DI LEGGERE DEVE RIPOSIZIONARSELO PER LA SUA PORZIONE IN BASE A DOVE ERA ARRIVATO PRECEDENTEMENTE
//USIAMO MUTEX

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>


//GLOBALI CHE CREANO RACE CONDITION:
int tot=0; 
int fdFileR;

pthread_mutex_t mutex1=PTHREAD_MUTEX_INITIALIZER; 

//GLOBALI CHE NON CREANO RACE CONDITION:
int porzione=0; 
char key;

void * reader_thread_start(void * arg); 

int main(int argc,char *argv[]){
if(argc!=4){
	printf("\n nomefile.txt numthread key\n");
	exit(0);

}


int n,size;
struct stat info1;

sscanf(argv[3],"%c",&key);
sscanf(argv[2],"%d",&n);

if(stat(argv[1],&info1)<0){
	perror("\n");
	exit(1);
}

size=info1.st_size;
porzione=size/n;

printf("\nDobbiamo cercare il char %c nel file %s che e' grande %d,creando %d thread e ogni thread dovra' controllare su %d KBytes\n",key,argv[1],size,n,porzione);
fflush(stdout);

if((fdFileR=open(argv[1],O_RDONLY))<0){
	perror("\n");
	exit(1);
}

int i,ret,*startOff=(int *)calloc(n,sizeof(int)); //l'argomento da passare non puo' essere un'unica variabile perche' noi ne passiam ol'indirizzo, quindi tutti i thread si ritroverebbero ad avere lo stesso offset

pthread_t *tid=(pthread_t *)calloc(n,sizeof(pthread_t));
//creo i thread
for(i=0;i<n;i++){
	startOff[i]=i*porzione;
	if((ret=pthread_create(tid+i,NULL,reader_thread_start,(void *)(startOff+i)))!=0){
		printf("\n%s",strerror(ret));
		exit(1);	
	}
}

//aspetto che terminino i thread
for(i=0;i<n;i++)
	if((ret=pthread_join(tid[i],NULL))!=0){
		printf("\n%s\n",strerror(ret));
		exit(1);	
	}

printf("\nIl valore totale dell'occorrenza di %c e' %d\n",key,tot);
fflush(stdout);
//terminazione main thread e tutto processo

}


void * reader_thread_start(void * arg){


int startOff=*((int *)arg),i,parz=0;
char t;

printf("\nSono thread appena nato e mi e' stato assegnato di lavorare da %d a %d\n",startOff,(startOff+porzione));
fflush(stdout);

for(i=0;i<porzione;i++){
	pthread_mutex_lock(&mutex1);
	//INIZIO CRITICAL SECTION:
	lseek(fdFileR,startOff+i,SEEK_SET); //mi riposiziono sull'off di start che mi era stato assegnato + il valore iesimo di quanto mi ero spostato nei cicli precedenti di lettura
	if(read(fdFileR,&t,1)<0){
		perror("\n");
		exit(1);	
	}
	if(t==key){
		parz++; //parziale e' una variabile locale ad ogni thread, non deve stare per forza sott omutex, e la incrementiamo quando troviamo un'occorrenza, proprio come incrementiamo totale. Solo che parz e' solo nostra, quindi alla fine serve per vedere quali processi hanno trovato piu' a
		tot++;
	} 
	// FINE CRITICAL SECTION:
        pthread_mutex_unlock(&mutex1);	
}


printf("\nSono il thread che aveva lavorato da %d a %d e ho trovato %d occorrenze di %c\n",startOff,(startOff+porzione),parz,key);
fflush(stdout);
//e termina il thread ritornando dalla funzione
pthread_exit(NULL);





}

