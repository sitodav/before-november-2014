/*
Scrivere un programma C in ambiente Linux conforme allo standard Posix che accetta due directory, dir1
e dir2, da riga di comando cerca in dir1 il primo link simbolico che punta ad un file .txt, ne prende il
nome e ne crea una copia invertita all’interno della dir2 in un file con stesso nome ed estensione del file
individuato precedentemente. Alla copia bisogna assegnare i diritti di scrittura, lettura ed esecuzione per
il proprietario e sola lettura per il gruppo e per gli altri.

*/
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>


void copia_inversa(char path[],int fdFileR);

int main(int argc,char *argv[]){
if(argc!=3){
	printf("\nUSO: dir1 dir2\n");
	exit(0);
}
char dir1[FILENAME_MAX],dir2[FILENAME_MAX],wd[FILENAME_MAX];



fflush(stdout); 


DIR * dirStream;
struct dirent *queryRes;
struct stat info;
char *realPath;
char *pathCompleto;
char *pathCompleto2;

if((dirStream=opendir(argv[1]))==NULL){
	
	perror("\n");
	exit(1);
}



while((queryRes=readdir(dirStream))!=NULL){	
	fflush(stdout);
	printf("\nFile %s\n",queryRes->d_name);
	fflush(stdout);

	pathCompleto=calloc(FILENAME_MAX,sizeof(char));
	strcpy(pathCompleto,argv[1]);
	strcat(pathCompleto,"/");
	strcat(pathCompleto,queryRes->d_name);
	printf("\nCerco informazioni per %s\n",pathCompleto);
	fflush(stdout);
	

	if(lstat(pathCompleto,&info)<0){	
		perror("\n");
		exit(1);	
	}
	
	if(S_ISLNK(info.st_mode)){
		printf("\n%s e' un sym link\n",queryRes->d_name);
		fflush(stdout);	

		realPath=(char *)calloc(FILENAME_MAX,sizeof(char));
		if(readlink(pathCompleto,realPath,FILENAME_MAX)<0){
			perror("\n");
			exit(1);		
		}
		printf("\nIl sym link %s punta a %s\n",queryRes->d_name,realPath); fflush(stdout);
		if(strcmp(realPath+(strlen(realPath)-4),".txt")==0){
			printf("\nIl file puntato dal symlink e' un .txt\nInizio operazioni\n");
			fflush(stdout);		
			int fdFileR,fdFileW;
			if((fdFileR=open(realPath,O_RDONLY))<0){
				perror("\n");
				exit(1);			
			}
			pathCompleto2=calloc(FILENAME_MAX,sizeof(char));
			strcpy(pathCompleto2,argv[2]);
			strcat(pathCompleto2,"/");
			int j;
			for(j=strlen(realPath)-1;j>=0;j--)
				if(realPath[j]=='/')
					break;
						
			
			strcat(pathCompleto2,realPath+j+1);
			printf("\nIl file che dovro' creare ha path %s\n",pathCompleto2);	
			fflush(stdout);
			copia_inversa(pathCompleto2,fdFileR);
			exit(0);
			
			
			

		
		}

	}
	
	


}


}



void copia_inversa(char path[],int fdFileR){
int fdFileW;
if((fdFileW=open(path,O_WRONLY | O_CREAT,00755))<0){
	perror("\n");
	exit(1);
}

int maxOff=lseek(fdFileR,0,SEEK_END);
int i;
char t;
for(i=0;i<=maxOff;i++){
	lseek(fdFileR,maxOff-i,SEEK_SET);
	if(read(fdFileR,&t,1)<0){
		perror("\n");
		exit(1);
	}
	if(write(fdFileW,&t,1)<0){
		perror("\n");
		exit(1);	
	}
}

printf("\nOperazioni terminate\n");

}
