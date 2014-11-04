#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <time.h>


void sigusr1handler(int sign);
int main(int argc,char *argv[]){
if(argc!=2){
	printf("\npassami n\n");
	exit(1);
}


if(signal(SIGUSR1,sigusr1handler)==SIG_ERR){
	perror("\n");
	exit(1);
}
int n;
sscanf(argv[1],"%d",&n);
srand((unsigned int)time(NULL));


int childpid[2],i;
int temp,temp2;
int fdPipe[2],fdPipe2[2];

if(pipe(fdPipe)!=0 || pipe(fdPipe2)!=0){
	perror("\n");
	exit(1);
}

for(i=0;i<2;i++){
	childpid[i]=fork();
	if(childpid[i]==0)
		break;
}

int ind,*vector;

if(i==2){
	printf("\nSono padre!\n");
	fflush(stdout);
	close(fdPipe[0]);
	close(fdPipe2[1]);
	ind=0;
	while(ind<n){
		sleep(1);
		if(kill(childpid[0],SIGUSR1)!=0){
			perror("\n");	
		}
		if(read(fdPipe2[0],&temp,sizeof(int))<0){
			perror("\n");
			exit(1);	
		}
		sleep(1);
		if(kill(childpid[1],SIGUSR1)!=0){
			perror("\n");	
		}
		if(read(fdPipe2[0],&temp2,sizeof(int))<0){
			perror("\n");
			exit(1);	
		}
		sleep(1);
		if(kill(childpid[0],SIGUSR1)!=0){
			perror("\n");	
		}
		if(write(fdPipe[1],&temp2,sizeof(temp2))<=0){
			perror("\n");
			exit(1);	
		}
		sleep(1);
		if(kill(childpid[1],SIGUSR1)!=0){
			perror("\n");	
		}
		if(write(fdPipe[1],&temp,sizeof(temp))<=0){
			perror("\n");
			exit(1);	
		}
	
		ind++;
	}




}


else{
	
	fflush(stdout);
	close(fdPipe[1]);
	close(fdPipe2[0]);
	vector=(int *)calloc(n,sizeof(int));
	for(ind=0;ind<n;ind++){
		*(vector+ind)=i*123;
		if(i==0)
			break;
		printf("%d ",vector[i]);
		fflush(stdout);	
	}
	ind=0;
	while(ind<n){
		pause();
		if(write(fdPipe2[1],vector+ind,sizeof(vector[ind]))<=0){
			perror("oooo\n");
			exit(1);		
		}
		pause();
		if(read(fdPipe[0],vector+ind,sizeof(int))<0){
			perror("aaaaa\n");
			exit(1);		
		}
		ind++;	
	}
	for(ind=0;ind<n;ind++){
		if(i==0)
			break;
		printf("%d ",vector[ind]);
		fflush(stdout);
	}






}

return;



}



void sigusr1handler(int sign){
return;


}
