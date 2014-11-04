#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>



int main(int argc,char *argv[]){
int childpid;
int termstat;
int ret;

if((childpid=fork())==0){//siamo nel figlio
	execlp("ls","-l",NULL);
	printf("\nsiamo nel figlio ma l'exec e' fallita\n");
}

else if(childpid>0){ //siamo nel padre
	do
		ret=waitpid(childpid,&termstat,WNOHANG);
	while 
		(ret!=childpid); //usiamo una waitpid sul pid specifico dell'unico figlio, ma 					non bloccante, quindi continuiamo a lanciarla facendo polling 					finche' non ritorna proprio il valore dell'unico figlio, ad 							indicare quindi che quel figlio e' terminato
	if(WIFEXITED(termstat))
		printf("\nFiglio terminato volontariamente\n");
	else if(WIFSIGNALED(termstat))
		printf("\nFiglio interrotto da signal\n");
}

else { //siamo nel padre ma la fork e' fallita
	printf("\nfork() fallita\n");
	exit(1);
}








return 0;
}
