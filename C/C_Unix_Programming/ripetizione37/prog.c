#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>


int max=0;
pthread_mutex_t mutex1=PTHREAD_MUTEX_INITIALIZER;



void *thread_start(void *arg);
int main(int argc,char *argv[]){

int nDir=argc-1,i,ret;
pthread_t *tid=calloc(nDir,sizeof(pthread_t));





for(i=0;i<nDir;i++){
	if((ret=pthread_create(tid+i,NULL,thread_start,(void *)(*(argv+i+1))  ))!=0){
		printf("%s",strerror(ret));
		exit(1);	
	}

}


for(i=0;i<nDir;i++){
	if((ret=pthread_join(tid[i],NULL))!=0){
		printf("%s",strerror(ret));
		exit(1);	
	}

}

printf("\nIl valore di max e' %d\n",max);
fflush(stdout);


}


void *thread_start(void *arg){

char *myPath=(char *)(arg);


printf("%s",myPath);
fflush(stdout);
int count=0;

DIR *dirStream;
struct stat info;
struct dirent *query;

if((dirStream=opendir(myPath))==NULL){
	perror("\n");
	exit(1);

}

char *fullPath=(char *)calloc(FILENAME_MAX,sizeof(char));

while((query=readdir(dirStream))!=NULL){
	strcpy(fullPath,myPath);
	strcat(fullPath,"/");
	strcat(fullPath,query->d_name);
	if(stat(fullPath,&info)!=0){
		perror("\n");
		exit(1);	
	}
	if(S_ISREG(info.st_mode))
		count++;
	

}

pthread_mutex_lock(&mutex1);
if(count>max)
	max=count;
pthread_mutex_unlock(&mutex1);

pthread_exit(NULL);





}
