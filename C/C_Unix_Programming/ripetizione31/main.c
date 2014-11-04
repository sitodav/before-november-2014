#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>


/*Scrivere un programma C e Posix in ambiente Linux che implementi la seguente pipeline: ps xo comm
| sort | uniq 
*/


int main(int argc,char *argv[]){

int childpid[3],i,fdPipe1[2],fdPipe2[2];

if(pipe(fdPipe1) || pipe(fdPipe2) ){
	//perror("\n");
	exit(1);
}


for(i=0;i<3;i++){
	childpid[i]=fork();
	if(childpid[i]==0)
		break;
}

if(i==3){
	int j;
	for(j=0;j<2;j++){
		close(fdPipe1[j]);
		close(fdPipe2[j]);	
	}

	
	for(j=0;j<3;j++)
		if(waitpid(childpid[j],NULL)<0){
			//perror("\n");
			exit(1);		
		}
	printf("\nFigli terminati!\n"); fflush(stdout);
	return;

}

else if(i==0){
	int j;
	printf("\nSono 0\n");
	fflush(stdout);
	close(fdPipe2[0]); close(fdPipe2[1]);
	close(fdPipe1[0]);
	dup2(fdPipe1[1],STDOUT_FILENO);
	char **par=(char **)calloc(4,sizeof(char *));
	for(j=0;j<3;j++)
		*(par+j)=calloc(FILENAME_MAX,sizeof(char));
	strcpy(par[0],"ps");
	strcpy(par[1],"xo");
	strcpy(par[2],"comm");
	par[3]=(char *)NULL;
	execvp("ps",par);
	printf("\nProblema con la execvp del figlio 0!\n"); 
}

else if(i==1){
	printf("\nSono 1\n");
	fflush(stdout);
	
	close(fdPipe1[1]);
	close(fdPipe2[0]);
	dup2(fdPipe1[0],STDIN_FILENO);
	dup2(fdPipe2[1],STDOUT_FILENO);
	execlp("sort","sort",(char *)NULL);
	printf("\nerrore con la execlp del figlio 1\n"); 

}

else if(i==2){
	close(fdPipe1[0]); 
	close(fdPipe1[1]);
	close(fdPipe2[1]);
	dup2(fdPipe2[0],STDIN_FILENO);
	execlp("uniq","uniq",(char *)NULL);
	printf("\nerrore con la execlp del figlio 2\n"); 
}



}
