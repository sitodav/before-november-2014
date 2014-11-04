/*4. 9 punti (PER CFU 12 e 10)
Si scriva un programma C e Posix in ambiente Linux che abbia un’interfaccia del tipo: esame Nfigli
Nlivello FileRisultati dove Nfigli e Nlivello sono due numeri interi, e FileRisultati è una stringa che
rappresenta il nome di un file. Il programma provvede a generare un numero Nfigli di processi figlio.
Ogni processo figlio scrive il proprio pid all'interno del file FileRisultati e inoltre genera a sua volta un
altro processo che si comporterà come ognuno dei processi figlio. Il numero intero Nlivello rappresenta
il livello di profondità della gerarchia di processi da creare. Ad esempio, nel caso in cui Nlivello sia 2, il
processo iniziale genererà il numero di figli (livello 1) richiesto e ognuno dei figli, dopo aver scritto il
proprio pid nel file, metterà in esecuzione un processo nipote (livello 2) che si limiterà alla scrittura del
proprio pid sul file, senza generare altri processi. Al termine, il processo iniziale deve visualizzare sulla
console il contenuto del file.
*/


//ci potrebbe essere un problema di sincronizzazione sulla scrittura al file per il quale i processi figli condividono l'offset, ma per semplicita' non lo consideriamo

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

int main(int argc,char *argv[]){
if(argc!=4){
	printf("\nUso: nfigliperliv nmaxlivell file.txt\n");
	exit(1);
}

int figliPerLiv,maxLiv;
sscanf(argv[1],"%d",&figliPerLiv);
sscanf(argv[2],"%d",&maxLiv);
printf("%d figli per livello !\n",figliPerLiv);
fflush(stdout);
int *childpid=calloc(figliPerLiv,sizeof(char));

int fdFileW;

if((fdFileW=open(argv[3],O_WRONLY | O_CREAT, 00777))<0){
	perror("\n");
	exit(1);
}

int actualLvl=0,i;

while(1){
	if(actualLvl==maxLiv){
		printf("\nRaggiunto livello massimo\n");
		fflush(stdout);	
		break;
	}

	for(i=0;i<figliPerLiv;i++){
		printf("\nSono a liv %d e creo figlio\n",actualLvl);
		fflush(stdout);
		childpid[i]=fork();
		if(childpid[i]==0)
			break;		
	}
	if(i==figliPerLiv) //padre
		break;
	//altrimenti sono figlio, incremento livello e ricomincio ciclo
	actualLvl++;
	
}

//tutti quanti scrivono il loro pid ed il loro livello nel file
char toWrite[50];
sprintf(toWrite,"mio pid:%d, figlio di %d, mio liv:%d\n",getpid(),getppid(),actualLvl);
if(write(fdFileW,toWrite,strlen(toWrite))<0){
	perror("\n");
	exit(1);
}

if(actualLvl==0){
//padre
	printf("\nSono padre, e attendo i figli\n");
	for(i=0;i<figliPerLiv;i++)
		waitpid(childpid[i],NULL);
	printf("\nTutti i miei figli sono terminati\n");
	fflush(stdout);
			

}

}





