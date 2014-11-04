/*

while(1)
	se sono proc foglia, attendi stampa  e muori
	se non sono proc foglia:
			fai figlio con fork(): se dopo fork sono figlio, vai al passo successivo, altrimenti se sono padre
						fai di nuovo figlio (finche' non raggiungi max 3, in quel caso vai al passo 							successivo anche tu)
	sono figlio ? 
		allora incrementa actual level e ritorna all'inizio del while
	sono quello che era il padre ?
		allora attendi la terminazione dei figli, poi attendi stampa e muori
				


*/







#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#define FIGLIPERLVL 3


int main(int argc,char *argv[],char *envp[]){
int childpid,i,maxLevel,actualLevel;

actualLevel=0;
maxLevel=atoi(argv[1]);

if(argc!=2){
	printf("\nuso : <cmd> <max lvl>\n");
	fflush(stdout);
	exit(1);
}



if(maxLevel==0)
	exit(1);
	

while(1){
	
	if(actualLevel!=maxLevel){
	
		do{	
			if( (childpid=fork())<0 );  //faccio fork controllando l'errore eventuale
			i++;
		} while(childpid!=0 && i<FIGLIPERLVL); //in questo modo solo 1 processo ne spawna 3, mentre i 3 figli
		// saltano direttamente ...
		
		//...qui (e il loro padre ci arriva dopo averli spawnati tutti e 3):
		if(childpid!=0){
			for(i=0;i<FIGLIPERLVL;i++) 
				wait(NULL); //facciamo wait per il numero totale dei figli, e non ci interessano term stat
			printf("\nAttendo %d secondi e poi termino\n",actualLevel);
			sleep(actualLevel);
			printf("\nConcluso");
			break;
		}
		else { //allora sono uno dei 3 figli
			actualLevel++;
			i=0;
			continue;
		
		}
		
	
	}
	else{//i processi che entrano qui e quindi non verificano l'if, sono gli ultimi nodi(foglie) che quindi devono
		//solo stampare e attendere tot secondi e poi morire
		printf("\nAttendo %d sec e poi termino\n",actualLevel);
		sleep(actualLevel);
		printf("\nConcluso");
		break;
			
	}



}








}
