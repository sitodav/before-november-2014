#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#define LOCKED 1
#define UNLOCKED 0




void SIGUSR1handler(int sign);
int lock=LOCKED;
int main(int argc,char *argv[]){

if(signal(SIGUSR1,SIGUSR1handler)==SIG_ERR){
	perror("\n");
	exit(1);

}

if(argc!=2){
	printf("\n uso: n\n"); 
	exit(1);
}

int n,fdFile,childpid,i;
sscanf(argv[1],"%d",&n);
if(n>25 || n<7){
	printf("\nIl numero deve essere in [7,25]!\n");
	exit(0);
}


if((fdFile=open("temp.txt",O_RDWR | O_CREAT,00777))<0){
	perror("\n");
	exit(1);
}


childpid=fork();
if(childpid==0){ //figlio
	int t;
	for(i=0;i<n;i++){
		while(lock==LOCKED);
		if(lseek(fdFile,3,SEEK_SET)!=3){
			perror("\n");
			exit(1);	
		}
		if(read(fdFile,&t,sizeof(int))<0){
			perror("\n");
			exit(1);	
		}
		printf("\nSono figlio, ho letto %d\n",t);
		if(kill(getppid(),SIGUSR1)<0){
			perror("\n");
			exit(1);		
		}
		lock=LOCKED;
	

	}
	


}

else{ //padre
lock=UNLOCKED;
	for(i=1;i<=n;i++){
		while(lock==LOCKED);
		if(lseek(fdFile,3,SEEK_SET)!=3){
			perror("\n");
			exit(1);	
		}
		if(write(fdFile,&i,sizeof(int))<0){
			perror("\n");
			exit(1);	
		}
		if(kill(childpid,SIGUSR1)<0){
			perror("\n");
			exit(1);	
		}
		lock=LOCKED;


	}


}

}

void SIGUSR1handler(int sign){
lock=UNLOCKED;
}
