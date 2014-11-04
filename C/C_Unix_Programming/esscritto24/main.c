/*Scrivere un programma C e Posix in ambiente Linux che, fornita una directory DIR come argomento,
ricerchi il file con estensione .txt la cui dimensione è maggiore di 200Kb, puntato da un link simbolico
presente in DIR. Individuato il file .txt, il programma crea un file testo con estensione .txt che contiene il
nome del file. Il file creato deve avere i permessi di lettura, scrittura ed esecuzione per il proprietario
ed il gruppo e sola lettura per gli altri.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#define ESTENSIONE_CERCATA "txt"


int main(int argc,char *argv[]){
if(argc!=2){
	printf("\nUso: nome dir\n");
	exit(0);
}


DIR *dirStream;
struct stat info;
struct dirent *queryRes;
char *realPath=calloc(FILENAME_MAX,sizeof(char));
char pathCompleto[FILENAME_MAX];

if((dirStream=opendir(argv[1]))==NULL){
	perror("\n");
	exit(1);
}

while((queryRes=readdir(dirStream))!=NULL){
	printf("\nTrovato il file %s\n",queryRes->d_name);
	fflush(stdout);
	//NB FAI ATTENZIONE, CHE CON LA READDIR VIENE RITORNATO IL NOME DI UN FILE TROVATO IN QUELLA DIR, MA SENZA IL SUO PATH COMPLETO, QUINDI SE VOGLIAMO PASSARE IL NOME ALLA STAT (O LSTAT CHE SIA) NON POSSIAMO PASSARGLI IL NOME SINGOLO RESTITUITO DALLA READDIR, PERCHE' QUESTO FUNZIONEREBBE SOLTANTO NEL CASO IN CUI CI TROVIAMO IN QUELLA DIR, QUINDI VA FATTO UNO STRCAT TRA IL PATH DI QUELLA CARTELLA E IL NOME DEL FILE RESTITUITO DALLA READDIR
	strcpy(pathCompleto,argv[1]);
	strcat(pathCompleto,"/");
	strcat(pathCompleto,queryRes->d_name);
	printf("\nEseguo lstat per %s\n",pathCompleto);
	fflush(stdout);
	if(lstat(pathCompleto,&info)<0){
		perror("\n");
		exit(1);	
	}
	if(S_ISLNK(info.st_mode)){
		printf("\nIl file %s e' un symbolic link\n",queryRes->d_name);
		fflush(stdout);
		//otteniamo il path vero a cui punta il sym link (passando il path completo del sym link alla readlynk)
		free(realPath);
		realPath=(char *)calloc(FILENAME_MAX,sizeof(char));		
		if(readlink(pathCompleto,realPath,FILENAME_MAX)<0){
			perror("\n");
			exit(1);	
		}
		printf("\nIl symlink %s punta in realta' a %s\n",queryRes->d_name,realPath);

		if(strcmp(realPath+(strlen(realPath)-strlen(ESTENSIONE_CERCATA)),ESTENSIONE_CERCATA)==0){
			printf("\nIl file e' dell'estensioe cercata (=%s)\n",ESTENSIONE_CERCATA);
			fflush(stdout);		
		}
		
		//NB ORA DOBBIAMO CONTROLLARE LE DIMENSIONI, MA NON DELL SYMLINK, QUINDI ABBIAMO BISOGNO DI RIFARE UNA STAT, MA QUESTA VOLTA PROPRIO STAT NON LSTAT, PERCHE' ABBIAMO BISOGNO DI RECUPERARE LE INFORMAZIONI DEL VERO FILE PUNTATO
		//possiamo indipendentemente ora fare o una stat sul symlink o una lstat/stat sul path del ver ofile puntato che ci siamo tirati fuori con la readlynk		
		if(stat(realPath,&info)<0){
			perror("\n");
			exit(1);		
		}
		//e controlliamo dimensione vero file puntato
		int fdFileW; 

		if(info.st_size>200){
			//il file lo creiamo nella working directory attuale e lo chiamiamo new.txt
			if((fdFileW=open("new.txt",O_CREAT | O_WRONLY,00775))<0){
				perror("\n");
				exit(1);			
			}
			if(write(fdFileW,realPath,strlen(realPath))<0){ //scriviamo il nome completo del file trovato su new.txt
				perror("\n");
				exit(1);			
			}
			printf("\nCompito TERMINATO\n");
			close(fdFileW);			
			exit(0);
								
		}
		
	}

}







}
