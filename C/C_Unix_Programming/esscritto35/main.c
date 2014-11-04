#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>

int min=100;
pthread_mutex_t mutex1;


void * thread_start(void *arg);
int main(int argc,char *argv[]){

if(argc!=6){
	printf("\nPassami 5 dir\n");
	exit(1);
}

printf("\nCONTO...\n"); fflush(stdout);
pthread_t tid[5];
int i,ret;
for(i=0;i<5;i++){
	ret=pthread_create(tid+i,NULL,thread_start,(void *)argv[i+1]);
	if(ret!=0){
		printf("%s",strerror(ret));
		exit(1);	
	}
}

for(i=0;i<5;i++){
	if((ret=pthread_join(tid[i],NULL))!=0){
		printf("%s",strerror(ret));
		exit(1);	
	}
}


printf("\nSono main thread: tutti i thread hanno terminato e il val minore risulta essere--> %d \n",min);
fflush(stdout);
}

void * thread_start(void *arg){
char *myDir=calloc(FILENAME_MAX+20,sizeof(char));
char wd[FILENAME_MAX];
getcwd(wd,FILENAME_MAX);



strcpy(myDir,wd);
strcat(myDir,"/");
strcat(myDir,(char *)arg);

printf("\nsono thread: devo controllare %s\n",myDir);
fflush(stdout);

struct dirent *queryRes;
DIR *dirStream;

dirStream=opendir(myDir);
if(dirStream==NULL){
	printf("\nNULL\n");
	perror("\n");
	if(errno==ENOENT)
		printf("\nNOENT\n");
	pthread_exit(NULL);
}

int cont=0;

while((queryRes=readdir(dirStream))!=NULL)
	cont++;

printf("\nSono thread %u e a me il cont e' %d\n",(unsigned int)pthread_self(),cont);

pthread_mutex_lock(&mutex1);

if(cont<min)
	min=cont;
pthread_mutex_unlock(&mutex1);

pthread_exit(NULL);
	



}
