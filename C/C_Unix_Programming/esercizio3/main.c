#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <dirent.h>

int main(int argc,char *argv[]){

char wd[FILENAME_MAX]; //qui inseriamo la working dir del processo

DIR *pStreamDir; //qui salviamo il punt allo stream della directory aperta, ritornato dalla opendir

struct dirent *entryDir; //qui salviamo di volta in volta la struct dirent ritornata dalla readdir (struct che rappresenta 				l'entry di un file presente nello stream della cartella)

struct stat infoFile; //e per ogni file trovato dalla readdir, prendiamo il nome e lo mandiamo alla lstat, che ci riempie 				questa struct che gli passiamo



if(getcwd(wd,FILENAME_MAX)<0){ //ottengo path della working directory
	
	printf("\nImpossibile ottenere la working directory attuale\n"); perror("");
	exit(1);
}

if((pStreamDir=opendir(wd))==NULL){ //passo il path della working directory alla open dir che la associa ad uno stream in 					    mem, e mi ritorna il punt a questo stream (pStreamDir)
	
	printf("\nImpossibile aprire la working directory\n"); perror("");
	exit(1);
}

while((entryDir=readdir(pStreamDir))!=NULL){ //finche' la readdir leggendo nello stream, ritorna un entry di un file 						     presente (che e' ritornato come punt struct dirent che salviamo in entryDir...)
		
		if(lstat(entryDir->d_name,&infoFile)<0) perror("\n"); //prendi il nome dalla struct dirent di quel file 									trovato, e passalo alla lstat cosi' che 									quest'ultima possa riempirmi la struct infoFile con 										le informazioni del file
		
		if(S_ISLNK(infoFile.st_mode)){ //se testando il campo st_mode nella struct infoFile, se risulta un soft link
			
			printf("\nTrovato link simbolico di nome %s\n",entryDir->d_name);		//stampane il nome
		}
		

}










return 0;

}
