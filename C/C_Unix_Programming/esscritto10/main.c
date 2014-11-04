/*
2. 6 punti (PER CFU 12 e 10)
Scrivere un programma C in ambiente Linux conforme allo standard Posix che accetta due directory, dir1
e dir2, da riga di comando cerca in dir1 il primo link simbolico che punta ad un file .txt, ne prende il
nome e ne crea una copia invertita all’interno della dir2 in un file con stesso nome ed estensione del file
individuato precedentemente. Alla copia bisogna assegnare i diritti di scrittura, lettura ed esecuzione per
il proprietario e sola lettura per il gruppo e per gli altri
*/

//QUESTO ESERCIZIO L'HO RESO PIU COMPLESSO PER ESERCITARMI CON LA REALLOC


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

struct file{
 	char nome[FILENAME_MAX];
	char path[FILENAME_MAX];
	int sl;
};

int main(int argc,char **argv){

if(argc!=3){
	printf("\nUso dir1 dir2\n");
	exit(0);
}


struct stat infoFile;
struct dirent *queryDir;
DIR *wdStream;

struct file **head=NULL;
int i=0;



if((wdStream=opendir(argv[1]))==NULL){
	perror("\n"); exit(1);
}

while((queryDir=readdir(wdStream))!=NULL){
	i++;
	head=(struct file **)realloc(head,i*sizeof(struct file *));
	*(head+i-1)=calloc(1,sizeof(struct file));
	strcpy((*(head+i-1))->nome,queryDir->d_name);
	strcpy((*(head+i-1))->path,argv[1]);
	strcat((*(head+i-1))->path,"/");
	strcat((*(head+i-1))->path,queryDir->d_name);

	if(lstat(head[i-1]->path,&infoFile)<0){
		perror("\n"); exit(1);	
	}
		
	if(S_ISLNK(infoFile.st_mode))
		head[i-1]->sl=1;
	else head[i-1]->sl=0;
					
}

int j;
printf("\nLista file trovati nella cartella data in input\n");
for(j=0;j<i;j++)
	printf("\n%s %d in -> %s\n",(*(head+j))->nome,(*(head+j))->sl,(*(head+j))->path);

printf("Vediamo quali di questi sono soft link che puntano ad un file testuale\n");
char pointed[FILENAME_MAX];

for(j=0;j<i;j++){
	if((*(head+j))->sl){
		if(readlink((*(head+j))->path,pointed,FILENAME_MAX)<0){
			perror("\n"); exit(1);		
		}
		printf("\n%s\n",pointed); fflush(stdout);
		if(strcmp(pointed+strlen(pointed)-4,".txt")==0){
			printf("\nIl link simbolico %s punta a file testuale %s\n",(*(head+j))->nome,pointed);	
			fflush(stdout);	
			printf("\nCopio al contrario\n"); 
			int fdFileR,fdFileW;			
			if((fdFileR=open(pointed,O_RDONLY))<0){
				perror("\n"); exit(1);			
			}
			char nomePointed[FILENAME_MAX];
			int k;
			for(k=strlen(pointed)-1;k>=0;k--)
				if(pointed[k]=='/') 
					break;
			strcpy(nomePointed,pointed+k);
			char destinazione[FILENAME_MAX];
			strcpy(destinazione,argv[2]);
			strcat(destinazione,nomePointed);
			printf("\nIl file sara' %s\n",destinazione);
					
			
				
			if((fdFileW=open(destinazione,O_WRONLY | O_CREAT,00777))<0){
				perror("\n"); exit(1);			
			}	
			char c;
			lseek(fdFileR,-1,SEEK_END);
			printf("\n%d\n",lseek(fdFileR,0,SEEK_CUR)); fflush(stdout);
			while(lseek(fdFileR,0,SEEK_CUR)>=0){
				if(read(fdFileR,&c,1)<0){
					perror("\n"); exit(1);				
				}
				printf("offsetattuale:%d\n",lseek(fdFileR,0,SEEK_CUR));
				fflush(stdout);
				if(write(fdFileW,&c,1)<0){
					perror("\n");
					exit(1);
				}
				lseek(fdFileR,-2,SEEK_CUR);
				if(lseek(fdFileR,0,SEEK_CUR)==0) break;			
			}
				
		}
	}

}







}
