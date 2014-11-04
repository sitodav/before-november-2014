/*
Si realizzi un programma C e Posix in ambiente Linux che, impiegando la libreria Pthread, crei una
matrice di interi di dimensione nxn con n passato come argomento da riga di comando, e provveda a
sommare concorrentemente ciascuna colonna della matrice e a determinarne il minimo il cui valore deve
esser assegnato ad una variabile globale minimo.
*/


#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#define APERTA 0
#define CHIUSA 1

//facciamo che il main thread crea la matrice e la riempie, poi ognuno degli n thraed concorrentemente usando variabili di lock, aggiorna il contenuto della var globale min, e il main thread prima di ritornare, stampa il valore di questa
//variabile globale
//IN REALTA' SI POTREBBE USARE UNA SINGOLA VARIABILE DI LOCK SULLA VARIABILE GLOBALE, MA VA IMPLEMENTATA USANDO UN OPERAZIOE INDIVISIBILE. SICCOME NON L'HO ANCORA STUDIATA, CREO UN SET DI VARIABILI DI LOCK, E OGNI THREAD CHE RIESCE AD ACCEDERE ALLA SEZIONE CRITICA (VALUTAZIONE VARIABILE MIN) QUAND OHA TERMINATO, SBLOCCA SOLO IL THREAD I-ESIMO SUCCESSIVO, OVVERO SOLO LA LOCK PER QUEL THREAD, QUINDI SOLO QUELLO POTRA' USCIRE DALLA BUSY WAIT. IN QUESTO MODO NON PUO' CAPITARE CHE 2 O PIU' THREAD SUPERINO CONTEMPORANEAMENTE LA BUSY WAIT, PERCHE' LE LOCK VAR SONO DEDICATE E SOLO 1 SULLE N ALLA VOLTA SI SBLOCCA. OVVIAMENTE QUESTO APPROCCIO CREA DEADLOCK NEL CASO IN CUI CRASHI UNO QUALUNQUE DEI PROCESSI


//globale
int *lockDedicate; //siccome non so quante lock in tutto andranno fatte (perche' n non lo conosco ancora, alloco dinamicamente)
int min;
int **matrix;
int n;



void * start_thread(void *arg);
int main(int argc,char *argv[]){

srand((unsigned int)time(NULL));
if(argc!=2){
	printf("\nUso: n\n"); 
	exit(0);
}


sscanf(argv[1],"%d",&n);
matrix=(int **)calloc(n,sizeof(int *));

int i;
for(i=0;i<n;i++)
	matrix[i]=(int *)calloc(n,sizeof(int));
int k;
for(i=0;i<n;i++){
	for(k=0;k<n;k++){
		matrix[i][k]=rand()%21;
		printf("%d ",matrix[i][k]);
	}
	printf("\n");
}

//alloco l'array di lock dedicate
lockDedicate=(int *)calloc(n,sizeof(int));
//e le blocco tutte per ora
for(i=0;i<n;i++)
	lockDedicate[i]=CHIUSA;
//creo un array di id in modo tale che passo ad ogni thread, a seconda dell'indice a cui e' stato generato
//l'indirizzo del solo elemento iesimo dell'array, che conterra' l'indice, perche' se gli passo l'indirizzo dell'indice che faccio girare nel ciclod i creazione dei thread, ovviamente quello si aggiorna e loro gli puntano con indirizzo, quindi finirebbero per avere tutti lo stesso 
int *arrayId;
arrayId=(int *)calloc(n,sizeof(int));
for(i=0;i<n;i++)
	arrayId[i]=i;

pthread_t *tid=calloc(n,sizeof(pthread_t));


int ret;
for(i=0;i<n;i++){
	ret=pthread_create(tid+i,NULL,start_thread,(void *)&arrayId[i]);
	if(ret<0){
		printf("\n%s",strerror(ret));
		exit(1);	
	}
	
}

//il main thread sblocca il primo thread attivandogli la lock in 0
lockDedicate[0]=APERTA;
//non mi interessa la variabile ritornata dai figli, tuttavia aspetto che terminino
for(i=0;i<n;i++){
	ret=pthread_join(tid[i],NULL);
	if(ret<0){
		printf("\n%s",strerror(ret));
		exit(1);	
	}
}

printf("\nSono main thread, il valore min e' %d",min);
fflush(stdout);


}


void * start_thread(void *arg){
//la funzione del thread riceve in input il suo indice di generazione, cosi' che sapra' qual e' la sua lock dedicata e qual e' la colonna sulla quale deve lavorare


int myid=*(int *)arg;
fflush(stdout);
printf("\nSono thread con id %d",myid);
fflush(stdout);
int mySum=0;
int i;

while(lockDedicate[myid]==CHIUSA);//busywait sulla mia sola lock dedicata, che mi ha aperto il thread prima di me
for(i=0;i<n;i++)
	mySum+=matrix[i][myid];
printf("\nSono thread %d e la mia somma col e' %d",myid,mySum);

//CS:
//se il thread e' il primo (myid==0) allora il minimo lo imposta lui, perche' sicuramente essendo il primo valore valutato, sara' il minimo fino a QUEL MOMENTO (e possiamo farlo perche' sappiamo che sicuramente il prim othread che parte e' quello con id 0)...questo e' per avere semplicemente un valore di inizializzazione corretto della variabile con cui paragonare i minimi eventuali

if(myid==0)
	min=mySum;
else if(mySum<min) //altrimenti tutti gl ialtri thread pongono il minimo solo se la loro somma colonna i-esima e' minore del minimo precedentemente trovato
	min=mySum;
//FINE CS
if(myid!=n-1) //ovviamente se ero l'ultimo thread mi fermo
	lockDedicate[myid+1]=APERTA;
lockDedicate[myid]=CHIUSA; //superfluo
pthread_exit(NULL);

}
