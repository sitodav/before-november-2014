/*1. 9 punti (SOLO PER CFU 10)
Realizzare un programma in C e Posix sotto Linux che realizzi una struttura di processi ad albero binario
tale che ogni processo si metta in attesa che i suoi figli terminino. Ogni figlio termina dopo aver atteso
per un numero di secondi pari al livello dell'albero al quale si trova, allo scadere del quale stampa a
schermo il proprio pid diviso i, dove i è il livello dell'albero a cui il processo figlio si trova, stampa il pid
del genitore e comunica a quest’ultimo la sua terminazione.


*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc,char *argv[]){
int actualLvl,maxLvl,childpid[2],i;

printf("Ins max lvl (pid padre %d)\n",getpid());
scanf("%d",&maxLvl);
actualLvl=0;
while(actualLvl<maxLvl){
	for(i=0;i<2;i++){
		childpid[i]=fork();
		if(childpid[i]==0) break;	
	}
	if(i==2) break;
	else actualLvl++;
}

for(i=0;i<2;i++)
	if(actualLvl==maxLvl)
		break;
	else
		wait(NULL);
sleep(actualLvl);
printf("\nSono processo con pid %d, figlo di %d e sto a liv %d, termino!\n",getpid(),getppid(),actualLvl);
fflush(stdout);
exit(123);





}

