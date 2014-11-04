#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#define file1 "file1.txt"
#define file2 "file2.txt"
#define temp "temp.txt"
#define CHIUSA 1
#define APERTA 0
#define FIGLIO 0
#define PADRE 1

//e' vero che in file2 ci sono solo caratteri numerici. Tuttavia sono scritti con codifica ascii come char testuali,
//quindi basta che leggiamo 1 char alla volta, e lo trasformiamo con sscanf in un int, perche' se leggessimo in int,
//leggerebbe tutta l'intera stringa di numeri considerandolo un solo grande numero (purche' non sia maggiore di 32.000.000.000 circa come numero) e non potremmo lavorare sulle singole digit

//per complicare le cose facci oche entrambi i processi lavorano su entrambi i files

int lock;
int alone=0;
//uso la variabile alone in modo che quando un processo non ha piu' niente da fare, segnale con sigusr2 l'altro processo, che si imposta la variabile, cosi' non dovra' piu' aspettare l'altro processo che lo attivi, che avendo terminato, lo farebbe rimanere in deadlock. In sostanza quando alone e' ==1 allora il processo non va piu' in busy wait ma fa tutto quello che deve fare



void handler(int sign);

int main(int argc,char *argv[]){

int childpid;
int fdFileR,fdFileW,fdFileR2,ret1,ret2;
char t;

if((fdFileW=open(temp,O_WRONLY | O_CREAT,00777))<0){
	perror("\n");
	exit(1);


}

if(signal(SIGUSR1,handler)==SIG_ERR){
		perror("\n");
		exit(1);
}
if(signal(SIGUSR2,handler)==SIG_ERR){
		perror("\n");
		exit(1);
}

childpid=fork();


if(childpid==0){
		//figlio
	
	
	///////////////7
	if((fdFileR=open(file1,O_RDONLY))<0){
		perror("\n");
		exit(1);
	}
	
	if((fdFileR2=open(file2,O_RDONLY))<0){
		perror("\n");
		exit(1);
	}
	if(alone==0)
		lock=CHIUSA;
	
	while(1){
		while(lock==CHIUSA);
		ret1=read(fdFileR,&t,sizeof(char));
		if(ret1<0){
			perror("\n");
			exit(1);		
		}
		if(ret1!=0){ //se invece non c'e' piu' niente da prendere su file1, va avanti perche' potrebbe esserci altro su file 2
		 	lseek(fdFileW,1,SEEK_SET);
			if(write(fdFileW,&t,sizeof(char))<0){
				perror("\n"); exit(1);			
			}
			printf("\nSono figlio: scritto %c",t);
			fflush(stdout);
				
		}
		//in ogni caso, che abbia ancora da lavorare su file1 o no, deve sbloccare l'altro processo ora
		if(kill(getppid(),SIGUSR1)<0){
			perror("\n");
			exit(1);		
		}
		//e bloccarsi prima di iniziare a lavorare col file2
		if(alone==0) /*se l'altro process oha finito, ci ha fatt impostare alone su 1 e non andremo in busy */
 			lock=CHIUSA;
			
		while(lock==CHIUSA);
		
		ret2=read(fdFileR2,&t,sizeof(char));
		if(ret2<0){
			perror("\n");
			exit(1);		
		}
		if(ret2!=0){ //non puo' ancora uscire se e' finito il file 1 perche' potrebbero esserci altre cose da leggere sul file 1
		 	lseek(fdFileW,2,SEEK_SET);
			if(write(fdFileW,&t,sizeof(char))<0){
				perror("\n"); exit(1);			
			}
			printf("\nSono figlio: scritto %c",t);
			fflush(stdout);
				
		}
		if(kill(getppid(),SIGUSR1)<0){
			perror("\n");
			exit(1);		
		}
		//prima di bloccarmi di nuovo, controllo se per caso non ho finito su entrambi i file
		if(ret1==0 && ret2==0){//allora abbiamo finito, quindi usciamo, ma per evitare che l'altro processo rimanga in deadlock: gli facciamo impostare mandandogli un segnale, la var alone su 1, cosi' non dovra' piu' aspettarci
			if(kill(getppid(),SIGUSR2)<0){
				perror("\n"); 
				exit(1);			
			}
			break;
		}
		if(alone==0)
			lock=CHIUSA;	
		
		
			
		
	}


	/////////////

}

else{
	//padre
	
	
	if((fdFileR=open(file1,O_RDONLY))<0){
		perror("\n");
		exit(1);
	}
	
	if((fdFileR2=open(file2,O_RDONLY))<0){
		perror("\n");
		exit(1);
	}
	
	lock=APERTA;
	
	while(1){
		
		ret1=read(fdFileR,&t,sizeof(char));
		if(ret1<0){
			perror("\n");
			exit(1);		
		}
		if(ret1!=0){ //se invece non c'e' piu' niente da prendere su file1, va avanti perche' potrebbe esserci altro su file 2
		 	lseek(fdFileW,1,SEEK_SET);
			if(write(fdFileW,&t,sizeof(char))<0){
				perror("\n"); exit(1);			
			}
			printf("\nSono padre: scritto %c",t);
			fflush(stdout);
				
		}
		//in ogni caso, che abbia ancora da lavorare su file1 o no, deve sbloccare l'altro processo ora
		if(kill(childpid,SIGUSR1)<0){
			perror("\n");
			exit(1);		
		}
		//e bloccarsi prima di iniziare a lavorare col file2
		if(alone==0) /*se l'altro process oha finito, ci ha fatt impostare alone su 1 e non andremo in busy */
 			lock=CHIUSA;
			
		while(lock==CHIUSA);
		
		ret2=read(fdFileR2,&t,sizeof(char));
		if(ret2<0){
			perror("\n");
			exit(1);		
		}
		if(ret2!=0){ //non puo' ancora uscire se e' finito il file 1 perche' potrebbero esserci altre cose da leggere sul file 1
		 	lseek(fdFileW,2,SEEK_SET);
			if(write(fdFileW,&t,sizeof(char))<0){
				perror("\n"); exit(1);			
			}
			printf("\nSono padre: scritto %c",t);
			fflush(stdout);
				
		}
		if(kill(childpid,SIGUSR1)<0){
			perror("\n");
			exit(1);		
		}
		//prima di bloccarmi di nuovo, controllo se per caso non ho finito su entrambi i file
		if(ret1==0 && ret2==0){//allora abbiamo finito, quindi usciamo, ma per evitare che l'altro processo rimanga in deadlock: gli facciamo impostare mandandogli un segnale, la var alone su 1, cosi' non dovra' piu' aspettarci
			if(kill(childpid,SIGUSR2)<0){
				perror("\n"); 
				exit(1);			
			}
			break;
		}	
		if(alone==0)
			lock=CHIUSA;
		while(lock==CHIUSA);
			
		
	}

}

close(fdFileW);


return 0;
}



void handler(int sign){
if(sign==SIGUSR1)
	lock=APERTA;
else if(sign==SIGUSR2)
	alone=1;

}
