/*programma che stampa il tipo di ogni file presente nella working directory del processo*/


#define _BSD_SOURCE 
//define delle test macro richieste per testare il file aperto 

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>



int main(int argc,char *argv[]){
	DIR *dirStream;
	struct dirent *queryDir;
	char workingDir[100];
	
	getwd(workingDir); //prendiamo il path della working directory
	printf("\nStampiamo tipo per i file nella cartella corrente :\n%s\n",workingDir); fflush(stdout);
	
	if((dirStream=opendir(workingDir))==NULL){ //apriamo lo stream della working directory passandogli il path 
		perror("\n:"); exit(1);
	}
	
	
	while((queryDir=readdir(dirStream))!=NULL){ //finche' la readdir ci ritora puntatori a struct che contengono info 							    file continuiamo

		printf("file: %s",queryDir->d_name);
		printf("\n\t size: %d",queryDir->d_off);
		if(DT_BLK(queryDir->d_type))
			printf("  tipo: BLOCK FILE\n");
		else if(DT_DIR(queryDir->d_type))
			printf("  tipo: DIRECTORY FILE\n");
		else if(DT_CHR(queryDir->d_type))
			printf("  tipo: CHAR DEVICE\n");
		else if(DT_REG(queryDir->d_type))
			printf("  tipo: REGULAR FILE\n");
		else if(DT_LNK(queryDir->d_type))
			printf("  tipo: SYMBOLIC LINK\n");
		else 
			printf("  tipo: UNKNOWN\n"); 

	}
	
	
	


return 0;
}
