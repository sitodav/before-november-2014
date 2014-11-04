//prova di esercizio mio dove i figli vengono duplicati con un diverso flow di codice (invece del
//do while)



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>


int main(int argc,char *argv[],char *envp[]){
int childpid,i;
char *new_argv[argc+1];

if(argc<2){
	printf("\nuso: cmd arg1 arg2 arg3\n");
	exit(0);
}


while(1){

	if(argc==2){
		printf("\nEccomi%s",argv[1]); fflush(stdout);
		exit(0);	
	}
	
	childpid=fork();
		
	 if(childpid>0){
		childpid=fork();
			
		if(childpid==0){
			strcpy(new_argv[0],argv[0]);
			i=2;
			while(argv[i]!=NULL)
				strcpy(new_argv[i-1],argv[i++]);
			new_argv[i]=NULL;

			execve(argv[0],new_argv,envp);
			printf("\nExec fallita\n");

		}
		for(i=0;i<2;i++)
			wait(NULL);
		printf("%s",argv[1]);
		fflush(stdout);
		break;
			
	}
	strcpy(new_argv[0],argv[0]);
	i=2;
	while(argv[i]!=NULL)
		strcpy(new_argv[i-1],argv[i++]);
	new_argv[i]=NULL;
	execve(argv[0],new_argv,envp);
	printf("\nExec fallita\n");
	
}




return 0;}
