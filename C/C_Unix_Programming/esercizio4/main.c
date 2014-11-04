#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <dirent.h>
#include <fcntl.h>
#include <string.h>

int main(){
DIR *pStreamDir;
char wd[FILENAME_MAX],buff[FILENAME_MAX],winner[FILENAME_MAX];
struct dirent *fileEntryDir;
struct stat infoFile;
struct stat infoRegFile;
int maxsize=0;

getcwd(wd,FILENAME_MAX);

if((pStreamDir=opendir(wd))<0){
	perror("\n"); exit(1);
}

while((fileEntryDir=readdir(pStreamDir))!=NULL){
	
	if(lstat(fileEntryDir->d_name,&infoFile)<0){
		perror("\n"); exit(1);
	}
	
	if(S_ISLNK(infoFile.st_mode)){ //se il file trovato e' un symlink allora dobbiamo prendere info sul file a cui punta
					//(readlink +stat()) e vedere quanto e' grande quest'ultimo
		if(readlink(fileEntryDir->d_name,buff,FILENAME_MAX)<0){ //ora in buff stara' il path del vero file puntato 										  dal symbolic link
			perror("\n"); exit(1);		
		}
		
		if(stat(buff,&infoRegFile)<0){ //ora in infoRegFile ci saranno le inform del vero reg file puntato dal sim 								link tra cui anche la dimensione di questo
			perror("\n"); exit(1);
		}

		if(infoRegFile.st_size>maxsize){
			maxsize=infoRegFile.st_size;
			strcpy(winner,fileEntryDir->d_name);
					
		}	
	}


}

printf("\nIl symb link che punta al file di taglia maggiore e' : %s con un file puntato di taglia %d KB\n",winner,maxsize);



return 0;}
