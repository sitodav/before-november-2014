/*Scrivere un programma C e Posix sotto Linux che implement i la segue nte pipeline acquisita in input da
riga di comando: ps -f I grep :+,.Axt I wc -c
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>



int main(int argc,char *argv[]){

if(argc!=2){
	printf("\nErrore d'uso\n");
	exit(1);
}

char *argomenti[9],*tok;
int i=0;
argomenti[0]=calloc(20,sizeof(char));
tok=strtok(argv[1]," ");
strcpy(argomenti[0],tok);

while((tok=strtok(NULL," |"))!=NULL){
	i++;
	argomenti[i]=calloc(20,sizeof(char));
	strcpy(argomenti[i],tok);
}

int j;
/*printf("\nArgomenti\n");

for(j=0;j<=i;j++){
	printf("\n%s",argomenti[j]);
	fflush(stdout);

}
*/

int fdPipe1[2],fdPipe2[2],childpid[2];
if(pipe(fdPipe1)<0 || pipe(fdPipe2)<0){
	perror("\n");
	exit(1);
}

for(j=0;j<2;j++){
	childpid[j]=fork();
	if(childpid[j]==0)
		break;

}

if(j==2){
	//padre
	/*printf("\nSono padre, miei argomenti sono %s %s\n",argomenti[0],argomenti[1]);
	fflush(stdout);*/
	
	close(fdPipe1[0]);
	close(fdPipe2[1]);
	close(fdPipe2[0]);
	
	if(dup2(fdPipe1[1],1)<0){
		perror("\n");
		exit(1);	
	}
	
	

	execlp(argomenti[0],argomenti[0],argomenti[1],NULL);
	printf("\nErrore della execlp in padre\n"); fflush(stdout);
}


else if(j==0){
		//primo figlio
	close(fdPipe1[1]);
	close(fdPipe2[0]);

	/*printf("\nSono primo figlio, miei argomenti sono %s %s\n",argomenti[2],argomenti[3]);
	fflush(stdout);*/

	if(dup2(fdPipe1[0],0)<0){
		perror("\n");
		exit(1);	
	}
	if(dup2(fdPipe2[1],1)<0){
		perror("\n");
		exit(1);	
	}

	

	execlp(argomenti[2],argomenti[2],argomenti[3],NULL);
	printf("\nErrore della execlp in primo figlio\n"); fflush(stdout);
	
}

else{
		//secondo figlio
	close(fdPipe1[1]);
	close(fdPipe1[0]);
	close(fdPipe2[1]);

	/*printf("\nSono secondo figlio, miei argomenti sono %s %s\n",argomenti[4],argomenti[5]);
	fflush(stdout); */

	if(dup2(fdPipe2[0],0)<0){
		perror("\n");
		exit(1);	
	}



	execlp(argomenti[4],argomenti[4],argomenti[5],NULL);
	printf("\nErrore della execlp in secondo figlio\n"); fflush(stdout);
}



return 0;



}
