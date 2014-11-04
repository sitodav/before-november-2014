//programma che fa 3 figli, lancia sigint ai figli, questi grabbano il segnale con un handler esplicito
//che prima stampa che hanno ricevuto segnale e poi escono, quindi inviano il segnale di SIGCHLD al padre
//e il padre grabba il segnale di sigchld per ciascuno dei figli e lancia un suo handler che prima
//stampa che ha ricevuto segnale e poi lancia la wait per toglierli dallo stato di zombie

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void sigChldHandler(int signo);
void sigIntHandler(int signo);

int main(int argc,char *argv[]){
int childpid[3]={0,0,0},i=0;
int padre=1; //var booleana

while(i<3){
	childpid[i]=fork(); //genero un figlio che metto in childpid[i]
	
	if(childpid[i]==0) //allora sono figlio, e devo uscire, altrimenti farei anche io figli
		break; //nota che il secondo figlio avra' si' childpid[1]==0, ma childpid in indice precedente uguale al pid
			//del figlio fatto al ciclo precedente dal suo padre
	
	else
		i++; //altrimenti sono il padre, quindi incremento la i
}
//qui arrivano direttamente i figli, e il padre dopo che ha fatto i 3 figli
//fai attenzione qui a controllare la disposizione dell'array di childpid 
//perche' il padre e' l'unico dei 4 processi che non ha neanche un elemento array vuoto
//mentre i figli hanno l'elemento di indice precedente a quello in cui sono stati creati loro, riempito col pid
//del figlio precedente. Questo e' un problema dell'usare l'array di pid invece di singole variabili.
//Tuttavia l'array di pid ci serve perche' il padre dovra' lanciare sigint a tutti i suoi figli (quindi deve sapere i loro pid)

for(i=0;i<3;i++)
	if(childpid[i]==0){ //allora ho almeno 1 elemento vuoto sull'array, quindi sono un figlio
		printf("\nIo sono un figlio\n");
		padre=0;
		break;
	}		


//solo il processo che non ha neanche un elemento 0esimo sull'array, e' il padre, e solo lui uscira' da questo ciclo
//con la var booleana padre lasciata impostata su 1
//quindi

if(!padre){
	//se sono uno dei figli, installo l'handler per il sigint che mi mandera' il padre
	if(signal(SIGINT,sigIntHandler)==SIG_ERR){
		perror("\n"); exit(1);	
	}
	//sono il figlio, quindi ho l'handler installato, e mi metto in ciclo infinito fino a quando 
	//non ricevo sigint dal padre, che grabbero lanciando esplicitamente il mio handler, che mi fara' terminare andando
	//in zombie
	pause(); //bloccati finche' non sei interrotto da un signal (essend ofiglio l'interruzione sara' il sigint de lpadre)
}

else{ //sono il padre dei 3 processi allora
	if(signal(SIGCHLD,sigChldHandler)==SIG_ERR){ //registro l'handler per il sigchld
		perror("\n"); exit(1); 	
	}
	//aspetto un paio di secondi e poi lancio il sigint ai figli
	sleep(4);
	for(i=0;i<3;i++){
		printf("\nLancio SIGINT al mio figlio con pid %d\n",childpid[i]);
		if(kill(childpid[i],SIGINT)<0)
			perror("\n");
	}
	//dopo averlo lanciato, mi blocco su un while infinito, i miei figli grabbano sigint col loro handler per i lsignint, stampano, escono, mi arriva quindi
	//un sigchld che grabbo col mio handler dove lancio wait e da li' mi termino e stesso da li' aspetto la terminazione
	//di tutti i figli, cosi' alla fine dell'handler non avro' piu' figli e potro' terminare
	pause(); //bloccati finche' non sei interrotto da un signal (essendo i lpadre, il signal sara' il SIGCHLD che ricevo per la terminazioen del figlio)
	
}






return 0;
}

void sigChldHandler(int signo){
int exitStatusChild,i;
	printf("\nUn mio figlio e' terminato: mi metto in wait per lui e tutti gli altri\n");
	fflush(stdout);
	for(i=0;i<3;i++)
		wait(&exitStatusChild);
	//potrei implementare un controllo sullo stato di exit del figlio etc...ma non lo faccio perche' mi scoccio
	exit(0);
	
}

void sigIntHandler(int signo){
	printf("\nSono figlio con pid %d-> Ho ricevuto un sigInt\nTermino (e rimango zombie)\n",getpid());
	fflush(stdout);
	exit(0);

}
