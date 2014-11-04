#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#define CHIUSA 1
#define APERTA 0

/*Si realizzi un programma in C e Posix in ambiente Linux che gestisca la seguente interazione di processi.
Il sistema consiste di 3 processi: un processo padre (P1) che provvede alla creazione di 2 processi figli
(P11 e P12); dopo la creazione dei figli, provvede a generare casualmente 20 numeri compresi tra 50 e
100 e un messaggio di terminazione, scrivendoli su una pipe; P11 e P12 attendono da P1 un segnale
SIGUSR1 di inizio di attività ; P11 e P12 prelevano in modo competitivo i numeri dalla pipe e li
trascrivono, ciascuno su un proprio file; P1 deve attendere il completamento dei figli.
*/


//glob
//le variabili di lock sono globali rispetto al main,ma visto che stiamo programmando in multiprocesso, ogni 
//processo figlio avra' la sua diversa variabile di lock. La facciamo cambiare dal signal handler, ecco perche' la mettiamo globale
int startLock=CHIUSA;
int miaLock=CHIUSA; 
//NB METRE IN AMBIENTE MULTITHREAD, SE OGNI THREAD HA BISOGNO DI UNA LOCK DEDICATA, SI CREANO 2 VARIABILI DI LOCK,
//QUI ESSENDO IN AMBIENTE MULTIPROCESSO, L'UNICA VARIABILE DI LOCK MIALOCK DEL PADRE, VIENE EREDITATA COME COPIA 2 VOLTE 
//DAI FIGLI, E OGNUNA SARA' INDIPENDENTE

void handler(int sign); //unico handler sia per SIGUSR1 che SIGUSR2

