#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>


int main(int argc,char *argv[]){

if(argc!=2){
	printf("\npassami dir\n");
	exit(1);
}
char *wd=argv[1];

DIR *dirStream;
struct stat info;
struct dirent *query;
char *fullPath;

if((dirStream=opendir(wd))==NULL){
	perror("\n");
	exit(1);
}

while((query=readdir(dirStream))!=NULL){
	fullPath=(char *)calloc(FILENAME_MAX,sizeof(char));
	strcpy(fullPath,wd);
	strcat(fullPath,"/");
	strcat(fullPath,query->d_name);
	if(lstat(fullPath,&info)!=0){
		perror("\n");
		exit(1);	
	}

	if(S_ISREG(info.st_mode)){
		printf("\n%s e' un file regolare!\n",fullPath);
		printf("\nsize->%d",info.st_size);
		if(info.st_mode & S_IWUSR)
			printf("\nil propr puo' scriverci!\n");
		if(info.st_mode & S_IRUSR)
			printf("\nIl proprietario puo' leggerci!\n");
		if(info.st_mode & S_IXGRP)
			printf("\nIl gruppo puo' eseguirlo!\n");
		if(info.st_mode & S_IWGRP)
			printf("\nIl gruppo puo' scriverci!\n");
		fflush(stdout);
		if(strcmp(query->d_name,"main.c")==0 || strcmp(query->d_name,"main")==0)
			continue;
		printf("\nLO CANCELLO!\n"); fflush(stdout);
		if(unlink(fullPath)!=0){
			perror("\n");
			exit(1);		
		}	
	}
}

}
