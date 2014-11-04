#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc,char *argv[]){
int pipeFd[2];
int childpid[2];
int i;
char pattern[30];

if(pipe(pipeFd)<0){
	perror("\n");
	exit(1);
}
 //aperto pipe con le 2 estremita'
i=0;
while(i<2){
	childpid[i]=fork();
	if(childpid[i]==0) //sono il figlio iesimo
		break;
	else
		i++;
		
}

//qui arrivera' il padre con i==2. Il primo figlio avra' i==0, il secondo avra' i==1

if(i==2){
	printf("\nSono il padre, non devo fare piu' nulla. Chiudo le estremita' pipe per me e termino\n"); fflush(stdout);
	close(pipeFd[0]);
	close(pipeFd[1]);
	pause();
}

if(i==0){ //primo figlio forkato
	//non ci serve l'estremita' in lettura della pipe:
	close(pipeFd[0]);
	//impostiamo che l'fd associato allo stdout punti all'estremita' in scrittura della pipe
	//invece che al buffer stdout collegato al terminale
	if(dup2(pipeFd[1],1)<0){
		perror("\n");
		exit(1);	
	}
	//	close(pipeFd[1]);
	//e diventiamo con exec il nuovo programma, che manterra' il suo standard output collegato in realta' all'estremita' in scrittura della pipe anonima
		
	execlp("ls","ls","-R",NULL); //nome,argv0,argv1,NULL
	printf("\nExec fallita\n");
	exit(1);

}

if(i==1){ //secondo figlio forkato
	//non ci serve l'estremita' in scrittura della pipe anonima
	close(pipeFd[1]);
	//colleghiamo lo standard input (riferito con 0) all'estremita' in lettura della pipe invece che //al buffer stdin
	printf("\nInserisci pattern\n"); fflush(stdout);
	fgets(pattern,30,stdin);	
	if(dup2(pipeFd[0],0)<0){
		perror("\n");
		exit(1);	
	}
	//close(pipeFd[0]); 
	
	//e diventiamo il programma scelto con exec 
	execlp("grep","grep",pattern,NULL);
	printf("\nExec fallita per grep!\n");
	fflush(stdout);
	

}

return 0;}
