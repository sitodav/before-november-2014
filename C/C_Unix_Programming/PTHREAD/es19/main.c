/*Scrivere un programma in C che esamini il contenuto della directory corrente, verificando per ogni file
regolare se la sua estensione è .c. In questo caso, verificarne i permessi di scrittura per il gruppo e
visualizzare sullo standard output il contenuto del secondo file di lunghezza minore.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>

int main(int argc,char *argv[]){

int max=0,max2=0;
char nomeSecondo[FILENAME_MAX];
struct stat info;
struct dirent *queryRes;
DIR *dirStream;
char curDir[FILENAME_MAX];

getcwd(curDir,FILENAME_MAX);

if((dirStream=opendir(curDir))==NULL){
	perror("\n");
	exit(1);
}

while((queryRes=readdir(dirStream))!=NULL){
	printf("\nTrovato file %s\n",queryRes->d_name);
	printf("\nAnalizzo...\n"); fflush(stdout);
	if(lstat(queryRes->d_name,&info)<0){
		perror("\n");
		exit(1);	
	}
	if(S_ISREG(info.st_mode)){
		printf("\nil file %s e' regolare\n",queryRes->d_name);
		fflush(stdout);
	 	//se il file aperto con la readdir fosse un softlink, dovremmo andare ad aprire il vero file puntato per vedere se e' un punto.c tuttavia non e' richiesto dal testo
		if(strcmp(queryRes->d_name+strlen(queryRes->d_name)-2,".c")==0){
			printf("\nIl file e' anche punto c\n");
			if(info.st_size>max)
				max=info.st_size;
			else if(info.st_size>max2){
				max2=info.st_size;
				strcpy(nomeSecondo,queryRes->d_name);
				printf("\nIl file %s risulta essere il secondo piu' grande fino ad ora!\n",nomeSecondo); fflush(stdout);
				
							
			}
			if(!access(queryRes->d_name,R_OK)){
				printf("\nIl file ha permessi di lettura per il gruppo!\n");
				fflush(stdout);			
			}
			else 
				printf("\nIl file non ha permessi di lettura per il gruppo\n");
			
				
					
		}		
			
	}


}

printf("\nAbbiamo analizzato tutti i file nella directory. Il file punto.c che ha come dimensione la seconda piu' grande  e' %s\n",nomeSecondo); fflush(stdout);

printf("\nIl contenuto di %s e' \n",nomeSecondo); 

int fdFileR;
if((fdFileR=open(nomeSecondo,O_RDONLY))<0){
	perror("\n");
	exit(1);
}
char toRead[100];
int ret;
while((ret=read(fdFileR,toRead,strlen(toRead)))>0){
	printf("%s",toRead);
	fflush(stdout);
}


}