int main(int argc,char *argv[]){
int childpid[2],i,ret,t;
int fdPipe[2]; //questa pipe ha dal lato scrittura il padre, e dal lato lettura i 2 figli, quindi occorre sincronizzare
//l'accesso dei figli alla pipe, per evitare la race condition
//usiamo SIGUSR1 dal padre ai figli, per dire ai figli di iniziare a leggere, e i figli tra loro si sincronizzano con SIGUSR2 e variabili di lock e busy wait. Quindi le variabili di lock saranno 3, 1 per lo start e 2 per la sincronizzazione tra i figli
srand((unsigned int)time(NULL));
//per evitare di dover innstallare i signal handler sui 2 figli, li installo sul padre prima delle fork()
if(signal(SIGUSR1,handler)==SIG_ERR || signal(SIGUSR2,handler)==SIG_ERR){
	perror("\n");
	exit(1);
}

//creo la pipe
if(pipe(fdPipe)<0){
	perror("\n");
	exit(1);
}

//creo figli
for(i=0;i<2;i++){
	childpid[i]=fork();
	if(childpid[i]<0){
		perror("\n");
		exit(1);	
	}
	else if(childpid[i]==0) //figlio
		break;

}


if(i==2){
	//padre
	close(fdPipe[0]);
	//il padre da start ad entrambi i figli (SIGUSR1 ad entrambi) e sblocca la miaLock sono ad uno dei 2
	//i 2 figli poi continueranno ad alternarsi bloccandosi e sbloccandosi tra di loro
	if(kill(childpid[0],SIGUSR1)<0 || kill(childpid[1],SIGUSR1)<0 ){
		perror("\n");
		exit(1);	
	}
	//i figli ora sono sbloccati, ma bloccati su miaLock
	//sblocco solo il primo
	if(kill(childpid[0],SIGUSR2)<0){
		perror("\n");
		exit(1);	
	}
	//e manda pid secondo figlio al primo figlio
	if(write(fdPipe[1],childpid+1,sizeof(int))<0){
		perror("\n");
		exit(1);	
	}
	//e ora inizio a generare i 20 int e a mandarli sulla pipe
	int randN;
	int k;
	for(k=0;k<20;k++){
		randN=50+rand()%51;
		printf("\nSono padre, generato %d, lo metto su pipe\n",randN); fflush(stdout);
		if(write(fdPipe[1],&randN,sizeof(int))<0){
			perror("\n");
			exit(1);		
		}
	}
	printf("\nSONO PADRE: HO TERMINATO\n"); fflush(stdout);
	close(fdPipe[1]); //devo chiudere la mia estremita' in scrittura, altrimenti i figli rimarrann obloccati sulla read sulla pipe
	//e mi metto in attesa dei figli
	for(k=0;k<2;k++)
		if(wait(NULL)<0){ //non ci interessa stato di uscita dei figli
			perror("\n");
			exit(1);		
		}
	printf("\nSono padre, i miei figli sono terminati e termino anche io\n"); fflush(stdout);
	exit(0);
}

else if(i==0){ //siccome i 2 figli devono mandarsi i segnali, il secondo ha pid del primo, ma non viceversa. Quindi facciamo che il padre come primo numero manda il pid del secondo figlio al primo, quindi usiamo una var booleana nel primo figlio per sapere se stiamo ricevendo uno dei 20 numeri normali o pid primo figlio
	//primo figlio
	int otherChild=-1;
	close(fdPipe[1]);
	while(startLock==CHIUSA); //busy wait in attesa dello start del padre inviatoci tramite SIGUSR1
	//ora inizia il vero corpo del figlio
	while(1){
		while(miaLock==CHIUSA); //busy wait su miaLock (ne vengo sbloccato o al primo giro soltanto dal padre visto che sono io il primo figlio, o ogni volta che il secondo figlio termina il suo giro di lavoro)
		//INIZIO CS<----------
		if((ret=read(fdPipe[0],&t,sizeof(int)))<0){ //leggo da pipe in maniera esclusiva
			perror("\n");
			exit(1);		
		}
		
		
		
		printf("\nSONO FIGLIO 0 LETTO %d",t); fflush(stdout);
		//FINE CS<------
		//se quello che ho ricevuto era un falso start, ovvero era solo il pid dell'altro figlio, siccome i osono il primo figlio e devo iniziare sempre io, non mi sblocco (dopo aver sbloccat l'altro) ma mi riprendo direttamente un altro giro dopo essermi salvato pid dell'altro figlio
		if(otherChild==-1){
			otherChild=t;
			continue;		
		}
		
		//quando al successivo giro ricevero' uno dei 20 numeri, il flusso continuera' direttamente qui...
		//quindi devo attivare l'altro figlio di cui adesso ho pid, e bloccarmi
		if(kill(otherChild,SIGUSR2)<0){
			perror("\n");
			exit(1);		
		}
		if(ret==0)
			break; //perche' se ritorna dalla read leggendo 0, vuol dire che, essedo questa una pipe, il padre ha chiuso l'estremita' in scrittura, quindi ho terminato pero' devo sbloccare l'altro figlio perche' se questo controllo lo faccio dopo la read, a lui lo lascio in busy wait, e non potra' arrivare alla read sulla quale accorgersi che l'end in scrittura e' stata chiusa dal padre. Quindi si avra' un processo bloccato, ed il padre, che deve attendere terminazione entrambi i processi figli per terminare, rimarrebbe bloccato anch'egli!
		miaLock=CHIUSA; //mi blocco quindi all'inizio del ciclo succ
			
		
	}
}

else if(i==1){
	//secondo figlio
	close(fdPipe[1]);
	while(startLock==CHIUSA); //busy wait i attesa dellos tart del padre etc...
	while(1){
		while(miaLock==CHIUSA); 
		//INIZIO CS<----------
		if((ret=read(fdPipe[0],&t,sizeof(int)))<0){ 
			perror("\n");
			exit(1);		
		}
		
		 
		
		
		printf("\nSONO FIGLIO 1 LETTO %d",t); fflush(stdout);
		//FINE CS<------

		if(kill(childpid[0],SIGUSR2)<0){
			perror("\n");
			exit(1);		
		}
		if(ret==0)
			break;
		miaLock=CHIUSA; //mi blocco quindi all'inizio del ciclo succ
			
		
	}
}	

}


void handler(int sign){

if(sign==SIGUSR1){
//allora il processo che l'ha ricevuto, deve sbloccare la variabile di lock dello start
	startLock=APERTA;
}

else if(sign==SIGUSR2){
	miaLock=APERTA;

}
return;


}
