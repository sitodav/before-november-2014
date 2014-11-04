#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h> //per le funzioni non pthread

//si usano n thread per contare le occorrenze del file passato da cmd line



//glob
int porzione; //e' globale il tot di byte assegnati ad ogni thread
//NB: NON PUO' ASSOLUTAMENTE ESSERE GLOBALE LA VARIABILE CHE MANTIENE IL NUMERO DI OCCORRENZE CALCOLATE FINO AD ORA, CI SAREBBERO SOLO RISULTATI INCONGRUENTI, VISTO CHE I THREAD LAVORANO IN CONCORRENZA, QUINDI RISOLVIAMO FACENDO CHE OGNI THREAD SI CALCOLA SOLO IL NUM OCCORRENZE RELATIVE ALLA PROPRIA PORZIONE, E LO RITORNA COME VAR DI RITORNO AL MAIN THREAD, CHE LE SOMMA

struct argomento{
	int startOffset;
	int fdFileR;

}; //QUESTA STRUCT ASSOLUTAMENTE NON VA DICHIARATA GLOBALE, ma va passata come argomento ad ogni nuovo thread, perche' se fosse globale, si otterrebbero risultati incoerenti 


void * conta_occ(void * arg);
//alla funzione del thread, dovremo passare 2 argomenti (quindi li mettiamo in una struct il quale indirizzo passiamo come UNICO argomento alla funzione conta_occ attribuita ogni thread)
//un argomento che indica l'offset sul quale lavorare (ogni thread lavora su una porzioen di file) e un argomento che indica il fd del file condiviso...questa avremmo potuto anche metterla globale, e semplificare ma per esercitarmi...)


int main(int argc,char *argv[]){
if(argc!=3){
	printf("\nn // file\n"); 
	exit(1);
}

int n,i,fdFileR;
pthread_t *tid;

sscanf(argv[1],"%d",&n);
if((fdFileR=open(argv[2],O_RDONLY))<0){
	perror("\n");
	exit(1);
}

printf("\nFile aperto\n"); fflush(stdout);

//alloco dinamicamente array per i TID
tid=calloc(n,sizeof(pthread_t));

//la struct NON puo' essere unica, perche' noi ne passiamo l'indirizzo alle funzioni dei thread


//ora vediamo quanto e' grande il file
struct stat info1;

if(stat(argv[2],&info1)<0){
	perror("\n");
	exit(1);
}

int sizeFile;
sizeFile=info1.st_size;
printf("\nIl file %s e' grande %d bytes\n",argv[2],sizeFile);
fflush(stdout);
porzione=sizeFile/n;
printf("\nOgni thread dovra' lavorare su un tot di %d bytes\n",porzione);
fflush(stdout);

struct argomento **arg1=(struct argomento **)calloc(n,sizeof(struct argomento *));
//uso un array di puntatori a struct, 1 per ogni thread, perche' non si puo' allocare e deallocare sempre sullo stesso puntatore perche' essendo passata per riferimento, quando il thread lavora nella sua funzione, quella struct deve esistere

int ret;
for(i=0;i<n;i++){
	arg1[i]=(struct argomento *)calloc(1,sizeof(struct argomento));
	arg1[i]->fdFileR=fdFileR;	
	arg1[i]->startOffset=i*porzione;
	ret=pthread_create(tid+i,NULL,conta_occ,(void *)arg1[i]);
	//assegnato al nuovo thread creato, la funzione conta_occ passandogli come argomento
	//l'ind della struct contenente l'offset da cui deve partire e il fd del file da leggere
	if(ret<0){
		printf("\n%s\n",strerror(ret));
		fflush(stdout);
		exit(1);	
	}
	printf("\nSono padre: creato thread %d-esimo\n",i);
	fflush(stdout);
}

//e ora aspettiamo la terminazione di tutti i thread creati, joinandoli e prendendoci il valore di return di ognuno, che sara' il suo conto delle occorrenze e le sommiamo

int totOcc=0,*parzOcc; //il valore restituito dalla funzione attribuita al thread puo' essere solo un puntatore, quindi in questo caso sara' un puntatore ad intero restituito castato void, e noi dovremo passare alla join indirizzo di un puntatore ad int, castato void, affinche' lo modifichi e gli faccia puntare l'indirizzo restituito dalla funzione attribuita ad ogni thread

for(i=0;i<n;i++){
	ret=pthread_join(tid[i],(void *)&parzOcc);
	if(ret<0){
		printf("\n%s",strerror(ret));
		fflush(stdout);
		exit(1);	
	}
	printf("\nSono main thread, il thread %d-esimo mi ha restituito come occ %d\n",i,*parzOcc);
	fflush(stdout);
	totOcc+=*parzOcc;
	
}

printf("\nSono main thread: il tot delle occorrenze e' %d\n",totOcc);
fflush(stdout);


}


void * conta_occ(void * arg){
//non possiamo leggere dall'argomento ricevuto direttamente, ma dobbiamo ricastarlo in struct argomento
struct argomento *arg1;
arg1=(struct argomento *)arg;
printf("\nSono un thread, opero nella porzione da %d a %d del file fd=%d\n",arg1->startOffset,(arg1->startOffset)+porzione,arg1->fdFileR);
fflush(stdout);
//NB SICCOME I THREAD CONDIVIDONO L'OFFSET, NEL MOMENTO IN CUI UN THREAD SI POSIZIONA E INIZIA A CONTARE LE X, QUALCHE ALTRO THREAD IMPOSTA IL NUOVO OFFSET, E QUINDI IL THREAD DI PRIMA SI RITROVA ANCHE LUI SPIAZZATO, USIAMO QUINDI UNA SLEEP PARI ALL'INDICE DI GENERAZIONE
//CHE SI PUO' RIOTTENERE FACENDO L'OFFSET SU CUI GLI E' STATO INDICATO DI LEGGERE/PORZIONE
int myInd;
myInd=arg1->startOffset/porzione;
sleep(myInd);
if(lseek(arg1->fdFileR,arg1->startOffset,SEEK_SET)<0){
	perror("\n");
	exit(1);
}
int *occ,i;
//il tot delle occorrenze lo allochiamo nell'heap per evitare problema...
char t;
occ=calloc(1,sizeof(int));
for(i=arg1->startOffset;i<(arg1->startOffset)+porzione;i++){
	if(read(arg1->fdFileR,&t,1)<0 && errno!=EINTR){
		perror("\n");
		exit(1);
	}
	if(t=='x')
		(*occ)++;	
}
printf("\nSono uno dei thread e ho trovato %d x nella porzione %d-%d",*occ,arg1->startOffset,(arg1->startOffset)+porzione);
fflush(stdout);
//e ritorna ind della var intera occ, castato come void *
pthread_exit((void *)occ);


}


