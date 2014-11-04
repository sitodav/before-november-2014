#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#define psw "/etc/passwd"
#include <dirent.h>



int main(int argc,char *argv[]){

struct dirent *queryRes;
DIR *wdStream;

int fdFile,fdFileW;
char *buff;
char wd[FILENAME_MAX]; getcwd(wd,FILENAME_MAX);

if((fdFile=open(psw,O_RDONLY))<0){
	perror("\n");
	exit(1);
}



buff=(char *)calloc(1000,sizeof(char));
if(read(fdFile,buff,1000)<0){
	perror("\n");
	exit(1);
}

	
if((wdStream=opendir(wd))==NULL){
	perror("\n");
	exit(1);
}

while((queryRes=readdir(wdStream))!=NULL){
	char temp[FILENAME_MAX];
	int j=strlen(queryRes->d_name)-1;
	strcpy(temp,queryRes->d_name+(j-3));
	
	if(strcmp(".txt",temp)==0){
		printf("\nTrovato file testuale in curr work dir\n");
		if(access(queryRes->d_name,W_OK)==0){
			printf("\nAbbiamo diritti di scrittura sul file %s\n",queryRes->d_name);
			fflush(stdout);
			if((fdFileW=open(queryRes->d_name,O_WRONLY))<0){
				perror("\n");
				exit(1);			
			}
			
			if(write(fdFileW,buff,strlen(buff))<0){
				perror("\n");
				exit(1);			
			}
			
			printf("\nScrittura completata con successo\n");
			close(fdFileW);
			close(fdFile);
			return 0;
		}
		
	}
	


}

printf("\nNon esiste nella cwd nessun file \".txt\" sul quale abbiamo diritti di scrittura\n");
fflush(stdout);
return 0;







}
