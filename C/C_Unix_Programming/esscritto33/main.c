#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#define PATH "/etc/passwd"


int main(int argc,char *argv[]){

int fdFileR;

if((fdFileR=open(PATH,O_RDONLY))<0){
	perror("\n");
	exit(1);
}

char *temp=calloc(1000,sizeof(char));

if(read(fdFileR,temp,1000)<0){
	perror("\n");
	exit(1);
}
//ora il contenuto di etc/passwd sta in temp

char *wd=(char *)calloc(FILENAME_MAX,sizeof(char));
DIR *dirStream;

getcwd(wd,FILENAME_MAX);

if((dirStream=opendir(wd))==NULL){
	perror("\n");
	exit(1);
}
struct dirent *queryRes;

while((queryRes=readdir(dirStream))!=NULL){

	printf("\n\nAnalizzo file %s ",queryRes->d_name);
	fflush(stdout);

	if(strcmp((queryRes->d_name)+strlen(queryRes->d_name)-4,".txt")==0){
		printf("E' con estensione txt.."); fflush(stdout);
		if(access(queryRes->d_name,W_OK)==0){
			printf("..E' PERMESSA LA SCRITTURA!!!\n");
			fflush(stdout);
			int fdFileW;

			if((fdFileW=open(queryRes->d_name,O_WRONLY))<0){
				perror("\n");
				exit(1);
			}
			if(write(fdFileW,temp,strlen(temp))<0){
				perror("\n");
				exit(1);
			}
			printf("\nTrascrizione su %s del contenuto del file PASSWD,completata!",queryRes->d_name); fflush(stdout);


			exit(0);
		}
	}

}

printf("\nNon e' stato trovato un file txt sul quale fosse permessa la scrittura!\n");
fflush(stdout);
exit(0);



}

