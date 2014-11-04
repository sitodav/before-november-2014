#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>



int main(int argc,char *argv[]){



char *path="file.txt";

int fdFileR;

if((fdFileR=open(path,O_RDONLY,0))<0){
	perror("\nerr 21\n");
	exit(1);
}

char *whole=(char *)calloc(2000,sizeof(char));

while(read(fdFileR,whole+strlen(whole),10)>0);

printf("\nIl contenuto del file e' \n%s\n",whole);
fflush(stdout);

char *wd=calloc(FILENAME_MAX,sizeof(char));
getcwd(wd,FILENAME_MAX);

DIR * dirStream;
if((dirStream=opendir(wd))==NULL){
	perror("\nerrore 37\n");
	exit(1);

}

struct dirent *query;
struct stat info;
char *pathCompleto=calloc(FILENAME_MAX,sizeof(char));
while((query=readdir(dirStream))!=NULL){
	if(!strcmp((query->d_name)+strlen(query->d_name)-4,".txt")){
		if(strcmp(query->d_name,path)==0)
			continue;		
		printf("\nTrovato file con estensione .txt\n");
		
		fflush(stdout);
		strcpy(pathCompleto,wd);
		strcat(pathCompleto,"/");
		strcat(pathCompleto,query->d_name);
		if(stat(pathCompleto,&info)!=0){
			perror("\nerrore 53\n");
			exit(1);		
		}

		if( info.st_mode & S_IWUSR   ){
			printf("\nL'utente ha anche permesso di scrittura sul file!\n");	
			fflush(stdout);
			printf("\nQuindi il file dove trascriviamo e' :%s\n",pathCompleto);
			
			int fdFileW;
			if((fdFileW=open(pathCompleto,O_WRONLY,0))<0){
				perror("\nerrore 64\n");
				exit(1);			
			}	
			int nWrite;
			if((nWrite=write(fdFileW,whole,strlen(whole)+1))<=0){
				perror("\nerrore 68\n");
				exit(1);
			}		
			printf("\nTrascrizione completata con successo (%d byte trascritti)!\n",nWrite*sizeof(char));
			close(fdFileR);
			
			exit(0);	
				
		}
	}
	

}

printf("\nNon sono stati trovati file adatti su cui trascrivere!\n");
fflush(stdout);
return;

}
