#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void handler(int sign);
int main(int argc,char *argv[]){


void (*pHandler[2])(int);
pHandler[0]=&handler;
pHandler[1]=&handler;






int childpid[2];
childpid[1]=fork();
int i;
for(i=0;i<2;i++){
	childpid[i]=fork();
	if(childpid[i]==0)
		break;

}

if(i==2){
	sleep(5);
	kill(childpid[0],SIGUSR1);
	kill(childpid[1],SIGUSR1);
	
}

else{
	if(signal(SIGUSR1,pHandler[i])==SIG_ERR){
	perror("\n");
	exit(1);
	}
	pause();	

}

}


void handler(int sign){
printf("\nCIAO!\n");


}
