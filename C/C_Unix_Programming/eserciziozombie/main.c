#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/select.h>


int main(int argc, char *argv[],char *envp[]){

int childpid,child2pid,termstat;
fd_set set1;
char r;


if((childpid=fork())==0){
	printf("\nSono il processo figlio, e termino diventando ZOMBIE\n");
	exit(123);
}

else if(childpid>0){
//il padre fa un altra fork ancora, e una diventa un proc "ps -F" con exec, e lui padre (di 2 processi ora) fa una wait pid quando si inserisce y da tastiera, per far togliere dallo stato zombie il primo figlio (non ci interessa che il processo ps vada in zombie)

	if((child2pid=fork())==0){
			execlp("ps","ps","-F",NULL);
			printf("\nSono il secondo figlio, e ho fallito a diventare proc ps\n");	
	}
	
	else{	printf("\nPREMI TASTO Y PER ESEGUIRE WAIT SUL PRIMO FIGLIO ZOMBIE\n"); 
		fflush(stdout);
		FD_ZERO(&set1);
		FD_SET(fileno(stdin),&set1);
		if(select(1,&set1,NULL,NULL,NULL)<0)
			exit(0);
		//select scattata,se lo stdin e' ready (quindi abbiamo inserito un tasto)
		if(FD_ISSET(fileno(stdin),&set1)){
			r=getc(stdin);
			if(r=='y'){
				waitpid(childpid,&termstat,0);
				
				if(WIFEXITED(termstat))
				printf("\nPrimo figlio term con%d\n",WEXITSTATUS(termstat));	
				else if(WIFSIGNALED(termstat))
				printf("\nFiglio int per segn %d",WTERMSIG(termstat));		
			}
			else exit(0);
		}

	}




}


else 
	perror("\n");









return 0;}

