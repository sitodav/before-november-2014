#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc,char *argv[],char *envp[]){



int childpid,termstat,pidTerminato;

if((childpid=fork())==0){ //creiamo figlio che diventa ls tramite exec
	execlp("ls","ls","-l",NULL);
	printf("\nsono il figlio:exec per ls fallita\n"); fflush(stdout);
	


}

else if(childpid>0){ //il padre invece va in attesa con la wait bloccandosi finche' il figlio non diventa zombie
	printf("\nIn attesa\n"); fflush(stdout);
	wait(&termstat);
//quando il figlio e' terminato, lo analizziamo con la wait, e creiamo un nuovo figlio
	printf("\nnuovo figlio\n"); fflush(stdout);
	childpid=fork();
	if(childpid<0)
		perror("\n");
	else if(childpid==0){ //il nuovo figlio stampa messaggio e muore dopo aver aspettato 5 secondi
		sleep(5);
		printf("\nSono il figlio e stampo a video questo messaggio\n");
		exit(123);	
	}
		
	else{ //mentre il padre attende che termini e poi stampa il pid dell'ultimo figlio terminato
		pidTerminato=wait(NULL); //non ci interessa il term status
		printf("e' terminato il nostro figlio con pid %d\n",pidTerminato); 
	
		
	}
}

else 
	perror("\n");







return 0;
}
