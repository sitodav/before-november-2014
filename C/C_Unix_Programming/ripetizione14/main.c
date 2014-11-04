#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>


int main(int argc,char *argv[]){



int fdPipe[2];
int fdPipe2[2];

if(pipe(fdPipe)!=0 || pipe(fdPipe2)!=0){

	perror("\n");
	exit(1);
}

int childpid;
char *buff;
childpid=fork();
if(childpid==0){
	printf("\nSono figlio!\n");
	fflush(stdout);
	close(fdPipe[1]);
	close(fdPipe2[0]);
	
	buff=(char *)calloc(100,sizeof(char));
	while(read(fdPipe[0],buff,100)!=0){
		printf("\nHo letto %s\n",buff);
		fflush(stdout);
		printf("\nRimando indietro!\n"); fflush(stdout);
		if(write(fdPipe2[1],buff,strlen(buff)+1)<=0){
			perror("\n");
			exit(1);		
		}		
			
	}
	
	
}

else {
	printf("\nSono padre!\n"); fflush(stdout);
	close(fdPipe[0]);
	close(fdPipe2[1]);
	
	while(1){
		buff=(char *)calloc(100,sizeof(char));		
		printf("\nScrivi!\n"); fflush(stdout);
		fgets(buff,100,stdin);
		if(write(fdPipe[1],buff,strlen(buff)+1)<=0){
			perror("\n");
			exit(1);		
		}
		if(read(fdPipe2[0],buff,100)<0){
			perror("\n");
			exit(1);		
		}
		printf("\nIl figlio mi ha mandato indietro %s\n",buff);
		fflush(stdout);
		
		free(buff);
	}

}


}
