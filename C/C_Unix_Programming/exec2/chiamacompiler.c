#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc,char *argv[]){

char toCompile[FILENAME_MAX],compiled[FILENAME_MAX];

printf("\nIns nome file da compilare\n");
fgets(toCompile,FILENAME_MAX,stdin);
toCompile[strlen(toCompile)-1]='\0';

printf("\nIns nome eseguibile\n");
fgets(compiled,FILENAME_MAX,stdin);
compiled[strlen(compiled)-1]='\0';

if(fork()==0){
	execlp("gcc","-o",compiled,toCompile,NULL);
	return -1; //torna -1 nel caso in cui non sia andata a buon fine la exec
}

//il padre controlla il termination status del figlio
int statret;
wait(&statret);
if(WIFEXITED(statret) && !WEXITSTATUS(statret))
	printf("L'exec del figlio verso il compilatore non e' andata a buon fine");	
else printf("\nTUTTO OK\n");




}
