#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>



void SIGCHLDhandler(int sign);

int main(){
int childpid[3],i=0;
int pipe1fd[2],pipe2fd[2]; //2 pipe perche' bisogna far dialogare 3 figli (primo con secondo e sec con terzo)
char *pattern;


if(signal(SIGCHLD,SIGCHLDhandler)==SIG_ERR){
	perror("\n");
	exit(1);
}

//prima delle fork creiamo la pipe

if(pipe(pipe1fd)<0 || pipe(pipe2fd)<0){
	perror("\n");
	exit(1);
}


while(i<3){
	childpid[i]=fork();
	if(childpid[i]==0)
		break;
	else i++;
}

//il primo figlio spawnato arrivera' direttamente qui con i=0, il secondo con i=1, il terzo con
//i=2 e il padre con i=3


if(i==3){ //padre
	close(pipe2fd[0]);
	close(pipe2fd[1]);
	close(pipe1fd[0]);
	close(pipe1fd[1]);
//e lo mettiamo in busy waiting a lanciare continuamente kill sui 3 pid dei figli, e finche' ha
//ancora qualche figlio esistente, non esce dal while e non ritorna
	i=0;
	int k;
	while(1){
		k=0;
		for(i=0;i<3;i++)
			if(kill(childpid[i],0)!=-1)
				k++;
		if(k==0) break;
	}

	exit(0);

}

if(i==0){ //primo figlio forkato
	//non ci serve per niente la seconda pipe, ma ci serve l'estremita'
	//in scrittura solo della prima, quindi
	close(pipe2fd[0]);
	close(pipe2fd[1]);
	close(pipe1fd[0]);
	//dup2 tra l'fd di riferimento dello stdout e l'estremita' in scrittura della prima pipe
	if(dup2(pipe1fd[1],1)<0){
		perror("\n");
		exit(1);
	}
	execlp("ls","ls",NULL);
	printf("\nExec per ls fallita!\n"); fflush(stdout);

}

if(i==1){ //secondo figlio forkato
	//avra' bisogno dell'estremita' in lettura della prima pipe (che colleghera'
	//al suo standard input) e dell'estremita' in scrittura della seconda pipe (che colleghera' al suo standard output)
	close(pipe1fd[1]);
	close(pipe2fd[0]);
	if(dup2(pipe1fd[0],0)<0){
		perror("\n");
		exit(1);
	}

	if(dup2(pipe2fd[1],1)<0){
		perror("\n");
		exit(1);
	}
	execlp("sort","sort",NULL);
	printf("\nExec per sort fallita!\n"); fflush(stdout);

}

if(i==2){
	//non ci serve ne' la prima pipe, ne' l'estremita' in scrittura della seconda,
	//ma solo l'estremita' in lettura della seconda
	close(pipe1fd[0]);
	close(pipe1fd[1]);
	close(pipe2fd[1]);
	printf("\nInserisci pattern\n"); //prima di redirezionare lo standard input dalla pipe, prendiamo dallo stdin collegato a tastiera il pattern
	pattern=calloc(30,sizeof(char));
	fgets(pattern,30,stdin);

	if(dup2(pipe2fd[0],0)<0){
		perror("\n");
		exit(1);
	}
	execlp("grep","grep",pattern,NULL);
	printf("\nExec per grep fallita\n");
	fflush(stdout);




}



}

void SIGCHLDhandler(int sign){
//installiamo il SIGCHLDhandler all'interno di se' stesso, perche e' settato su SIG_IGN
//di default
if(signal(SIGCHLD,SIGCHLDhandler)==SIG_ERR){
	perror("\n");
	exit(1);
}
printf("\nRicevuto SIGCHLD: un figlio e' terminato\n");
wait(NULL); //non ci interessa exit status del figlio

}

