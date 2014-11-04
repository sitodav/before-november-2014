/*Scrivere un programma C e Posix sotto Linux che implement i la segue nte pipeline acquisita in input da
riga di comando: ps -f | grep *.txt | wc -c
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>


int main(int argc,char *argv[]){

int pipe1[2],pipe2[2];

if(pipe(pipe1)<0 || pipe(pipe2)<0){
	perror("\n");
	exit(1);
}

int childpid[3],i;
for(i=0;i<3;i++){
	childpid[i]=fork();
	if(childpid[i]==0)
		break;

}

if(i==0){
	close(pipe1[0]);
	close(pipe2[1]);
	close(pipe2[0]);
	if(dup2(pipe1[1],1)<0){
		perror("\n");
		exit(1);	
	}
	execlp("echo","echo","\"ciao come va\"",NULL);
	printf("\nExec per PS fallita!\n");
	fflush(stdout);

}

else if(i==1){
	close(pipe1[1]);
	close(pipe2[0]);
	if(dup2(pipe1[0],0)<0){
		perror("\n");
		exit(1);	
	}
	if(dup2(pipe2[1],1)<0){
		perror("\n");
		exit(1);	
	}
	execlp("grep","grep","i",NULL);
	printf("\nExec per grep fallita!\n");
	fflush(stdout);
}

else if(i==2){
	close(pipe1[1]);
	close(pipe1[0]);
	close(pipe2[1]);
	
	if(dup2(pipe2[0],0)<0){
		perror("\n");
		exit(1);	
	}
	execlp("wc","wc","-c",NULL);
	printf("\nExec per WC	 fallita!\n");
	fflush(stdout);
}

else if(i==3){
	printf("\nSono padre, attendo!\n");

}



}


