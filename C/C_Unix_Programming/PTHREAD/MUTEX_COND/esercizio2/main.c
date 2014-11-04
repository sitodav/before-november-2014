//es 2 del libro di cui ho capito male il testo

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>



// VAR GLOBALI
pthread_mutex_t mutex1=PTHREAD_MUTEX_INITIALIZER; //mutex e var di condizione inizializzate staticamente a default
pthread_cond_t cond1=PTHREAD_COND_INITIALIZER;
int contatore=0;
int sumtot=0;

void * primidue_start(void * arg);
void * terzo_start(void * arg);

int main(int argc,char *argv[]){
int maxCont; //questo non lo mettiamo globale, ma lo passiamo come argometo arg unico alle funzioni attribuite ai thread

if(argc!=2){
	printf("\nUso: nMaxCont\n");
	exit(0);
}

sscanf(argv[1],"%d",&maxCont);

pthread_t tid[3];
int ret,i;

//creo i primi 2 thread
for(i=0;i<2;i++)
	if((ret=pthread_create(tid+i,NULL,primidue_start,(void *)&maxCont))!=0){
		printf("ERRORE: %s",strerror(ret));
		exit(1);
	}


if((ret=pthread_create(tid+2,NULL,terzo_start,(void *)&maxCont))!=0){
	printf("\nErrore: %s\n",strerror(ret));
	exit(1);

}

//attendo la terminazione dei 3 thread
for(i=0;i<3;i++)
	if((ret=pthread_join(tid[i],NULL))!=0){ //non ci interessa la strutt dati eventualmente ritornata dai thread
		printf("\n%s",strerror(ret));
		exit(1);	
	}	
printf("\nMain thread terminante!\n"); fflush(stdout);
exit(0); //equivalente ad una return 0


}

void * primidue_start(void * arg){
//per prima cosa ci prendiamo il valore max contatore al quale fermarci, che abbiam oricevuto come argomento di input
int maxCont=*((int *)arg);

while(1){
	pthread_mutex_lock(&mutex1);
	if(contatore==maxCont){
		pthread_mutex_unlock(&mutex1); //il terzo lo dovremmo sbloccare o dopo questa unlock...
		return NULL;	
	}
	//altrimenti
	//il numero mi scoccio di farlo casuale, incrementa 1
	printf("\nSono thread %d, tocca a me incrementare\n",(unsigned int)pthread_self());
	fflush(stdout);
	sumtot++;
	contatore++;
	pthread_mutex_unlock(&mutex1);
	if(contatore==maxCont) //cioe' se ha raggiunto il massimo solo dopo che NOI abbiamo incrementato (se gia' stava sul massimo sarebbe ritornato all'if precedente...
//...ma lo sblocchiamo qui, in modo tale che solo l'ultimo thread dei primi 2 che modifica cont per l'ultima volta lo sveglia (cosi' si evitano 2 signal d icui una sarebbe inutile)
		pthread_cond_signal(&cond1);


}



}

void * terzo_start(void * arg){

//controlla se cont ha raggiunto massimo, altrimenti si ferma e aspetta la segnalazione dall'ultimo dei primi 2 che finisce di incrementare
int maxCont=*((int *)arg);

pthread_mutex_lock(&mutex1); //poiche' mutex 1 e' anche il mutex associato alla var di condizione (o piu' precisamente alla variabile contenuta nel suo predicato)

while(contatore!=maxCont)
	pthread_cond_wait(&cond1,&mutex1);
//se valutando la prima volta (sottomutua esclusione perche' abbiamo chiuso prima il mutex1) il valore di contatore, risulta non pronto, allora va in wait (e sblocca automaticamente il mutex1). Quando qualcuno gl imanda signal (l'ultimo a finire dei 2) riblocca il mutex, e ricontrolla il valore del predicato. Se e' valido, continua, stampa, risblocca il mutex (anche se e' superfluo perche 'sicuramente nessun altro dovra' usarla) e termina

printf("\nSono thread %d, i primi 2 hann ofinito di incrementare!\n",(unsigned int)pthread_self());
pthread_mutex_unlock(&mutex1);
return NULL;

}




