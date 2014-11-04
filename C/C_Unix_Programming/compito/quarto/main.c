#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <errno.h>
#include <string.h>


int lock=0;

void SIGUSR1handler(int sign);
int main(int argc,char *argv[]){
int n,pipe1[2];
printf("\nInserisci n processi\n");
scanf("%d",&n);
while(getchar()!='\n');

int *childpid=calloc(n,sizeof(int));
int i;

if(signal(SIGUSR1,SIGUSR1handler)==SIG_ERR){
	perror("\n");
	exit(1);

}
	
if(pipe(pipe1)<0){
	perror("\n");
	exit(1);
}

for(i=0;i<n;i++){
	childpid[i]=fork();
	if(childpid[i]==0)
		break;

}


if(i==n){
	printf("\nSono padre\n");
	fflush(stdout);
	//il padre sblocca un figlio e si sblocca, viene sbloccato dal figlio e legge il char che gli ha messo in pipe e lo stampa, se pero' la read ritorna 0 e' perche' non c'e' rimasto piu' nessun figlio con la sua estremita' in scrittura aperta
	close(pipe1[1]); //7altrimenti non si accorge quando tutti i figli chiudono la loro perche' c'e' la sua aperta inutilmente	
	int actualI=-1;	
	char t;
	while(1){

		sleep(1);
		actualI++;
		if(actualI>n-1)
			actualI%=n;
		
		fflush(stdout);
		if(kill(childpid[actualI],SIGUSR1)<0){
			perror("\n");
			exit(1);		
		}
		printf("devo lanciato signal a----> %d",actualI);
		
		
		
		if(read(pipe1[0],&t,sizeof(char))==0){
			printf("\nTutti i figli han chiuso pipe!\n"); fflush(stdout);			
			break;
		}
		printf("Sono padre: letto %c\n",t);
		fflush(stdout);
	
		
	}
	printf("\nSono padre, tutti i figli han chiuso il loro lato in scrittura della pipe condivisa!\n"); fflush(stdout);
	

}

if(i!=n){
	int fdFileR;
	char *toOpen=calloc(FILENAME_MAX,sizeof(char));
	sprintf(toOpen,"%d",i);
	strcat(toOpen,".txt");
	if((fdFileR=open(toOpen,O_RDONLY))<0){
		perror("\n");
		exit(1);	
	}
	char t;
	char *allRead=calloc(200,sizeof(char));
	int j=0;
	while(read(fdFileR,&t,sizeof(char))>0){
		if(i%2==0 && (int)t>=48 && (int)t<=67)
			allRead[j++]=t;
		else if(i%2!=0 && ( (int)t<48 || (int)t>67 )){
			allRead[j++]=t;
		}
			
		
	}
	printf("\nSono processo %d e ho tirato fuori:\n%s\n",i,allRead);
	fflush(stdout);
	
	
	int k;
	for(k=0;k<j;k++){
		while(lock==0);
		//viene sbloccato da sigusr1 dal padre
		if(write(pipe1[1],allRead+k,sizeof(char))<0){
			perror("\n");
			exit(1);		
		}
		
		lock=0;
		
	}
	printf("\nTerminati char tirati fuori!\n");
	fflush(stdout);
	close(pipe1[1]);
	exit(0);
			
	
	


}

}



void SIGUSR1handler(int sign){
	lock=1;
	printf("\nMi hai sbloccato!\n");
	fflush(stdout);

}
