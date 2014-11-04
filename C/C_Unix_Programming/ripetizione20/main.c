#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>


int nMax=0;

pthread_mutex_t mutex1=PTHREAD_MUTEX_INITIALIZER;
char *maxPath;

void * thread_start(void *arg);
int main(int argc,char *argv[]){
maxPath=(char *)calloc(FILENAME_MAX,sizeof(char));
if(argc!=2){
	printf("\nPassami n\n");
	exit(1);
}

int i,n;
sscanf(argv[1],"%d",&n);
char **aStringhe=(char **)calloc(n,sizeof(char *));
for(i=0;i<n;i++){
	*(aStringhe+i)=(char *)calloc(FILENAME_MAX,sizeof(char));
	printf("\nInserisci path cartella\n"); fflush(stdout);
	fgets(*(aStringhe+i),FILENAME_MAX,stdin);
	*(*(aStringhe+i)+strlen(*(aStringhe+i))-1)='\0';
}

pthread_t *tid=(pthread_t *)calloc(n,sizeof(pthread_t));
int ret;
for(i=0;i<n;i++){
	if((ret=pthread_create(tid+i,NULL,thread_start,(void *)(*(aStringhe+i))  ))!=0){
		printf("%s",strerror(ret));
		exit(1);
	}

}
	
for(i=0;i<n;i++){
	if( ( ret=pthread_join(tid[i],NULL))!=0){
		
		printf("%s",strerror(ret));
		exit(1);
	}
}


printf("\nThread figli terminati! Sono main thread e la dir %s ha %d file\n",maxPath,nMax);
fflush(stdout);
return;
}


void * thread_start(void *arg){
char *myDir=(char *)(arg);
printf("\nSono thread e lavoro con %s\n",myDir);
fflush(stdout);

DIR *dirStream;
struct stat info;
struct dirent *query;

if((dirStream=opendir(myDir))==NULL){
	perror("\n");
	exit(1);
}

int myCount=0;
char *fullPath=(char *)calloc(FILENAME_MAX,sizeof(char));


while((query=readdir(dirStream))!=NULL){
	strcpy(fullPath,myDir);
	strcat(fullPath,"/");
	strcat(fullPath,query->d_name);
	if(lstat(fullPath,&info)!=0){
		perror("\n");
		exit(1);	
	}	
	if(S_ISREG(info.st_mode)){
		myCount++;		
		printf("\n%s e' un file regolare! (myCount:%d)\n",fullPath,myCount);
		fflush(stdout);
			
	}
	
	

}

pthread_mutex_lock(&mutex1);
if(myCount>nMax){
	nMax=myCount;
	strcpy(maxPath,myDir);
}
pthread_mutex_unlock(&mutex1);

pthread_exit(NULL);





}
