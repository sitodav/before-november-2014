#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>






int main(int argc,char *argv[]){
if(argc!=2){
	printf("uso: passami directory\n");
	exit(0);
}


char *directory=(char *)calloc(FILENAME_MAX,sizeof(char));
strcpy(directory,argv[1]);


DIR* dirStream;
struct stat info;
struct dirent *query;


if((dirStream=opendir(directory))==NULL){
	perror("errore 30\n");
	exit(1);
}

char *fullPathBuff=(char *)calloc(FILENAME_MAX,sizeof(char)),*fullPathFirst=calloc(FILENAME_MAX,sizeof(char)),*fullPathSecond=calloc(FILENAME_MAX,sizeof(char));
int sizeFirst=0,sizeSecond=0;

while((query=readdir(dirStream))!=NULL){
	strcpy(fullPathBuff,directory);
	strcat(fullPathBuff,"/");
	strcat(fullPathBuff,query->d_name);
	
	if(lstat(fullPathBuff,&info)!=0){
		perror("err 43\n");
		exit(1);
	
	}
	
	if(S_ISLNK(info.st_mode)){
		printf("\n%s e' un link simbolico...\n",fullPathBuff);
		char *realFullPath=calloc(FILENAME_MAX,sizeof(char));
		readlink(fullPathBuff,realFullPath,FILENAME_MAX);
		printf("...che punta a %s...\n",realFullPath);
		fflush(stdout);
		if(strcmp(realFullPath+(strlen(realFullPath)-4),".txt")==0){
			printf("\n...che e' anche un file.txt\n");
			
			printf("\nReal path: %s\n",realFullPath);
			fflush(stdout);
			if(stat(realFullPath,&info)!=0){
				perror("err 58\n"); 
				exit(1);			
			}
			
			if(info.st_size>sizeFirst && info.st_size>sizeSecond){
				sizeSecond=sizeFirst;
				
				strcpy(fullPathSecond,fullPathFirst);
				strcpy(fullPathFirst,fullPathBuff);
				sizeFirst=info.st_size;
			}
			else if(info.st_size>sizeSecond){
				sizeSecond=info.st_size;
				strcpy(fullPathSecond,fullPathBuff);			
			}
		}
			
	}


}

printf("\nIl link simbolico che punta ad un file.txt la cui dimensione e' la seconda (nella dir passata da riga di comando) e'\n",fullPathSecond);
return;

}
