//NB MANDANDO IN ESECUZIONE QUESTO PROGRAMMA, CI SI ACCORGE CHE IL TERZO PROCESS ORIESCE SI' AD ACCORGERSI CHE E' STATA SUPERATA LA SOGLIA, MA NON RIESCE A FARLO APPENA VIENE SUPERATA. POICHE' NON E' DETTO CHE SE 3 THREAD STANNO IN ATTESA SU UN MUTEX, E UNO DI QUESTI USA QUEL MUTEX ASSOCIATO AD UNA VAR DI CONDIZIONE, E QUINDI USA ANCHE PRIMITIVE WAIT, NON E' DETTO CHE SIA PRECISO L'ORDINE IN CUI LO SCHEDULER DEL SISTEMA OPERATIVO L IFA ANDARE. QUINDI POTREBBE CAPITARE CHE VENGANO ESEGUITI PIU' VOLTE I 2 PROCESSI RISPETTO AL TERZO, NEL TEMPO CHE CI METTE IL TERZO A RITORNARE DALLA WAIT OTTENENDO DI NUOVO L'ACCESSO AL LOCK



//questo e' l'es 2 che sta sul libro, versione con testo corretto
//si creano 2 processi, ognuno dei quali genera SEMPRE 500 e 500 numeri, che si vanno a sommare in una variabile condivisa
//di somma, fino a che pero' il valore della variabile condivisa di somma non supera una soglia che passiamo da linea di comando. Il controllo di quando la variabile condivisa supera il valore di soglia, e' assegnato al terzo thread
//Usare variabili di condizione



