//limitante prende da riga di comando il tempo da concedere al programma chiamato.
//In questo caso il programma chiamato se e' limitato, stampa a video finche' non e' interrotto da limitante
//il limitante gestisce il SIGCHLD dal figlio terminato, con un signal handler che lancia una wait per togliere il figlio
//da stato zombie

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <signal.h>



void sigChldHandler(int sign);
int main(int argc,char *argv[]){

if(argc<3){
	printf("\nuso:<this> <timelimitSEC> <cmdToExecute> (timeToExecute)\n");
	exit(1);
}

int childpid,ret;
struct timespec timeToWait,timeRimanente;




childpid=fork();
if(childpid==0){ //sono figlio
	execvp(argv[2],&argv[2]);
	printf("\nExec per %s fallita\n",argv[2]);
}

//installo il signal handler per SIGCHLD
if(signal(SIGCHLD,sigChldHandler)==SIG_ERR){
	perror("\n"); exit(1);
}

//il professore utilizza la sleep, ma non e' proprio corretto perche' se il padre ricevesse un segnale mentre e' in attesa,
//ritornerebbe dalla sleep, ed eventualmente dopo aver gestito il segnale, continuerebbe subito prima magari che siano passati i timeLimit secondi totali da attendere, e subito lancerebbe sigint al figlio, che quindi si vedrebbe bloccato prima
//Usiamo una  funzione che ci permette di attendere n secondi , con n secondi memorizzati in una struct, in modo tale che se
//la funzione di attesa e' interrotta, nel caso in cui siano rimasti n-x secondi non attesi nella struct, riprende l'attesa
//potremmo usare la select, ma non e' sicura per quanto riguarda il tempo rimasto quanto la nanosleep:

sscanf(argv[1],"%d",&(timeToWait.tv_sec));//porto nella struct il tempo da attendere
timeRimanente.tv_sec=0; //azzero la struct che dovra' contenere il tempo eventualmente non atteso
timeRimanente.tv_nsec=0;

if(kill(0,childpid)==-1){ //usiamo la kill con segnale nullo per controllare che il figlio non sia gia' terminato prima ancora d iiniziare il conto alla rovescia con la nanosleep. In tal caso se il figlio fosse gia' terminato, avremmo anche gia' grabbato il suo SIGCHLD prima ancora di entrare in questo if e avremo gia' fatto la wait (tutto questo perche' il signhandler lo abbiamo installato piu' sopra)
	printf("\nIl mio figlio e' gia' terminato prima ancora che lanciassi il timer! Termino direttamente anche io\n\n");
	fflush(stdout);
	exit(0);
}

while(1){
	ret=nanosleep(&timeToWait,&timeRimanente);
	if(ret<0 && errno==EINTR) //allora dovra' essere riavviato il ciclo, ma mettendo nella struct del tempo da
		timeToWait=timeRimanente; //attendere, quello rimasto nella timeRimanente xke' non era stato tutto atteso
	else break;	
}

printf("\nSono passati %s secondi: E' ORA DI UCCIDERE MIO FIGLIO !!\n",argv[1]); fflush(stdout);

//scaduto il tempo quindi interrompiamo il figlio
if(kill(childpid,SIGINT)<0){
	perror("\n"); 
	exit(1);
}

//siccome nel padre c'e' installato un signal handler per il segnale SIGCHLD ricevuto quando un figlio termina
//sicuramente ci arrivera' quel segnale dopo aver ucciso il figlio (sopra) quindi non terminiamo ancora ,ma ci mettiamo
//in attesa di un segnale con la pause()

pause(); //verra' interrotta appena viene ricevuto il SIGCHLD che verra' anche grabbato e gestito dal nostro sigChldHandler



return 0;
}

void sigChldHandler(int sign){
int termStat;
//non ho bisogno di controllare il valore del segnale che ha fatto partire l'handler, perche' tanto ho installato l'handler
//soltanto per il segnale SIGCHLD, quindi solo lui puo' essere
printf("\nRicevuto SIGCHLD : UN MIO FIGLIO E' MORTO E ZOMBIE!\n");
printf("\nLancio wait\n");
wait(&termStat);

if(WIFEXITED(termStat))
	printf("\nExit status figlio : %d",WEXITSTATUS(termStat));
else
	printf("\nTERMINATION STATUS DEL FIGLIO : %d\n",WTERMSIG(&termStat));	





}
