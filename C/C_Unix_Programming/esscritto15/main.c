/*
4. 9 punti(PER CFU 12 e 10)
Scrivere un programma in C e Posix sotto Linu x che, preso un arg omento int ero pos it ivo da riga di
comando, gestisca la seguente situazione:
genera due figli A e B e
- se l'argomento è PARI invia un segnale SIGUSRl alla ricezione del quale il figlio A calcola il fattoriale
del numero passato come argomento da linea di comando, mentre il figlio B st am pa un messagg io di
arrivederci e termina.
_ se l'argomento è DISPARI invia un segnal e SIGUSR2 alla ri cezion e del qu ale il fig lio B calcola la
sequenza di Fibonacci di ordine pari al numero passato come argomento, attende per un numero di
second i pari al triplo del numero passato come argome nto ed invia un segnale SIGUSRl al .processo A
dopodiché term ina l'esecuzion e. Il figlio A, invece, attende la ri cezione del segnale SIGU SR1, stamp a un
messaggio e termina .
*/


#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#define CHIUSA 1
#define APERTA 0

int lock=CHIUSA;


void handler(int sign);
int main(int argc,char *argv[]){

if(argc!=2){
	printf("\nN\n");
	exit(0);
}

int n;
sscanf(argv[1],"%d",&n);


if(signal(SIGUSR1,handler)==SIG_ERR){
	perror("\n");
	exit(1);
}

if(signal(SIGUSR2,handler)==SIG_ERR){
	perror("\n");
	exit(1);
}

int childpid[2],i;
for(i=0;i<2;i++){
	childpid[i]=fork();
	if(childpid[i]==0)
		break;
}

if(i==0){
//primo figlio A


	while(lock==CHIUSA); //busy wait, si poteva fare anche con una pause, ma sarebbe potuta essere interrotta anche da altri segnali a meno che di usare la sigprocmask, e cmq la pause crea problema di sincr riguardo la velocita' di arrivo blabla

	//all'uscita della busy wait
	if((n%2)==0){
		int fatt=1;
		for(i=2;i<=n;i++)
			fatt*=i;
		printf("\nSono il figlio A: il fattoriale di %d e' %d\n",n,fatt); fflush(stdout);
		exit(0);
	
	}
	else {
		printf("\nSono il figlio A:Stampo un messaggio e termino\n"); 	
		fflush(stdout);
		exit(0);
	}
	

}

else if(i==1){
//secondo figlio B
	if((n%2)==0){ //termina perche' tanto non ricevera 'nulla dal padre
		printf("\nSono B. Termino\n");
		fflush(stdout);
		exit(0);
	}
	//altrimenti si mette in attesa di SIGUSR2
	while(lock==CHIUSA);
	
	int fib1=0,fib2=1,last;
	last=fib1+fib2;
	for(i=2;i<=n;i++){
		fib1=fib2;
		fib2=last;
		last=fib1+fib2;
	}
	printf("\nSono figlio B: il valore di fibonacci e' %d\n",last);
	sleep(3*n);
	if(kill(childpid[0],SIGUSR1)<0){
		perror("\n");
		exit(1);	
	}
	printf("\nSono figlio B, mandato segnale ad A, termino\n"); fflush(stdout);
	exit(0);
	

}

else{
//padre
	if((n%2)==0){
		printf("\nNumero pari\n");
		fflush(stdout);
		if(kill(childpid[0],SIGUSR1)<0){
			perror("\n");
			exit(1);		
		}	
	}
	else{
		printf("\nNumero dispari\n");
		fflush(stdout);
		if(kill(childpid[1],SIGUSR2)<0){
			perror("\n");
			exit(1);
		}	
	}
}


return 0;
}




void handler(int sign){
printf("\nSbloccato\n");
lock=APERTA;

}
