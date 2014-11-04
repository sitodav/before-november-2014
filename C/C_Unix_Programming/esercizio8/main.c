#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/select.h>





int main(int argc,char *argv[],char *envp[]){

int nFatt,childpid,termStat,retValue,retValue2;
struct timeval tempo;
char param[10];



printf("\nIns numero di cui calc fattoriale\n");
scanf("%d",&nFatt);
while(getchar()!='\n');
sprintf(param,"%d",nFatt);

while(1){
	if((childpid=fork())<0){
		perror("\n");
		exit(1);
	}
	if(childpid==0){ //allora sono il figlio A, e quindi divento con l'exec l'eseguibile del fattoriale
		execl("./fattoriale","./fattoriale",param,NULL); //cmd,argv[0],argv[1],NULL
		printf("\nExecl per proc fattoriale, fallita!");
		exit(1);	
	}
	else{ //sono il padre del proc figlio A, quindi aspetto il suo term stat (per semplicita' non consideriamo il caso in cui il figlio sia interrotto da signal ma ipotizziamo che restituisca sempre un exit status dovuto ad una terminazione volontaria)
	waitpid(childpid,&termStat,0);
	if(WIFEXITED(termStat))
		retValue=WEXITSTATUS(termStat);
		printf("\nFiglio terminato con exit value %d",retValue);
		//in base al valore con cui il figlio e' terminato, continuiamo l'esecuzione:
		if(retValue==0)
			exit(0); //se 0 allora termino anche io padre
		else if(retValue==1) //altrimenti faccio un altra fork (controllando eventuale errore) e ilfiglio diventa ls
			if((childpid=fork())<0 )
				exit(1);
			else{ //qui stiamo dopo la fork riuscita 
				if(childpid==0){ //sono secondo figlio quindi
					sleep(1);
					execlp("ls","ls",NULL); //come da esercizio 
					printf("\nExec fallita\n");				
				}
				else{ //allora qui sono ancora padre di tutti
					//quindi devo attendere la terminazione del figlio sopra
				waitpid(childpid,NULL,0); //non mi interessa il term stat del secondo figlio
				continue; //e riavvio il ciclo while ricominciando tutto da capo
				
				
				}
			}
		else if(retValue==2){//il figlio ha ritornato 2, quindi faccio una fork e l'ulteriore figlio diventa ps
			if((childpid=fork())<0) //quindi faccio fork e controllo l'errore
				exit(1);
			else if(childpid==0){ //se sono il figlio aspetto 2 secondi quindi e divento ps
				sleep(2);
				execlp("ps","ps",NULL);
				printf("\nExec per ps fallita\n");			
			}
			else{ //allora sono ancora il padre, quindi attendo terminazione figlio e ricomincio da capo
				waitpid(childpid,NULL,0);
				continue;
			}

		}

	}




}







return 0;
}
