#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>


pthread_mutex_t mutex1=PTHREAD_MUTEX_INITIALIZER;
int max=0;


void *thread_start(void *);
int main(int argc,char *argv[]){

char **dir=(char **)calloc(3,sizeof(char *));
int i;

for(i=0;i<3;i++){
	*(dir+i)=(char *)calloc(FILENAME_MAX,sizeof(char));
	printf("\nInserisci dir\n"); fflush(stdout);
	fgets(*(dir+i),FILENAME_MAX,stdin);
	*(*(dir+i)+strlen(*(dir+i))-1)='\0';
}

pthread_t *tid=(pthread_t *)calloc(3,sizeof(pthread_t));

int ret;
for(i=0;i<3;i++){
	if((ret=pthread_create(tid+i,NULL,thread_start,(void *)(*(dir+i)) ))!=0){
		printf("\n%s",strerror(ret));
		exit(1);	
	}

}

char *p;

for(i=0;i<3;i++){
	if((ret=pthread_join(tid[i],(void **)&p ))!=0){
		printf("%s",strerror(ret));
		exit(1);
	}
	printf("\nIl thread e' ritornato con %s\n",p);
	fflush(stdout);

}

printf("\nIl max risulta %d\n",max);
fflush(stdout);


free(tid);
free(dir[0]); free(dir[1]); free(dir[2]);



return 0;


}

void *thread_start(void *arg){

char *myString=(char *)(arg);
printf("\nSono thread  e devo lavorare con %s\n",myString);
fflush(stdout);

DIR *dirStream;
struct stat info;
struct dirent *query;

if((dirStream=opendir(myString))==NULL){
	perror("\n");
	exit(1);
}

int cont=0;

while((query=readdir(dirStream))!=NULL)
	cont++;

pthread_mutex_lock(&mutex1);

if(cont>max)
	max=cont;

pthread_mutex_unlock(&mutex1);


pthread_exit((void *)myString);

}
