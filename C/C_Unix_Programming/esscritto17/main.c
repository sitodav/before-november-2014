/*2. 6 punti (PER CFU 12 e 10)
Scrivere un programma C in ambiente Linux conforme allo standard Posix che accetta due directory, dir1
e dir2, da riga di comando cerca in dir1 il primo link simbolico che punta ad un file .txt, ne prende il
nome e ne crea una copia invertita all’interno della dir2 in un file con stesso nome ed estensione del file
individuato precedentemente. Alla copia bisogna assegnare i diritti di scrittura, lettura ed esecuzione per
il proprietario e sola lettura per il gruppo e per gli altri
*/



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>



int main(int argc,char *argv[]){

if(argc!=3){
	printf("\nUso: dir1 dir2\n");
	exit(0);
}

DIR *dirStream;
if((dirStream=opendir(argv[1]))==NULL){
	perror("\n");
	exit(1);
}


struct dirent *query;
struct stat info;

while((query=readdir(dirStream))!=NULL){
	if(lstat(query->d_name,&info)<0){
		perror("\n");
		exit(1);	
	}
	
	if(S_ISLNK(info.st_mode)){
		printf("\Il file %s e' un soft link\n",query->d_name);
		fflush(stdout);
		char realFile[FILENAME_MAX];	
		if(readlink(query->d_name,realFile,FILENAME_MAX)<0){
			perror("\n");
			exit(1);			
		}
	
		if(strcmp(realFile+(strlen(realFile)-4),".txt")==0){
			printf("\nIl file linkato dal softlink (%s) e' un .txt (%s)\n",query->d_name,realFile);
			fflush(stdout);
			printf("\neseguo copia invertita di %s in %s\n",realFile,argv[2]);
			
			int fdFileR,fdFileW;
			if((fdFileR=open(realFile,O_RDONLY))<0){
				perror("\n");
				exit(1);			
			}
			
			char nome[FILENAME_MAX];
			int k;
			for(k=strlen(realFile);k>=0;k--)
				if(realFile[k]=='/')
					break;
			strcpy(nome,realFile+k);	
			char percorsoNuovo[FILENAME_MAX];
			strcpy(percorsoNuovo,argv[2]);
			strcat(percorsoNuovo,nome);
			printf("\ncreo il file %s\n",percorsoNuovo);
			fflush(stdout);
			if((fdFileW=open(percorsoNuovo,O_WRONLY | O_CREAT,00777))<0){
				perror("\n");
				exit(1);			
			}		
			char t;
			lseek(fdFileR,-1,SEEK_END);
			while(lseek(fdFileR,0,SEEK_CUR)>0){
				if(read(fdFileR,&t,1)<0){
					perror("\n");
					exit(1);				
				}
				lseek(fdFileR,-2,SEEK_CUR);
				if(write(fdFileW,&t,1)<0){
					perror("\n");
					exit(1);				
				}			
			}
		}
	}


}

}
