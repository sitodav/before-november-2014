#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>


int main(int argc,char *argv[]){
char filename[FILENAME_MAX],toPrint[100],toRead[100];
int fdFile,childpid,actualLevel,nNodiPerLevel,maxLevel; //il processo radice e' considerato livello 0 e anche lui scrive il suo pid nel file
int i,nread;

if(argc!=4){
	printf("\nuso: <nome_eseguibile> <n figli> <lvl max> <path file>\n");
	exit(1);
}

strcpy(filename,argv[3]);
nNodiPerLevel=atoi(argv[1]);
maxLevel=atoi(argv[2]);

childpid=0;
actualLevel=0;

//il file lo apriamo prima delle fork, perche' tanto gli open file vengono ereditati da tutti le copie-figlio
if((fdFile=open(filename,O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IXUSR))<0){
	perror("\n");
	exit(1);
}



while(1){ //la prima cosa che fa qualunque processo inizio un ciclo di while, e' scrivere le proprie info sul file
	sprintf(toPrint,"sono proc con pid %d, figlio di %d, il mio liv: %d\n",getpid(),getppid(),actualLevel);
	write(fdFile,toPrint,strlen(toPrint));

	if(actualLevel==maxLevel) break; //break se i processi attuali sono dell'ultimo liv
	
	do{childpid=fork();  //in questo modo solo 1 proc istanzia n figli (con un semplice 					for sarebbero stati generati 2^n figli per processo che arriva 					nel for, e questo non va bene) 		
	   i++;
	}while(childpid!=0 && i<nNodiPerLevel);

	//gli n figli arrivano qui direttamente, il padre ci arriva dopo aver fatto i suoi n 		figli

	if(childpid==0) {//ognuno degli n figli torna all'inizio del while
		actualLevel++; 
		i=0;		
		continue;
	}
	else if(childpid>0) //invece il padre ha fatto il suo lavoro e breakka		
		break;
			
	
	else exit(0); //perche' fork non e' andata a buon termine
}

//qui arriveranno tutti i processi padri di n processi del liv successivo, e i processi finali
//che non dovranno avere figli

close(fdFile); //tutti i processi chiudono il file

//ora vogliamo che solo il processo radice (actualLevel==0) stampi il contenuto del file
//quindi solo lui riapre il file in lettura e stampa il contenuto
if(actualLevel==0){
	//prima di riaprire il file e leggerlo, andrebbe fatta la wait su tutti i figli, per aspettare che abbiano finito, e non stampare un valore non coerente del file
	//per semplicita' usiamo una sleep
	sleep(15);
	if((fdFile=open(filename,O_RDONLY))<0)
		perror("\n");
	else
		while((nread=read(fdFile,toRead,99))>0);
			printf("%s",toRead);
		
}
//per evitare che scrivendo le proprie info nel file, i figli scrivano come pid del padre quello
//di init() perche' al momento del getppid() il padre e' terminato gia' prima di loro,
//facciamo che ogni processo prima di terminare aspetta gli n figli, lanciando n volte wait


for(i=0;i<nNodiPerLevel;i++)
	wait(NULL);

return 0;
}
