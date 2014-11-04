#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>


void SIGCHLDhandler(int sign);
int main(int argc,char *argv[]){
char pathfifo[FILENAME_MAX]; getcwd(pathfifo,FILENAME_MAX); strcat(pathfifo,"\\miafifo\0");
int childpid[2],i,termStat;
if(signal(SIGCHLD,SIGCHLDhandler)==SIG_ERR){
	perror("\n");
	exit(1);
}

if(access(pathfifo,F_OK)!=0){
	printf("\nLa fifo specificata non esiste. La creo\n"); fflush(stdout);
	printf("\nCreazione fifo in:\n %s",pathfifo);
	if(mkfifo(pathfifo,0777)<0){
		perror("\n");	
		exit(1);	
	}
}

for(i=0;i<2;i++){
	childpid[i]=fork();
	if(childpid[i]==0) break;
}


if(i==2){
	waitpid(childpid[0],&termStat,0);
	
	if(WIFEXITED(termStat)){
		printf("\nIl risultato ritornato dal processo figlio A e' %d\n",WEXITSTATUS(termStat));
		fflush(stdout);
	
	}

}

if(i==0){
	execl("./lettore","./lettore",pathfifo,NULL);
	printf("\nExec per lettore (proc A) fallita!\n"); 
	fflush(stdout);

}

if(i==1){
	execl("./scrittore","./scrittore",pathfifo,NULL);
	printf("\nExec per scrittore (proc B) fallita\n");
	fflush(stdout);
	
}
	





return 0;
}




void SIGCHLDhandler(int sign){
	printf("\nFiglio terminato\n");
	fflush(stdout);


}
