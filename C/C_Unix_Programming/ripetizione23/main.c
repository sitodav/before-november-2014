#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>




int main(int argc,char *argv[]){

int fdFileR;

if((fdFileR=open("/etc/passwd",O_RDONLY))<0){
	perror("\n");
	exit(1);
}
printf("\nFile aperto!\n");
char *buff=(char *)calloc(100,sizeof(char));
char *bigBuff=(char *)calloc(2000,sizeof(char));

while(read(fdFileR,buff,100)>0)
	strcpy(bigBuff+strlen(bigBuff),buff);

printf("\nLETTO:\n");
printf("%s",bigBuff);
fflush(stdout);

char *fullPath=(char *)calloc(FILENAME_MAX,sizeof(char));

DIR *dirStream;
struct stat info;
struct dirent *query;
char wd[FILENAME_MAX];

if((dirStream=opendir(getcwd(wd,FILENAME_MAX)))==NULL){
	perror("\n");
	exit(1);
}


while((query=readdir(dirStream))!=NULL){
	strcpy(fullPath,getcwd(wd,FILENAME_MAX));
	strcat(fullPath,"/");
	strcat(fullPath,query->d_name);
	if(lstat(fullPath,&info)!=0){
		perror("\n");
		exit(1);
	}
	
	if(S_ISREG(info.st_mode)){
		printf("\n%s e' un file regolare!\n",fullPath);
		fflush(stdout);
		if(strcmp(query->d_name,"main.c")==0 || strcmp(query->d_name,"main")==0 )
			continue;
		if(strcmp((query->d_name)+strlen(query->d_name)-4,".txt")==0){
			printf("\nEd e' anche un file.txt!\n");
			fflush(stdout);
		}	
		
		if(info.st_mode & 00200){
			printf("\nE ci possiamo anche scrivere!\n");
			
			int fdFileW;
			if((fdFileW=open(fullPath,O_WRONLY))<0){
				perror("ioaisdjoi\n");
				exit(1);			
			}	
			printf("%s ---%d\n",bigBuff,strlen(bigBuff)); fflush(stdout);
			if(write(fdFileW,bigBuff,strlen(bigBuff))<0){
				perror("\n");
				exit(1);			
			}
			close(fdFileW);
			printf("\nTrascrizione file pass effettuata!\n");
			exit(0);	
		}
	}
}

printf("\nNon c'e' un file.txt nella cartella corrente, sul quale si possa scrivere (no permessi) !\n");
exit(1);

}
