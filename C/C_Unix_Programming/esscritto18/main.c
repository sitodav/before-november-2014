/*Scrivere un programma in C che esamini il contenuto della directory corrente, verificando per ogni file
regolare se la sua estensione è .c. In questo caso, verificarne i permessi di scrittura per il gruppo e
visualizzare sullo standard output il contenuto del secondo file di lunghezza minore.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>



int main(int argc,char *argv[]){

char wd[FILENAME_MAX];
getcwd(wd,FILENAME_MAX);

DIR *wdStream;
if((wdStream=opendir(wd))==NULL){
	perror("\n");
	exit(1);

}

int max=0,max2=0;
char max2name[FILENAME_MAX];
struct dirent *query;
struct stat info;
while((query=readdir(wdStream))!=NULL){
	if(strcmp(query->d_name+strlen(query->d_name)-2,".c")==0){
		printf("\nTrovato file con estensione .c -> %s\n",query->d_name);

		if(stat(query->d_name,&info)<0){
			perror("\n");
			exit(1);	
		}
		
		if(S_ISREG(info.st_mode)){
			printf("\nIl file %s e' anche un file REGOLARE!\n",query->d_name); fflush(stdout);
		}
		if(info.st_size>max){
			max=info.st_size;
		}
		else if(info.st_size>max2){
			max2=info.st_size;
			strcpy(max2name,query->d_name);		
		}
		
		if((info.st_mode & S_IWOTH)==1) //SBAGLIATO, RECUPERA COME SI FA!!!!
			printf("\nIl file e' accessibile in scrittura dal gruppo\n");
		
	}
	

}

printf("\nIl file regolare e .c che risulta avere seconda dimensione massima e' %s con dimensione di %d\n",max2name,max2);
fflush(stdout);




return 0;
}
