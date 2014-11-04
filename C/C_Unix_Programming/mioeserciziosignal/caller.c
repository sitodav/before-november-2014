/*esercizio dove caller.c fa n figli (n passato da cmd line)
//ogni figlio diventa called tramite una exec, e ad ogni exec e' passato da cmd line lista arg
//della exec array con pid degli n-lui figli avanti a lui.
//ogni called riceve i pid dei figli spawnati prima di lui, ed un SIGUSR dal called precedente:
//nell'handler di SIGUSR del called si comporta in questo modo:
//se ci sono altri called davanti a lui, manda SIGUSR al successivo,altrimenti se sei l'ultimo
//manda SIGUSR al padre, che lo gestira' col suo handler personale per SIGUSR
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>


void sigUsr1Handler(int sig);

int main(int argc,char *argv[]){
if(argc!=2){
	printf("\n uso : <this> <n>\n");
	exit(0);
}

//installiamo l'handler per SIGUSR1
if(signal(SIGUSR1,sigUsr1Handler)==SIG_ERR){
	perror("\n");
	exit(1);

}


int n,*childpid,i,j;
char **new_argv;
sscanf(argv[1],"%d",&n);
childpid=calloc(n,sizeof(int));
new_argv=calloc(n+1,sizeof(char *)); //al massimo l'array da passare alla execv che andiamo a costruire sara' grande n+1 dove: n-1 contengono i pid dei figli (l'ultimo figlio spawnato che 
//chiama l'exec avra' proprio tutti n-1 pid, quindi manca solo il suo) + 1 perche' argv[0] e' il nome del programma called, e l'ultimo che e' il NULL terminatore dell'array di stringhe

//spawno gli n figli con fork, e ogni figlio diventera' tramite exec
//un called con nella lista arg i pid dei figli spawnati prima (che avra' anche lui nell'array
//in quanto copia del padre, avra' il suo stesso array fino a quel momento meno il suo stesso pid perche' la fork() gli ritorna 0)
//da notare che l'unico figlio che ha i pid di tutti gli altri figli, e' l'ultimo spawnato (l'n-esimo) quindi tecnicamente della catena di n figli, quello che manda SIGUSR al padre e' il primo
//in ordine cronologico, ad essere stato spawnato, perche' e' lui che con la exec diventa un
//called con in lista arg solo il padre

for(i=0;i<n;i++){
	childpid[i]=fork();
	if(fork<0){
		perror("\n");
		exit(1);
	}
	if(childpid[i]!=0){ //sono il padre
		printf("\nSono padre e ho forkato proc con pid %d\n",childpid[i]);
		fflush(stdout);		
		continue; //riavvia il for allora
	}
	else{ //sono il figlio iesimo degli n totali da fare
		//devo creare un array di pid da passare al programma called che diventero' con 		la exec (i pid che per me figlio sono disponibili nell'array)
		//creiamo l'array da passare (argv0 nome prog..lista pid disponib... NULL)
		new_argv[0]=calloc(20,sizeof(char)); //qui costruiamo l'elemento 0esimo di argv
		strcpy(new_argv[0],"called");
		j=0;
		while(childpid[j]!=0){ //e qui, finche' nell'array dei child spawnati sono presenti pid dei figli spawnati precedentemente, alloca dinamicamente uno dei punt a stringa di new_argv e copiaci la conversione a stringa del pid disponibile sull'array 
		 new_argv[j+1]=calloc(20,sizeof(char));
		 sprintf(new_argv[j+1],"%s",childpid[j]);
		 j++;		
		}
		//e nell'ultima posizione non riempita di new_argv (che e' j+1 per come e' stato lasciato j all'uscita dal while) metti il terminatore NULL
		argv[j+1]=NULL;
		
		execv("called",new_argv); //quindi diventiamo il programma called che avra in input da cmd line in argv, tutti i pid dei figli spawnati prima di lui (lui escluso)
		printf("\nExec fallita\n"); fflush(stdout);
		exit(1); //qui non ci deve arrivare se la exec va a buon fine
	}
}

//qui all'uscita del for arrivera' solo il padre, perche' i figli saranno diventati 
//called

//siccome i called, si muovono soltanto all'interno dei loro handler per SIGUSR1, mandandosi
//ogni volta che ricevono un SIGUSR1, come comportamento dell'handler, un SIGUSR1 al called successivo, il padre quindi deve avviare la catena degli eventi.
//E siccome vogliamo che il caso di partenza sia il called che possiede i pid di tutti i figli, fino ad arrivare "ricorsivamente" al caso base che e' il called spawnato per primo (quello che quando era uscito dalla fork() nona veva pid di nessun figlio precedente) che non contiene pid
//in argv[1], allora il SIGUSR1 lo mandiamo al figlio (che e' diventato called con exec) forkato per ultimo
if(kill(childpid[n-1],SIGUSR1)<0){
	perror("\n");
	exit(1);
}



//il padre ora non deve fare niente, ma dovra' solo attendere di ricevere SIGUSR
//dall'ultimo called della lista (che in relta' e' il primo generato in ordine cronologico)

while(1); //l'handler per sigusr1 lo gestisce e poi termina il programma
//mentre se arriva qualunque altro segnale, no (boh, dipende dall'implementazione di default)



return 0;
}



void sigUsr1Handler(int sig){

printf("\nSONO IL PADRE: RICEVUTO SIGUSR1 DA CATENA!!!\n");
exit(0);




}

