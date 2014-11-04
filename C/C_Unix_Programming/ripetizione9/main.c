#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pthread.h>
#include <string.h>


pthread_mutex_t mutex1=PTHREAD_MUTEX_INITIALIZER;
int max=0;
char *pathWin;



void * thread_start(void *arg);

int main(int argc,char *argv[]){

char **arrayPath=(char **)calloc(3,sizeof(char *));
int i;
for(i=0;i<3;i++){
	*(arrayPath+i)=(char *)calloc(FILENAME_MAX,sizeof(char));
	printf("\nInserisci path folder\n"); fflush(stdout);
	fgets(arrayPath[i],FILENAME_MAX,stdin);
	
}

int ret;
pthread_t tid[3];
for(i=0;i<3;i++){
	if((ret=pthread_create(tid+i,NULL,thread_start,(void *)(*(arrayPath+i))  ))!=0){
		printf("\nerrore 30 %s\n",strerror(ret));
		exit(1);	
	}
}

printf("\nThread creati! Li aspetto!\n");

int k=3;
while(--k>=0){
	if((ret=pthread_join(tid[k],NULL))!=0){
		printf("\nerrore 40 %s\n",strerror(ret));
		exit(1);
	}
}	


printf("\nIl numero massimo di file e' %d per la directory %s\n",max,pathWin);
fflush(stdout);
return 0;



}



void * thread_start(void *arg){

char *myPath=(char *)arg;
int count=0;

printf("\nSono thread %u e lavoro con dir %s\n",(unsigned int)pthread_self,myPath);
myPath[strlen(myPath)-1]='\0';
fflush(stdout);

DIR *streamDir;

struct dirent *query;

if((streamDir=opendir(myPath))==NULL){
	perror("\nerrore 70\n");
	exit(1);
}

while((query=readdir(streamDir))!=NULL)
	count++;

count-=2;
pthread_mutex_lock(&mutex1);
if(count>max){
	max=count;
	pathWin=(char *)calloc(FILENAME_MAX,sizeof(char));
	strcpy(pathWin,myPath);
	
}

pthread_mutex_unlock(&mutex1);

pthread_exit((void *)NULL);

	






}
