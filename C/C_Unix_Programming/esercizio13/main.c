#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

int childpid[2],pipe1[2],pipe2[2],pipe3[2],n,tot,nread,nwrite,i,parz=0;

void siginthandler(int sign);
int main(int argc,char *argv[]){
int ret;



//installo il siginthandler che servira' solo sui figli
if(signal(SIGINT,siginthandler)==SIG_ERR){
	perror("\n"); exit(1);
}

//creo le 3
pipe(pipe1); 
pipe(pipe2);
pipe(pipe3);
//e forko i 2 figli
for(i=0;i<2;i++){
	childpid[i]=fork();
	if(childpid[i]==0) 
		break;
}
//qui arrivera' il primo figlio con i==0, il secondo con i==1, e il padre dopo averli fatti
//con i==2

if(i==2){ //inizio codice padre
	//chiudiamo le estremita' delle pipe anonime, che non ci servono
	close(pipe1[0]); close(pipe2[1]); close(pipe3[0]);
	while(1){
		printf("\nIN FATHER: inserisci numero\n"); fflush(stdout);
		scanf("%d",&n);	while(getchar()!='\n');
		if(n>=0 && n%2==0){ //inserito npari e positivo
			printf("\nHai inserito numero pari\n"); fflush(stdout);
			//e lo mandiamo al primo figlio usando la pipe1 
			ret=write(pipe1[1],&n,sizeof(n));
			if(ret<0){ //controllo errore su write
				perror("\n"); exit(1);
			}
				
			
		}
		else if(n>=0 && n%2!=0){ //num dispari e positivo
			printf("\nHai inserito numero dispari\n"); fflush(stdout);
			//e lo mandiamo al secondo figlio usando pipe3
			ret=write(pipe3[1],&n,sizeof(n));
			if(ret<0){ //controllo errore su write
				perror("\n"); exit(1);
			}
		}
		else { //numero negativo
			//dobbiamo inviare il segnale ai figli
			for(i=0;i<2;i++)			
				if(kill(childpid[i],SIGINT)<0){
					perror("\n"); exit(1);			
				}
			//dobbiamo quindi aspettare di ricevere i parziali da entrambi i figli
			
			tot=0;
			for(i=0;i<2;i++){ //2 volte fa una read, che ritorna sicuramente quando 					//legge i 2 numeri ogni volta e li somma al tot
				ret=read(pipe2[0],&n,sizeof(n));
				printf("\nRicevuto parziale dal figlio %d : %d",i,n); fflush(stdout);				
				tot+=n;
				
			}
			printf("\nIl totale delle somme e' %d\n",tot); fflush(stdout);
				
			
			
		
		}
		
	}


} //fine codice padre



else if(i==0){ //inizio codice primo figlio
	while(1){ //anche i figli non terminano mai
		//aspettiamo di ricevere il numero
		ret=read(pipe1[0],&n,sizeof(n));
		if(ret<0){ //controllo errore in lettura
			perror("\n"); exit(1);		
		}
		printf("\nIN CHILD %d: RICEVUTO NUMERO %d",i,n); fflush(stdout);
		parz+=n;
		//il figlio non uscira' mai dal while, ma quando ricevono sigint, parte l'handler dal quale mandano i parziali al padre, li azzerano e si ricomincia da capo
	
	}


}// fine codice primo figlio


else if(i==1){//inizio codice secondo figlio
	while(1){	
		ret=read(pipe3[0],&n,sizeof(n));
		if(ret<0){ //controllo errore in lettura
			perror("\n"); exit(1);		
		}
		printf("\nIN CHILD %d: RICEVUTO NUMERO %d",i,n); fflush(stdout);
		parz+=n;
		//il figlio non uscira' mai dal while, ma quando ricevono sigint, parte l'handler dal quale mandano i parziali al padre, li azzerano e si ricomincia da capo
	}
		
}//fine codice secondo figlio









return 0;
}



void siginthandler(int sign){
int ret;
printf("\nSono figlio %d, ricevuto sigint, mando parziale di %d!\n",i,parz);
fflush(stdout); 
//ricevuto sigint i figli non terminano, ma mandano soltanto i parziali al padre
//li mandano usando entrambi la pipe2
ret=write(pipe2[1],&parz,sizeof(parz));
if(ret<0){ //controllo errore sulla write
	perror("\n"); exit(1);
}

// e dopo averli mandati li azzeriamo per ricominciare da capo con valori nuovi
parz=0;



}
