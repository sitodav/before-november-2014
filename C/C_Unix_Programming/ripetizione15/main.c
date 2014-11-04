#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

int boolVar;
int myInd;
int n;
int childpid[2];



void sigusr1handler(int sign);
void sigusr2handler(int sign);
int main(int argc,char *argv[]){
if(argc!=2){
	printf("\npassami n\n");
	exit(0);
}


if(signal(SIGUSR1,sigusr1handler)==SIG_ERR || signal(SIGUSR2,sigusr2handler)==SIG_ERR){
	perror("\n");
	exit(1);
}

sscanf(argv[1],"%d",&n);
if(n<0)
	exit(0);




int i;

for(i=0;i<2;i++){
	childpid[i]=fork();
	if(!childpid[i])
		break;
}


myInd=i;

printf("\nmyInd%d\n",myInd); fflush(stdout);
sleep(2);

if(myInd==2){
	if( !(n%2) ){
		for(i=0;i<2;i++)
			if(kill(childpid[i],SIGUSR1)!=0){
				perror("\n");
				exit(1);			
			}	
	}
	else{
		
		for(i=0;i<2;i++)
			if(kill(childpid[i],SIGUSR2)!=0){
				perror("\n");
				exit(1);			
			}	
	}
	
}


else if(myInd==0){
	printf("\nSono primo figlio!\n");
	fflush(stdout);
	pause();
}


else if(myInd==1){
	printf("\nSono secondo figlio!\n");
	fflush(stdout);
	pause();
}


return 0;
}


void sigusr1handler(int sign){

if(myInd==0){
	if(boolVar==0){ //SIGUSR1 direttamente dal padre
		printf("Sono primo figlio, il cubo e' %d",n*n*n);
		fflush(stdout);
		exit(0);
	}
	else if(boolVar==1){ 
		printf("\nsono il primo figlio, MESSAGGIO E TERMINO!\n");
		fflush(stdout);
		exit(0);
	
	}
}

else if(myInd==1){
	printf("\nSono secondo figlio! Termino!\n");
	fflush(stdout);
	exit(1);
}

}



void sigusr2handler(int sign){

if(myInd==0){
	boolVar=1;
	pause();
}

else if(myInd==1){
	printf("\nSono secondo figlio, reciproco: %f",(1.0f/n));
	fflush(stdout);
	sleep(2*n);
	if(kill(childpid[0],SIGUSR1)<0){
		perror("\n");
		exit(1);	
	}
	exit(0);
	
}



}
