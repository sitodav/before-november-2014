#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>


//var globali (xke' devono essere visibili dai signal handler)
int myid;
int pipe1[2],pipe2[2];
int parzMax=0;
int fdFileRW;


void SIGINThandler(int sign);

int main(int argc,char *argv[]){
if(argc!=3){
	printf("uso nomefile.txt n\n");
	exit(1);
}

if(signal(SIGINT,SIGINThandler)<0){
	perror("\n");
	exit(1);
}

int nElem;
sscanf(argv[2],"%d",&nElem);

if(pipe(pipe1)<0 || pipe(pipe2)<0){
	perror("\n");
	exit(1);

}

int childpid[2],i;

for(i=0;i<2;i++){
	childpid[i]=fork();
	if(childpid[i]==0)
		break;	
}

//il file lo apriamo in 3 istanze separate in ognuno dei processi distinti, in modo tale che non ci sia race condition sull'accesso al puntatore offset file

if((fdFileRW=open(argv[1],O_RDWR))<0){
	perror("\n");
}

printf("\nFile aperto in %d\n", getpid());
fflush(stdout);

myid=i; //0 primo figlio, 1 secondo figlio e 2 il padre

char t[3];
int temp=0;

if(myid==2){
	close(pipe1[0]);
	close(pipe2[1]);
	close(pipe2[0]);

	printf("\nSono padre\n");
	fflush(stdout);
	for(i=0;i<nElem/3;i++){
		sleep(1); //per rallentare cosi' possiamo vedere se funziona i lCTRL-C
		lseek(fdFileRW,i,SEEK_SET);
		if(read(fdFileRW,t,2)<0){
			perror("\n");
			exit(1);		
		}
		
		sscanf(t,"%d",&temp);
		printf("\nSono processo con id: %d e ho letto numero %d\n",myid,temp);
		fflush(stdout);
		if(temp>parzMax)
			parzMax=temp;	
	}
	//i numeri vengono inviati tra i processi su pipe senza codificarli in ascii
	if(write(pipe1[1],&parzMax,sizeof(int))<0){
		perror("\n");
		exit(1);	
	}
	printf("\nSono processo con id %d e ho inviato il mio massimo parziale che risulta essere %d\n",myid,parzMax);
	fflush(stdout);

}

else if(myid==1){
	close(pipe2[0]);
	close(pipe1[1]);
	close(pipe1[0]);

	printf("\nSono secondo figlio\n");
	fflush(stdout);
	for(i=0;i<nElem;i++){
		sleep(1); //per rallentare cosi' possiamo vedere se funziona i lCTRL-C
		lseek(fdFileRW,i+(nElem/3)+1,SEEK_SET);
		if(read(fdFileRW,t,2)<0){
			perror("\n");
			exit(1);		
		}
		
		sscanf(t,"%d",&temp);
		printf("\nSono processo con id: %d e ho letto numero %d\n",myid,temp);
		fflush(stdout);
		if(temp>parzMax)
			parzMax=temp;	
	}
	//i numeri vengono inviati tra i processi su pipe senza codificarli in ascii
	if(write(pipe2[1],&parzMax,sizeof(int))<0){
		perror("\n");
		exit(1);	
	}
	printf("\nSono processo con id %d e ho inviato il mio massimo parziale che risulta essere %d\n",myid,parzMax);
	fflush(stdout);

}

else if(myid==0){
	//questo e' quello che si occupa ANCHE di raccogliere i parziali etc
	close(pipe2[1]);
	close(pipe1[1]);
	

	printf("\nSono primo figlio e devo fare tutto io!\n");
	fflush(stdout);
	for(i=0;i<nElem;i++){
		sleep(1); //per rallentare cosi' possiamo vedere se funziona i lCTRL-C
		lseek(fdFileRW,i+(2*nElem/3)+1,SEEK_SET);
		if(read(fdFileRW,t,2)<0){
			perror("\n");
			exit(1);		
		}
		
		sscanf(t,"%d",&temp);
		printf("\nSono processo con id: %d e ho letto numero %d\n",myid,temp);
		fflush(stdout);
		if(temp>parzMax)
			parzMax=temp;	
	}
	//riceviamo i parziali dagli altri 2 processi
	int *altriParz=(int *)calloc(2,sizeof(int));
	if(read(pipe1[0],altriParz,sizeof(int))<0){
		perror("\n");
		exit(1);	
	}
	if(read(pipe2[0],altriParz+1,sizeof(int))<0){
		perror("\n");
		exit(1);	
	}
	printf("\nIl parziale del padre e' %d e quello del secondo figlio e' %d\n",altriParz[0],altriParz[1]);
	printf("\nMentre il mio parziale e' %d\n",parzMax);
	for(i=0;i<2;i++){
		if(altriParz[i]>parzMax)
			parzMax=altriParz[i];
	}		
	//ora in parzMax ci sara' il parziale maggiore di tutti
	char toWrite[30];
	sprintf(toWrite,"\n\nIl max totale e' %d\n",parzMax);
	lseek(fdFileRW,0,SEEK_END);
	if(write(fdFileRW,toWrite,strlen(toWrite))<0){
		perror("\n");
		exit(1);	
	}
}
}

void SIGINThandler(int sign){
//solo il proc con myid==0 puo' fare lo scaricamento prematuro (cosi' come quello normale)
if(myid!=0) //gli altri 2 invece quando grabbano il segnale, si limitano a terminare !
	exit(0);
printf("\nRicevuta terminazione CTRL-C\nScarico quanto ho calcolato fino ad ora\n");
fflush(stdout);
char toWrite[30];
	sprintf(toWrite,"\n\nIl max totale e' %d\n",parzMax);
	lseek(fdFileRW,0,SEEK_END);
	if(write(fdFileRW,toWrite,strlen(toWrite))<0){
		perror("\n");
		exit(1);	
	}
exit(0);



}


