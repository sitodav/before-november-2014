#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void sigUsr1Handler(int sig);
void sigUsr2Handler(int sig);
int childpid[2]; //ci serve globale, perche' l'handler per SIGUSR2 deve mandare messaggio all'altro figlio, quindi
//serve che nell'handler sia visibile il pid del figlio B
int n; //e lo stesso per n

int main(int argc,char *argv[]){

if(argc!=2){
	printf("\nuso: <this> <n>\n");
	exit(0);
}

int id=0; //se 2 e' il padre, se 1 e' il figlio B, se e' 0 e' il figlio A
int i;
sscanf(argv[1],"%d",&n);

if(signal(SIGUSR2,sigUsr2Handler)==SIG_ERR){ 
	perror("\n");
}


if(signal(SIGUSR1,sigUsr1Handler)==SIG_ERR){ 
	perror("\n");
}





for(i=0;i<2;i++){
	childpid[i]=fork();
	if(childpid[i]==0)//allora sono un figlio
		break;
	
}
//tutti e 2 i figli arrivano direttamente qui, mentre il padre ci arriva dopo aver fatto i 2 figli
//ora il padre avra' l'array tutto pieno,mentre il figlio A (il primo spawnato) avra'
//l'array tutto vuoto, mentre il figlio B (il secondo spawnato) avra' il primo elemento in indice 0 pieno (col pid
//del figlio A) e il secondo vuoto
//quindi contiamo quanti elementi non vuoti ci sono nell'array, e in base al valore settiamo l'id numerico per sapere chi siamo se padre, a o b

for(i=0;i<2;i++)
	if(childpid[i]!=0)
		id++;

//e ora continuiamo normalmente a seconda di chi siamo

if(id==2) //ci sono 2 elementi non nulli nell'array childpid quindi sono il padre
{	
	if((n%2)==0){
	 printf("\nNumero pari\n");
	 kill(childpid[0],SIGUSR1); //mando SIGUSR1 al figlio A il cui pid sta in indice 0 di childpid
	}
	 		
	
	else{
	   printf("\nNumero dispari, mando SIGUSR2 a B\n");
	   kill(childpid[1],SIGUSR2); //mando SIGUSR2 al figlio B
	}

	//fatto quello che deve fare, il padre rimane in ciclo continuo perche' l'esercizio non dice nulla riguardo la sua
	//terminazione
        while(1); 

}

else if(id==1) //sono il secondo figlio spawnato, B
{ 
  
  //b attende tot secondi durante i quali se riceve SIGUSR2 allora parte l'handler che fa quello che deve fare, e poi B stampa e termina...
  //altrimenti se non riceve segnali stampa direttamente arrivederci e termina
  printf("\nSono il figlio B, attendo 5 secondi, se non ricevo SIGUSR2 o altri segnali, termino\n");
  sleep(5);
  printf("\nTermino. Ciao\n");
  
}


else if(id==0) //sono il primo figlio spawnato cioe' A
{ printf("\nSONO A CON ID %d\n",id); fflush(stdout); 
	if(signal(SIGUSR1,sigUsr1Handler)==SIG_ERR){ //nel figlio A installiamo l'handler per SIGUSR1
	perror("\n");
  }
  //il figlio A sta sempre in attesa di SIGUSR1, quindi mettiamo un while infinito
  while(1);


}


return 0;
}


void sigUsr1Handler(int sig){
int cubo;
cubo=n*n*n;
printf("\nIl cubo calcolato e' %d\n",cubo);
fflush(stdout);

exit(0);


}

void sigUsr2Handler(int sig){
float reciproco;
reciproco=1/n;
printf("\nIl reciproco e' %f\n",reciproco);
printf("\nAttendo il doppio dell'n passato: %d secondi e poi mando SIGUSR all'altro figlio\n",2*n);
fflush(stdout);
sleep(2*n);
if(kill(childpid[0],SIGUSR1)<0){
	perror("\n");
	exit(1);
}

}

//la terminazione di A avviene grazie all'handler (altrimenti nel main rimarrebbe in attesa all'infinito di SIGUSR1)
//mentre la terminazione di B avviene nel main, e non nell'handler (sia che riceva SIGUSR2 che non lo riceva)


