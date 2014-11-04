//il figlio legge dati dal padre, attraverso una pipe

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>



int main(int argc,char *argv[]){

int fdpipe[2],childpid;
char toread[100]="",towrite[100]="";

//il processo crea pipe
if(pipe(fdpipe)<0){
	perror("\n");
	exit(1);
}

//il padre fa figlio
if (fork()==0){ //sono il figlio
	//quindi chiudiamo l'estremita in scrittura
	close(fdpipe[1]);
	//e leggiamo dalla pipe
	printf("\nSono il figlio, attendo di ricevere dati dal padre\n");
	fflush(stdout);
	while(read(fdpipe[0],toread,99)!=0){
		printf("\nRICEVUTO\n%s",toread);
		fflush(stdout);
	}	
	exit(0);
}	

//qui ci arriva solo il padre
//quindi chiudo l'estremita' in lettura della pipe
close(fdpipe[0]);
printf("\nSono padre, cosa vuoi inviare al figlio ?\n"); fflush(stdout);
fgets(towrite,100,stdin);
towrite[strlen(towrite)-1]='\0';
if(write(fdpipe[1],towrite,strlen(towrite)+1)<0){
	perror("\n");
	exit(1);
}




return 0;}
