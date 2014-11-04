#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc,char *argv[]){

int pid_figlio;
printf("\nPrima della fork: mypid->%d myppid->%d (non ancora forkato!)\n",getpid(),getppid());

pid_figlio=fork(); //forkiamo()
if(pid_figlio==0) //allora la fork ha ritornato 0, e siamo quindi nel figlio
	printf("\n sono il figlio : mypid-> %d    myppid->%d (dopo la fork!)\n",getpid(),getppid());
else 
	printf("\nSono il padre di %d\n",pid_figlio);



return 0;
}
