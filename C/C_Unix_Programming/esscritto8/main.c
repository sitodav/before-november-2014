/*Scrivere un programma C in ambiente Linux che, ricevuto il nome di un file, lo cerchi all’interno della
directory corrente e se esso è un file regolare di dimensione maggiore di 1Kb ne faccia, eventualmente,
il taglia e incolla nella directory puntata da un link simbolico presente nella directory corrente.
*/




#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>


int main(int argc,char *argv[]){


if(argc!=2){
	perror("\n <this> <nomefile>\n");
	exit(0);
}

char wd[FILENAME_MAX];
getwd(wd);
printf("\nWD ATTUALE :%s\n",wd); fflush(stdout);

DIR *wdStream=opendir(wd);
if(wdStream==NULL){
	perror("\n"); exit(1);
}

struct dirent *queryRes;
struct stat *fileInfo;
int boolean1=0,boolean2=0;
char toMove[FILENAME_MAX];
char destinazione[FILENAME_MAX];

while((queryRes=readdir(wdStream))!=NULL){
	if(strcmp(argv[1],queryRes->d_name)==0){
		printf("\nTrovato il file da spostare\n"); fflush(stdout);
		fileInfo=calloc(1,sizeof(struct stat));
		if(stat(queryRes->d_name,fileInfo)<0){
			perror("\n");
			exit(1);	
		}
		if(S_ISREG(fileInfo->st_mode)){
			printf("\nIl file trovato e' regolare!\nDimensione:%d\n",fileInfo->st_size);
			fflush(stdout);
			if(fileInfo->st_size>1){
				printf("\nE' > DI 1 KB...\n"); 
				fflush(stdout);
				boolean1=1;
				strcpy(toMove,queryRes->d_name);	
			}	
		}
	}
	else{	
		
		fileInfo=calloc(1,sizeof(struct stat));
		if(lstat(queryRes->d_name,fileInfo)<0){ //usa lstat altrimenti il softlink quando trovato non e' analizzato in quanto tale!!
			perror("\n");
			exit(1);	
		}
		if(S_ISLNK(fileInfo->st_mode)){
			
			printf("\nTrovato il softlink che indica destinazione cut and paste!\n");
			if(readlink(queryRes->d_name,destinazione,FILENAME_MAX)<0){
				perror("\n");
				exit(1);			
			}
			printf("\nIl softlink dice che dobbiamo spostare il file in %s\n",destinazione); fflush(stdout);
			boolean2=1;		
		}
	
	}



}
if(boolean1 && boolean2){
//abbiamo trovato sia il file passato da cmd, che risulta file regolare e >1kB, sia il softlink
//che indica la destinazione
	strcat(destinazione,"/");
	strcat(destinazione,toMove);
	 //la destinazione dell'hard link che rappresenta la copia, deve avere anche il nome del file da creare
	printf("\nFacciamo cut del file %s e lo mettiamo in %s",toMove,destinazione);
	fflush(stdout);


	if(link(toMove,destinazione)<0){
		perror("\n");
		exit(1);
	}
//e' stato creato il nuovo hardlink nella destinazione puntata dal softlink, e ora cancelliamo
//il vecchio file
	if(unlink(toMove)<0){
		perror("\n");
		exit(1);
	}


}


}
