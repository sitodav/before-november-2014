#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <signal.h>

void SIGINThandler(int sign);
int main(int argc,char *argv[]){

if(signal(SIGINT,SIGINThandler)==SIG_ERR){
	perror("\n");
	exit(1);
}

int childpid;
int pipe1[2];

if(pipe(pipe1)<0){
	perror("\n");
	exit(1);
}

while(1){
	char dir[FILENAME_MAX];
	childpid=fork();
	if(childpid!=0){
		printf("\nInserisci nome directory\n");
		fflush(stdout);	
		fgets(dir,FILENAME_MAX,stdin);
		dir[strlen(dir)-1]='\0';
		if(write(pipe1[1],dir,strlen(dir)+1)<=0){
			perror("\n");
			exit(1);		
		}
		wait(NULL);
	}
	
	else if(childpid==0){
			int cont=0;
			DIR *dirStream;
			struct stat info;
			struct dirent *queryRes;
			char toRead[FILENAME_MAX];
			char *pathFile;
			close(pipe1[1]);
			if(read(pipe1[0],toRead,FILENAME_MAX)<0){
				perror("\n");
				exit(1);			
			}
			printf("\nSono figlio, lavoro con %s\n",toRead);
			fflush(stdout);
			
			if((dirStream=opendir(toRead))==NULL){
				perror("\n");
				exit(1);			
			}
			
			while((queryRes=readdir(dirStream))!=NULL){
				pathFile=calloc(FILENAME_MAX,sizeof(char));

				strcpy(pathFile,toRead);
				strcat(pathFile,"/");
				strcat(pathFile,queryRes->d_name);

				if(lstat(pathFile,&info)<0){
					perror("\n");				
				}
				if(S_ISREG(info.st_mode)){
					printf("\nTrovato file regolare: %s!\n",queryRes->d_name);
					cont++;
					
			
				
				}
			}
			printf("\nFIle terminati: ci sono in tot %d file regolari \n",cont);
			fflush(stdout);
			printf("\nL'ultimo file regolare trovato ha path completo\n%s",pathFile);
			if(info.st_mode & 040){
				printf("\nil file ha lettura per il gruppo!\n");				fflush(stdout);
			}
			if(info.st_mode & 004){
				printf("\nIl file ha anche lettura per gli altri\n");
				fflush(stdout);
			}
			if(info.st_mode & 010){
				printf("\nIl file ha anche scrittura per il gruppo!\n");
				fflush(stdout);
			}
			
		
			exit(0);
	
	}


}


}

void SIGINThandler(int sign){
printf("\nHo ricevuto CTRL-C sigint\n");
fflush(stdout);
exit(0);

}
