#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#define M_SIZE 3


int id;

void sighandler(int sign);
int main(int argc,char *argv[]){
int n;
printf("\ninserisci numero matrici!\n"); 
scanf("%d",&n);
while(getchar()!='\n');

if(signal(SIGUSR1,sighandler)==SIG_ERR){
	perror("\n");
	exit(1);
}

int ***m=(int ***)calloc(n,sizeof(int **));
int i,j,k;

int fdPipe[2];
if(pipe(fdPipe)!=0){
	perror("\n");
	exit(1);
}

for(i=0;i<n;i++){
	printf("\n\n");
	*(m+i)=(int **)calloc(M_SIZE,sizeof(int *));
	for(j=0;j<M_SIZE;j++){
		*(*(m+i)+j)=(int *)calloc(M_SIZE,sizeof(int));
		for(k=0;k<M_SIZE;k++){
			*(*(*(m+i)+j)+k)=i*j+k;
			printf("%d ",m[i][j][k]);		
		}		
		printf("\n");
	
	}
	
}


int *tid=(int *)calloc(n,sizeof(int));

for(id=0;id<n;id++){
	*(tid+id)=fork();
	if(*(tid+id)==0)
		break;
}

if(id==n){
	printf("\nSono padre!\n");
	close(fdPipe[1]);
	fflush(stdout);
	int maxmax=0,buff;
	for(i=0;i<n;i++){
		sleep(2);
		if(kill(tid[i],SIGUSR1)!=0){
			perror("\n");
			exit(1);		
		}	
		if(read(fdPipe[0],&buff,sizeof(int))<0){
			perror("\n");
			exit(1);		
		}
		printf("\nSono padre: ricevuto %d da figlio %d\n",buff,i);
		if(buff>maxmax)
			maxmax=buff;
		fflush(stdout);
		if(waitpid(tid[i],NULL,0)<0){
			perror("\n");
			exit(1);		
		}	
		
	}
	
	printf("\nIl max delle somme e' %d\n",maxmax);
	fflush(stdout);
}


else {
	printf("\nSono figlio %d-esimo. Dormo!!!\n",id);	
	close(fdPipe[0]);
	fflush(stdout);
	pause();
	int mySum=0;
	for(i=0;i<M_SIZE;i++)
		mySum+= *(*(*(m+id)+i)+i);
	printf("\nSono figlio %d-esimo e invio:%d\n",id,mySum);
	fflush(stdout);
	if(write(fdPipe[1],&mySum,sizeof(int))<=0){
		perror("\n");
		exit(1);
	}
	exit(0);

}




}



void sighandler(int sign){

printf("\nSono figlio %d, sbloccato!\n",id);

}
