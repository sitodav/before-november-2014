/*Realizzare un programma in C e Posix sotto Linux che realizzi una struttura di processi ad albero
ternario, tale che ogni processo si metta in attesa che i suoi figli terminino. Ogni figlio termina dopo aver
atteso per un numero di secondi pari al livello dell'albero al quale si trova, allo scadere del quale stampa
a schermo il proprio pid diviso 100 ed elevato alla i-ma potenza, dove i è il livello dell'albero a cui il
processo figlio si trova e comunica al genitore la sua terminazione.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>
#include <math.h>



int main(int argc,char **argv){

if(argc!=2){
	printf("\nIns max level\n"); 
	exit(0);

}



int maxLvl,i,actualLvl,childpid[3];
sscanf(argv[1],"%d",&maxLvl);

actualLvl=0;
while(actualLvl<maxLvl){
	for(i=0;i<3;i++){
		childpid[i]=fork();
		if(childpid[i]==0)
			break;	//1 dei 3 figli
	}
	if(i==3) //padre che ha fatto i 3 figli
		break;
	else actualLvl++;
		
}

if(actualLvl!=maxLvl)
	for(i=0;i<3;i++)
		waitpid(childpid[i],NULL,0);
sleep(actualLvl);
double t=(double)(getpid()/100);


printf("\nSono processo livello %d, pid %d",actualLvl,getpid());
fflush(stdout);









}
