#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pthread.h>


void *thread_start(void *arg);

int main(int argc,char *argv[]){
char wd[FILENAME_MAX];
getcwd(wd,sizeof(wd));

printf("\nLa dir corrente e' %s\n",wd);
printf("\nLa passo al thread di elaborazione\n");

pthread_t tid;
int ret;

if((ret=pthread_create(&tid,NULL,thread_start,(void *)wd))!=0){
	printf("\n25: %s\n",strerror(ret));
	exit(1);
}

printf("\nAspetto thread\n");
if((ret=pthread_join(tid,NULL))!=0){
	printf("31:%s\n",strerror(ret));
	exit(1);
}

printf("\nThread figlio terminato. Termino anche io main thread!\n");
return 0;

}

void *thread_start(void *arg){
char *wd=(char *)(arg);
printf("\nSono thread figlio, devo lavorare su dir->%s\n",wd);
fflush(stdout);

DIR *dirStream;
struct dirent *query;
struct stat info;

if((dirStream=opendir(wd))==NULL){
	perror("50 ");
	exit(1);
}

char *fullPathFirst=calloc(FILENAME_MAX,sizeof(char));
int sizeFirst=0;
char *fullPathSecond=calloc(FILENAME_MAX,sizeof(char));
int sizeSecond=0;
char *fullPathBuff=calloc(FILENAME_MAX,sizeof(char));

while( (query=readdir(dirStream))!=NULL ){
	printf("\nTrovato file: %s\n",query->d_name); fflush(stdout);
	if(strcmp((query->d_name)+strlen(query->d_name)-2,".c")==0){
		printf("\nIl file ha estensione .c\n"); fflush(stdout);
		
		
		strcpy(fullPathBuff,wd);
		strcat(fullPathBuff,"/");
		strcat(fullPathBuff,query->d_name);
		
		if(stat(fullPathBuff,&info)<0){
			perror("\n");
			exit(1);		
		}
		
		if(S_ISREG(info.st_mode)){
			printf("\nIl file e' regolare!\n"); fflush(stdout);		
		}
		
		if(info.st_mode & S_IWGRP){
			printf("\nIl gruppo ha permessi in scrittura sul file!\n");
			fflush(stdout);		
		}

		if(info.st_size >sizeFirst && info.st_size>sizeSecond){
			sizeSecond=sizeFirst;
			free(fullPathSecond);
			fullPathSecond=calloc(FILENAME_MAX,sizeof(char));
			strcpy(fullPathSecond,fullPathFirst);
			free(fullPathFirst);
			fullPathFirst=calloc(FILENAME_MAX,sizeof(char));
			strcpy(fullPathFirst,fullPathBuff);
			sizeFirst=info.st_size;		
		}
		else if(info.st_size<sizeFirst && info.st_size>sizeSecond){
			sizeSecond=info.st_size;
			free(fullPathSecond);
			fullPathSecond=calloc(FILENAME_MAX,sizeof(char));
			strcpy(fullPathSecond,fullPathBuff);		
		}
		
		
	
	}

	else printf("\nIl file non ha estensione .c\n");
	

}
	

printf("\nIl file.c e regolare che ha il size maggiore e' %s (%d Bytes)\n",fullPathFirst,sizeFirst);
printf("\nMentre il file.c e regolare che ha il secondo size maggiore e' %s (%d Bytes)\n",fullPathSecond,sizeSecond);
fflush(stdout);

pthread_exit((void *)NULL);

}
