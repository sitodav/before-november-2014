#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#define UNLOCKED 0
#define LOCKED 1

int lock=LOCKED;
void SIGUSR1handler(int sign);
int fatt(int n);
int main(int argc,char *argv[]){

int n,childpid,ft;
if(signal(SIGUSR1,SIGUSR1handler)==SIG_ERR){
	perror("\n");
	exit(1);
}

while(1){
	printf("\nInserisci un numero da tastiera\n"); 
	scanf("%d",&n);
	while(getchar()!='\n');
	
	childpid=fork();
	if(childpid==0)
		break;	
	int exChild;
	if(waitpid(childpid,&exChild,0)<0){
		perror("\n");
		exit(1);	
	}
	if(WIFEXITED(exChild)){
		exChild=WEXITSTATUS(exChild);	
	}
	printf("\nMio figlio e' terminato con stato %d\n",exChild);
	if(exChild==0){
		printf("\nSono il padre e termino!\n"); 
		exit(0);	
	}
	else{
		
		childpid=fork();
		if(childpid==0){
			while(lock==LOCKED); //busy wait
			printf("\nSono figlio, sono stato sbloccato, e divento prog_i\n"); fflush(stdout);
			char newArgv[10];
			sprintf(newArgv,"%d",exChild);
			execl("prog_i","prog_i",newArgv,NULL);
			printf("\nExecl fallita!\n");
			fflush(stdout);
			exit(1);		
		}
		else{ //padre
			
			if(kill(childpid,SIGUSR1)<0){
				perror("\n");
				exit(1);			
			}
			if(waitpid(childpid,NULL,0)<0){
				perror("\n");
				exit(1);			
			}	
			continue;
		}
		
	}
	

}

//qui arriva solo il figlio
srand((unsigned int)time(NULL));

ft=fatt(n);
printf("\nSono figlio, il fattoriale di %d e' %d\n",n,ft);
fflush(stdout);
int ex_stat=rand()%3;
exit(ex_stat);




return 0;
}


int fatt(int n){
int i,fatt=1;
for(i=1;i<=n;i++)
	fatt*=i;
return fatt;	


}


void SIGUSR1handler(int sign){
printf("\nsbloccato\n"); fflush(stdout);
lock=UNLOCKED;
return;

}