/*INNANZITUTTO C'E' UN PROBLEMA DI RACE CONDITION TRA I 2 THREAD CHE DEVONO SCRIVERE SULLA VAR CONDIVISA DI SOMMA
//POI C'E' UN PROBLEMA DI SEGNALAZIONE CHE DOBBIAMO APPROCCIARE USANDO VAR DI CONDIZIONE, TRA QUELLO CHE FANNO I 2 PROCESSI (SOMMA) E IL MOMENTO DI SEGNALAZIONE AL PROCESSO TERZO CHE DEVE ACCORGERSI DI QUANDO LA SOMMA SUPERA UNA DETERMINATA SOGLIA, E NO NSI VUOLE CHE QUESTA SEGNALAZIONE SIA APPROCCIATO CON UN DOPPIO POLLING DA PARTE DEL PROCESSO. QUINDI SI USA UNA VARIABILE DI CONDIZIONE, IL CUI PREDICATO SARA' VALUTATO PROPRIO SULLA VAR DI SOMMA CONDIVISA TRA I PRIMI 2 PROCESSI SOMMATORI. QUINDI IL MUTEX ASSOCIATO ALLA VAR DI CONDIZIONE USATA, PUO' ESSERE LO STESSO CHE USIAMO PER GARANTIRE LA MUTUA ESCLUSIONE TRA I PRIMI 2 PROCESSI SOMMATORI
SICCOME NON VOGLIAMO CHE SIA UNO DEI PRIMI 2 PROCESSI AD ACCORGERSI CHE LA SOGLIA E' STATA SUPERATA, MA SIA IL TERZ OPROCESSO, CHE PERO' NON E' IN DOPPI OPOLLING, MA IN WAIT SULLA VAR DI CONDIZIONE QUANDO LA SOGLIA NON E' SUPERATA, FACCIAMO CHE OGNUNO DEI 2 PROCESSI, DOPO AVER AVUTO ACCESSO MUTUAMENTE ESCLUSIVO ALLA VARIABILE DI SOMMA TOT, SBLOCCHI IL MUTEX E SEGNALI IL TERZO PROCESSO, CHE CONTROLLERA' CHE NON SIA STATA SUPERATA LA SOGLIA, E IN QUESTO CASO SI RIADDORMENTA
(IN REALTA' QUESTO E' COMUNQUE UN DOPPIO POLLING ANCHE SE CONCETTUALMENTE E' PIU' EFFICIENTE PERCHE' COMUNQUE E' COMPITO DEL TERZO PROCESSO VALUTARE IL SUPERAMENTO DELLA SOGLIA. SE COSI' NON FOSSE STATO AVREBBE AVUTO PIU' SENSO USARE LA VAR DI CONDIZIONE, MA SICCOME E' CHIESTO ESPLICITAMENTE DAL TESTO...)
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

//var globali
pthread_mutex_t mutex1=PTHREAD_MUTEX_INITIALIZER; //questo mutex uqindi evita sia la race condition nel prob di sincronizzazione tra i primi 2 processi nella somma, sia la race condition nella valutazione della variabile del predicato della var di condizione da parte del terzo processo (in uanto la variabile del predicato e' la stessa sulla quale fanno race condition i primi 2 thread)
pthread_cond_t cond1=PTHREAD_COND_INITIALIZER;
int sumTot=0;

void * start_primiduethread(void * arg); //come argomento ai primi 2 thread non passiamo nulla, mentre al terzo la soglia che deve controllare per sumTot
void * start_terzothread(void * arg);

int main(int argc,char *argv[]){
int maxSum;
if(argc!=2){
	printf("\nUso: max sum\n"); 
	exit(1);
}
sscanf(argv[1],"%d",&maxSum);



pthread_t tid[3];
int ret,i;

//creiamo i 3 thread
if((ret=pthread_create(tid+2,NULL,start_terzothread,(void *)&maxSum))!=0){
	printf("\nERRORE: %s",strerror(ret));
	exit(1);

}
for(i=0;i<2;i++)
	
	if((ret=pthread_create(tid+i,NULL,start_primiduethread,NULL))!=0){
		printf("ERRORE: %s",strerror(ret));
		exit(1);	
	}



//attendo la terminazione del solo terzo thread, in modo tale che quando il terzo thread termina, e' perche' la soglia e' stata superata, quindi io main thread continuo, arrivo alla return nel main che equivale ad una exit(0), quindi ammazzo il processo e termineranno anche gli altri 2 thread!

if((ret=pthread_join(tid[2],NULL))!=0){ //non ci interessa cosa ritornino dal loro stack dedicato i thread
	printf("%s",strerror(ret));
	exit(1);	
}

exit(0); //terminazione main thread, e quindi processo
}



void * start_primiduethread(void * arg){
//500 e 500 volte...per semplicita' faccio che sommano 1 invece di numero randomico, alla somma tot
int i;
for(i=0;i<500;i++){
	pthread_mutex_lock(&mutex1);
	//printf("\nSono thread %u, aggiungo 1\n",(unsigned int)pthread_self());
	//fflush(stdout);
	sumTot++;
	pthread_cond_signal(&cond1);	
	pthread_mutex_unlock(&mutex1); //sblocco la var che e' sia mutex per l'altro thread, sia mutex sulla var del predicato della variabile di condizione
	//sveglio thread terzo che sta bloccato sulla var di condizione
	
}

}



void * start_terzothread(void * arg) {

int maxSum=*((int *)arg); //soglia
	printf("\nSono terzo: valuto somma: la soglia e' %d e per ora siamo a %d\n",maxSum,sumTot);
	fflush(stdout);

pthread_mutex_lock(&mutex1); 
while(sumTot<maxSum){
	pthread_cond_wait(&cond1,&mutex1);
	printf("\nSono terzo: valuto somma: la soglia e' %d e per ora siamo a %d\n",maxSum,sumTot);
	fflush(stdout);
}
//finche' non e' superata la soglia, torna a dormire: la wait sblocca automaticamente il mutex1 dopo aver bloccato il thread chiamante. Quando ritornera', sara' perche' avremo ricevuto signal da uno dei 2 thread sommatori che ci stara' chiedendo di valutare la soglia. La wait in questo caso ritornera' (ma appena anche il mutex sara' riacquisibile..cioe' risultera' libero, e la wait lo riblocca, risveglia il Thread 3 e ritorna. 
printf("\nLa somma totale ha raggiunto la soglia di %d su %d\n",sumTot,maxSum);
fflush(stdout);
//non gli risblocco il mutex altrimenti potrebbero proseguire con la somma nel tempo che passa tra il return del terzo thread e la terminazione del main thread e quindi del processo
pthread_exit(NULL);



}
