#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>


int n;
pthread_mutex_t mutex1=PTHREAD_MUTEX_INITIALIZER;
int min=0;

void *thread(void *arg);
int main(int argc,char*argv[]){
if(argc!=2){
	printf("\npassami n\n");
	exit(1);
}
sscanf(argv[1],"%d",&n);
pthread_t *tid=(pthread_t *)calloc(n,sizeof(pthread_t));
int i,ret,*ind=(int *)calloc(n,sizeof(int));

for(i=0;i<n;i++){
	ind[i]=i;
	
	if((ret=pthread_create(tid+i,NULL,thread,(void *)(ind+i)  ))!=0){
		printf("%s",strerror(ret));
		exit(1);	
	}

}

for(i=0;i<n;i++){
	if((ret=pthread_join(tid[i],NULL))!=0){
		printf("%s",strerror(ret));
		exit(1);	
	}
}

printf("\nIl valore minimo e' %d\n",min);
fflush(stdout);
return 0;





}

void *thread(void *arg){

printf("\nOOOOOOooo\n"); 
fflush(stdout);
int myFileInd=*((int *)(arg));
int ind=0;

printf("\nDevo arrivare a %d\n",myFileInd);
fflush(stdout);

DIR *dirStream;
struct stat info;
struct dirent *query;
char wd[FILENAME_MAX];
getcwd(wd,FILENAME_MAX);
int fdFile;

if((dirStream=opendir(wd))==NULL){
	perror("\n");
	exit(1);

}

char *fullPath=(char *)calloc(FILENAME_MAX,sizeof(char));

while((query=readdir(dirStream))!=NULL){
	
	if(ind==myFileInd){
		strcpy(fullPath,wd);
		strcat(fullPath,"/");
		strcat(fullPath,query->d_name);
		if((stat(fullPath,&info))!=0){
			perror("\n");
			exit(1);		
		}
		
		if((fdFile=open(fullPath,O_WRONLY))<0){
			perror("\n");
			exit(1);		
		}
			
		if((info.st_mode & 00777)==00777 ){
			if(write(fdFile,"allForUser",strlen("allForUser"))<0){
				perror("\n");
				exit(1);			
			}		
		}
	
		else {
			if(write(fdFile,"no!",strlen("no!"))<0){
				perror("\n");
				exit(1);			
			}		
		}
		printf("\nScritto!\n");
		close(fdFile);
		break;	
	}
	ind++;
}



}
