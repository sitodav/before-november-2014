/*Si realizzi un programma C e Posix in ambiente Linux che, impiegando la libreria Pthread, generi tre
thread. I primi due thread sommano 1000 numeri generati casualmente ed ogni volta incrementano un
contatore. Il terzo thread attende che il contatore incrementato dai due thread raggiunga un valore limite
fornito da riga di comando, notifica l’avvenuta condizione e termina. Utilizzare le variabili condizione.
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <time.h>


//var globali da sincronizzare 
int sumCont=0;

pthread_mutex_t mutex1=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond1=PTHREAD_COND_INITIALIZER;


//var globali non da sincronizzare


void * thread_sommatori(void *);
void * thread_controllore(void *);

int main(int argc,char *argv[]){
if(argc!=2){
	printf("\nUSO: soglia\n");
	fflush(stdout);
}
srand((unsigned int)time(NULL));

int soglia;
sscanf(argv[1],"%d",&soglia); //lo passiamo come parametro al controllore

pthread_t tidSommatori[2],tidControllore; 
int i,ret;

//creo controllore
if((ret=pthread_create(&tidControllore,NULL,thread_controllore,(void *)&soglia))!=0){
	printf("%s",strerror(ret));
	exit(1);
}
for(i=0;i<2;i++){
	if((ret=pthread_create(tidSommatori+i,NULL,thread_sommatori,NULL))!=0){
		printf("%s",strerror(ret));
		exit(1);
	}
}

//il main thread si mette in attesa che finisca il solo controllore
//per esercitarmi, acquisisco strutt dati di uscita del controllore
int *p;
if((ret=pthread_join(tidControllore,(void **)&p))!=0){
	printf("%s",strerror(ret));
	exit(0);
}

printf("\nSono main thread: il thread controllore e' terminato restituendo valore di %d\n",(int)p);


exit(0);
}



void * thread_sommatori(void * arg){
int casuale,i;
//per testo comunque non fa piu' di 1000 somme
i=0;
while(i<1000){
	casuale=1+rand()%3; //tra 1 e 3
	i++;
	pthread_mutex_lock(&mutex1);
	sumCont+=casuale;
	pthread_cond_signal(&cond1); //segnala il controllore sulla variabile di condizione
	pthread_mutex_unlock(&mutex1);
	
}




}


void * thread_controllore(void * arg){
int soglia=*((int *)arg);

pthread_mutex_lock(&mutex1);
while(soglia>sumCont)
	pthread_cond_wait(&cond1,&mutex1);
//quando arriviamo qui, stiamo col mutex acquisito (bloccato in nostro favore) e la soglia e' stata superata. Quindi non sblocchiamo il mutex, altrimenti nel tempo che andiamo a stampare il valore del sumCont rispetto alla soglia, i sommatori farebbero altre somme. Quindi li lasciamo in deadlock perche' non devono piu' fare niente !
printf("\nE' stata superata la soglia di %d con un valore di %d\n",soglia,sumCont);
fflush(stdout);
pthread_exit((void *)1);


}
