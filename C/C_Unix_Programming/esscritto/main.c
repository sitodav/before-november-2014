#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#define file "file1.txt"

void SIGUSR1handler(int sign);
void SIGUSR2handler(int sign);
int main(int argc,char *argv[]){
if(argc==1){
	printf("\nuso: <this> <n>\n");
	exit(0);
}

int n,i; sscanf(argv[1],"%d",&n);

int childpid,fdFile;

if(signal(SIGUSR1,SIGUSR1handler)==SIG_ERR){
	perror("\n"); exit(1);
}


if(signal(SIGUSR2,SIGUSR2handler)==SIG_ERR){
	perror("\n"); exit(1);
}
if((fdFile=open(file,O_CREAT | O_RDWR,0777))<0){
	perror("\n");
	exit(1);

}

printf("\nFile aperto\n"); fflush(stdout);
childpid=fork();
if(childpid>0){ //padre
	printf("\nSono il padre\n");
	
	for(i=1;i<=n;i++){
		printf("\npos corrente:%d\n",lseek(fdFile,0,SEEK_SET)); fflush(stdout);
		if(write(fdFile,&i	,sizeof(int))<0){
			perror("\n");
			exit(1);		
		}	
		printf("\nSono il padre: scritto %d, sveglio il figlio e mi fermo!\n",i); fflush(stdout);
		if(kill(childpid,SIGUSR2)<0){
			perror("\n");
			exit(1);		
		}	
		pause();	

		

	
	
	}
close(fdFile);

}

else if(childpid==0){
	int nbuff;
	printf("\nSono il figlio\n"); fflush(stdout);
	for(i=1;i<=n;i++){
		pause();
		
		printf("\npos corrente:%d\n",lseek(fdFile,0,SEEK_SET)); fflush(stdout);
		if(read(fdFile,&nbuff,sizeof(int))<0){
			perror("\n");
			exit(1);		
		}
		printf("\nSono il figlio, letto %d,sveglio il padre e mi fermo\n",nbuff);
		fflush(stdout);		
		if(kill(getppid(),SIGUSR1)<0){
			perror("\n");
			exit(1);		
		}	
	}
close(fdFile);
	
}

else{
	perror("\n");
	exit(1);
}



}


void SIGUSR1handler(int sign){
printf("\nSono il padre, e sono stato sbloccato\n");
fflush(stdout);

}
void SIGUSR2handler(int sign){
printf("\nSono il figlio, e sono stato sbloccato\n");
fflush(stdout);
}
