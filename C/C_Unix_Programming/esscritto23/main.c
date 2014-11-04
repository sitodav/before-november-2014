/*Scrivere un programma C in ambiente Linux in cui il processo padre crea due figli A e B. Il processo
padre esamina il contenuto della directory corrente ed invia i nomi delle sotto-directory al figlio A ed i
nomi dei file regolari al figlio B. Quando A riceve dal padre un messaggio, stampa a video la stringa
“Figlio A: directory “ seguita dalla stringa ricevuta dal padre, similmente quando B riceve dal padre un
messaggio, stampa a video la stringa “Figlio B: file regolare“ seguita dalla stringa ricevuta dal padre.
*/

#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>



int main(int argc,char *argv[]){
//usiamo 2 pipe anonime
int childpid[2];
int fdPipeA[2],fdPipeB[2],ret;
char wd[FILENAME_MAX],*toRead;
DIR *wdStream;
struct stat info;
struct dirent *queryRes;

if(pipe(fdPipeA)<0 || pipe(fdPipeB)<0){
	perror("\n");
	exit(1);
}

int i;
for(i=0;i<2;i++){
	childpid[i]=fork();
	if(childpid[i]<0){
		perror("\n");
		exit(1);
	}
	if(childpid[i]==0) //figlio
		break;
}

if(i==0){
//figlio A
	/*close(fdPipeB[0]);
	close(fdPipeB[1]);
	close(fdPipeA[1]);*/

	toRead=(char *)calloc(FILENAME_MAX,sizeof(char));
	while((ret=read(fdPipeA[0],toRead,FILENAME_MAX))>0){
		//esced a questo while solo quando il padre, terminati i file nella directory, chiude le estremita' di scrittura, e intanto ogni volta che ritorna dalla read e' perche' ha letto qualcosa, e finche' non legge rimane in attesa finche' e' aperta l'estremita' in scrittura del padre
		printf("\nSono A: REGULAR FILE %s\n",toRead);
		fflush(stdout);
		free(toRead);
		toRead=calloc(FILENAME_MAX,sizeof(char));
	}
}

else if(i==1){
//figlio B
	/*close(fdPipeA[0]);
	close(fdPipeA[1]);
	close(fdPipeB[1]);*/
	toRead=(char *)calloc(FILENAME_MAX,sizeof(char));
	while((ret=read(fdPipeB[0],toRead,FILENAME_MAX))>0){
		//esced a questo while solo quando il padre, terminati i file nella directory, chiude le estremita' di scrittura, e intanto ogni volta che ritorna dalla read e' perche' ha letto qualcosa, e finche' non legge rimane in attesa finche' e' aperta l'estremita' in scrittura del padre
		printf("\nSono B: DIRECTORY %s\n",toRead);
		fflush(stdout);
		free(toRead);
		toRead=calloc(FILENAME_MAX,sizeof(char));
	}


}

else{
//padre
	/*close(fdPipeA[0]);
	close(fdPipeB[0]);*/
	getcwd(wd,FILENAME_MAX);
	if((wdStream=opendir(wd))==NULL){
		perror("\n");
		exit(1);
	}


	while((queryRes=readdir(wdStream))!=NULL){

		
		fflush(stdout);

		if(stat(queryRes->d_name,&info)<0){
			perror("\n");
			exit(1);
		}
		if(S_ISREG(info.st_mode)){//allora e' un reg file e lo mando ad A
			sleep(2); //usiamo questa sleep perche' il padre potrebbe scrivere troppo veloecmente su una delle pipe, quindi il figlio con una sola lettura prenderebbe i nomi di 2 file diversi e la stampa non sarebbe come indicata da testo esercizio
			ret=write(fdPipeA[1],queryRes->d_name,strlen(queryRes->d_name));
			if(ret<0){
				perror("\n");

				exit(1);
			}
		}
		else if(S_ISDIR(info.st_mode)){ //allora e' una directory e lo mando a B
			sleep(2);
			ret=write(fdPipeB[1],queryRes->d_name,strlen(queryRes->d_name));
			if(ret<0){
				perror("\n");

				exit(1);
			}

		}

	}
	printf("\nSono padre: file terminati nella directory corrente!\n");
	fflush(stdout);
	close(fdPipeA[1]);
	close(fdPipeB[1]); //chiude le estremita' in scrittura, cosi' i figli escono dal while sulla read>0

	exit(0);

}




}

