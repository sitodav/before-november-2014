
/*
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



#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>



int main(int argc,char *argv[]){


if(argc!=4){
	printf("\nuso: <this> nfigli nlivllo file\n");
	exit(0);
}

int nFigli,nLivello,i,*childpid,actualLvl,fdFile,mypid;
char *nomeFile=calloc(strlen(argv[3])+1,sizeof(char));
strcpy(nomeFile,argv[3]);
sscanf(argv[1],"%d",&nFigli);
sscanf(argv[2],"%d",&nLivello);
childpid=calloc(nFigli,sizeof(int));


if((fdFile=open(nomeFile,O_CREAT | O_RDWR,0777))<0){
	perror("\n");
	exit(1);
}

printf("\nFile %s aperto\n",argv[3]); fflush(stdout);


for(i=0;i<nFigli;i++){
	childpid[i]=fork();
	if(childpid[i]==0)
		break;	
	else if(childpid[i]<0){
		perror("\n"); exit(1);	
	}
}

if(i==nFigli){ //sono padre
	
	printf("\nSono padre\n");
	fflush(stdout);
	actualLvl=0;
	for(i=0;i<nFigli;i++){
		printf("\nSono padre, e' terminato il mio figlio con pid %d\n",wait(NULL));
		fflush(stdout);
	}
	printf("\nTerminati tutti i miei figli\n");
	lseek(fdFile,0,SEEK_SET);
	char buff[100];
	printf("\nContenuto del file %s\n",argv[3]);
	fflush(stdout);
	while(read(fdFile,buff,100)>0)
		printf("%s",buff);
		
	
	
	
	
}


else{
	char *toWrite;
	actualLvl=1;
	while(actualLvl<=nLivello){	
		mypid=getpid();
		
	  	toWrite=calloc(20,sizeof(char));
		sprintf(toWrite,"%d\n",getpid());
		if(write(fdFile,toWrite,strlen(toWrite))<0){
			perror("\n");
			exit(1);		
		}
		if(actualLvl<nLivello && fork()!=0)
			break;
		else actualLvl++;
	}
printf("\nSono il figlio di livello %d e con pid %d e ho finito il mio lavoro,attendo terminazione di mio figlio\n",actualLvl,mypid);
fflush(stdout);
wait(NULL);

close(fdFile);

}







}
