#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#define file1 "/tmp/miofiletemp1.txt"
#include <errno.h>

int myid,childpid[2];

int sigusrhandler(int sign);
int main(int argc,char *argv[]){
if(argc!=2){
	printf("\nuso: this n\n"); fflush(stdout);
	exit(1);
}

int n,fdFile,i,pipe1[2],pipe2[2],j,k;

sscanf(argv[1],"%d",&n);
pipe(pipe1);
pipe(pipe2);

if(signal(SIGUSR1,sigusrhandler)==SIG_ERR){
	perror("\n");
	exit(1);
}

if( (fdFile=open(file1,O_RDWR | O_CREAT,0777))<0 ){
	perror("\n");
	exit(1);
}

for(myid=0;myid<2;myid++){
	childpid[myid]=fork();
	if(childpid[myid]==0)
		break;
}

if(myid==2){
	close(pipe1[0]);
	close(pipe2[0]);
	printf("\nSono il padre, mando array pid\n"); fflush(stdout);
	if(write(pipe1[1],childpid+1,sizeof(int))<0){ //mando al primo figlio su pipe1 il pid dell'altro figlio
			perror("\n"); exit(1);
	}
	if(write(pipe2[1],childpid,sizeof(int))<0){ //mando al secondo figlio su pipe2 il pid del primo figlio
			perror("\n"); exit(1);
	}

	printf("\nSono padre, mandato!!\n"); fflush(stdout);
	close(fdFile);
	exit(0);

}

else if(myid==0){
	close(pipe1[1]);
	close(pipe2[0]);
	close(pipe2[1]);

	printf("\nSono lo scrittore,ricevo da padre pid altro figlio\n"); fflush(stdout);
	if(read(pipe1[0],childpid+1,sizeof(int))<0){
		perror("\n"); exit(1);
	}

	for(i=0;i<n;i++){
		printf("\nScrivo %d\n",i); fflush(stdout);
		if(write(fdFile,&i,sizeof(i))<0){
			perror("\n"); exit(1);
		}
		if(kill(childpid[1],SIGUSR1)<0){
			perror("\n");
			exit(1);
		}
		printf("\nMi blocco\n");
		//qui andrebbero bloccati tutti i segnali tranne sigusr1 per evitare che dalla pause si esca per altri motivi		
		pause();
	}
}


else if(myid==1){
	int nletto;
	close(pipe2[1]);
	close(pipe1[0]);
	close(pipe1[1]);

	printf("\nSono lettore,ricevo da padre pid altro figlio\n"); fflush(stdout);
	
	if(read(pipe2[0],childpid,sizeof(int))<0){
		perror("\n"); exit(1);
	}
	
	
	

	for(i=0;i<n;i++){
		//qui andrebbero bloccati tutti i segnali tranne sigusr1 per evitare che dalla pause si esca per altri motivi
		pause();
		//1 passo indietro perche' la sessione del file aperto e' condivisa
		lseek(fdFile,-sizeof(int),SEEK_CUR); 
		printf("\nLeggo...\n"); fflush(stdout);
		if(read(fdFile,&nletto,sizeof(int))<0){
			perror("\n"); exit(1);
		}
		printf("\nSono lettore e ho letto %d\n",nletto);
		fflush(stdout);
		if(kill(childpid[1],SIGUSR1)<0){
			perror("\n");
			exit(1);
		}
		
		printf("\nMi blocco\n");
		fflush(stdout);
	}

}





return 0;
}

int sigusrhandler(int sign){
if(myid==1){
	printf("Sono il lettore e sono stato sbloccato!\n");
	
}	
else if(myid==0)
	printf("\nSono lo scrittore e sono stato sbloccato\n");

}

