#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>

int cont=0,childpid[2],n,n1,n2,myid;

void sigalrmhandler(int sign);
void sigusr1handler(int sign);
int main(int argc, char *argv[]){
if(argc!=5){
	printf("\nuso: this n n1 n2 c\n");
	exit(1);	

}

sscanf(argv[1],"%d",&n);
sscanf(argv[2],"%d",&n1);
sscanf(argv[3],"%d",&n2);

if(signal(SIGUSR1,sigusr1handler)==SIG_ERR){
	perror("\n");
	exit(1);

}

for(myid=0;myid<2;myid++){
	childpid[myid]=fork();
	if(childpid[myid]==0)
		break;	
}

if(myid==0){
//primo figlio
printf("\nSono il primo figlio, attendo %d secondi\n",n1);
sleep(n1);
printf("\nSono primo figlio, ho terminato l'attesa e divento prog %s\n",argv[4]); fflush(stdout);
execl(argv[4],argv[4],NULL);
printf("\nExec fallita\n");

}

if(myid==1){
//secondo figlio
	if(signal(SIGALRM,sigalrmhandler)==SIG_ERR){
		perror("\n");
		exit(1);
	}

	alarm(n2);
	//nel frattempo prima che ci arrivi il SIGALRM
	while(1){//mando un segnale al secondo a mio padre
		sleep(1);	
		if(kill(getppid(),SIGUSR1)<0){
			perror("\n");
			exit(1);
		}	
	}
	
}

if(myid==2){
//sono padre
//qui andrebbe usata una sigprocmask per bloccare tutti i segnali che non sono sigusr1..ma per semplicita'...
while(1)
	pause();
//continuo a mettermi in attesa di un segnale in maniera infinita, finche'
//la mia var glob cont non arriva ==n, e in quel caso l'handler mi fa terminare




}



return 0;
}

void sigusr1handler(int sign){
if(cont==n){
	printf("\nAttendo i miei figli\n"); fflush(stdout);
	wait(NULL);
	wait(NULL);
	printf("\nTermino\n"); fflush(stdout);
	exit(0);
}

printf("\nIl mio pid e' %d\n",getpid());
cont++;

}


void sigalrmhandler(int sign){
if(kill(childpid[0],SIGINT)<0){ //questo handler lo usera' soltanto il secondo figlio, che essendo stato forkato per secondo, ha nell'array il pid del primo figlio al quale dovra' mandare il SIGINT
	perror("\n");
	exit(1);

}

printf("\nSono il secondo figlio: mandato SIGINT a mio fratello! TERMINO\n");
fflush(stdout);
exit(0);





}
