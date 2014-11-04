#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>



int main(int argc,char *argv[]){

int n,childpid,termstat;
char topass[10]="\0";

printf("\nInserire l'ordine del numero di fibonacci da calcolare\n");
scanf("%d",&n); while(getchar()!='\n');

sprintf(topass,"%d",n);

if((childpid=fork())==0){
	execl("./fib","./fib",topass,NULL); //nome_eseguibile,argv[0],argv[1],NULL
	printf("\nExec di fib fallito\n"); fflush(stdout);
}

else if(childpid>0){
	waitpid(childpid,&termstat,0);
	if(WIFEXITED(termstat)){
		printf("\nIl valore di fib per ordine %d e' %d\n",n,WEXITSTATUS(termstat));
		if(WEXITSTATUS(termstat)<50){ //per qualche motivo la traccia di questo esercizio chiede di fare tutta una serie di cose se il valore ritornato per fib e' <50...BOH!?
			if((childpid=fork())==0){
				execlp("ls","ls","-al",NULL);
				printf("\nExec fallita\n");
			}
			else if(childpid>0){
				printf("\nAttendo mio figlio\n");
				waitpid(childpid,NULL,0); //non ci interessa l'exit status del secondo figlio			
				printf("\nFiglio terminato, e non piu' zombie. Muoio anche io\n");	
			}
			else 
				perror("\n");
					
		}
	}
	
	else  //allora non e' terminato normalmente, ma e' stato interrotto da signal
		printf("\nStampo questo messaggio e termino\n");
	


}


else 
	perror("\n");


return 0;
}
