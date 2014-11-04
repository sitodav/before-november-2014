#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>



//modifica mia per rendere piu' complesso l'es con una lista globale di tutti i file trovati con la loro dimensione
struct file{
	char path[FILENAME_MAX];
	int size;
	int r,w,x;
} *arrayFiles;

int main(int argc,char *argv[]){

if(argc!=2){
	printf("\nuso: dirpath\n"); 
	exit(0);
}


DIR *dirStream;
struct dirent *query;
struct stat info;

if((dirStream=opendir(argv[1]))==NULL){
	perror("\n");
	exit(1);

}

int maxSize=0;
char *maxName=calloc(FILENAME_MAX,sizeof(char));
char *fullPath=calloc(FILENAME_MAX,sizeof(char));;
int n=0;

while((query=readdir(dirStream))!=NULL){
	n++;
	printf("\nNella directory c'e' il file %s\n",query->d_name);

	arrayFiles=realloc(arrayFiles,n*sizeof(struct file));

	strcpy(arrayFiles[n-1].path,argv[1]);
	strcat(arrayFiles[n-1].path,"/");
	strcat(arrayFiles[n-1].path,query->d_name);

	if(stat(arrayFiles[n-1].path,&info)<0){
		perror("\n");
		exit(1);
	}
	
	arrayFiles[n-1].size=info.st_size;
	
	if(info.st_size>maxSize){
		maxSize=info.st_size;	
		strcpy(maxName,query->d_name);
	}
	printf("\nIl file %s ha dimensione %d",query->d_name,info.st_size);
	fflush(stdout);
	
	if(access(query->d_name,R_OK)==0)
		arrayFiles[n-1].r=1;
	else arrayFiles[n-1].r=0;
	
	if(access(query->d_name,W_OK)==0)
		arrayFiles[n-1].w=1;
	else arrayFiles[n-1].w=0;
	
	if(access(query->d_name,X_OK)==0)
		arrayFiles[n-1].x=1;
	else arrayFiles[n-1].x=0;
}

printf("\nLa massima dimensione e' %d, per il file %s\n",maxSize,maxName);
fflush(stdout);

printf("\nLista completa\n");
int i;	
for(i=0;i<n;i++)
	printf("\n%s  : (size)%d  (r)%d  (w)%d  (x)%d",arrayFiles[i].path,arrayFiles[i].size,arrayFiles[i].r,arrayFiles[i].w,arrayFiles[i].x);
	



return 0;
}
