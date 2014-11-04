/*Scrivere un programma C in ambiente Linux che accetta due parametri che rappresentano il nome di un
file F e un numero intero positivo N. Il processo padre deve creare un file F1 che abbia lo stesso nome
del file F con aggiunta l’estensione “.sig” (ad es. pippo.txt -> pippo.txt.sig), controllando che tale file non
esista. Inoltre deve creare un numero di processi pari al numero rappresentato dal secondo parametro
più 1. Ogni processo figlio esegue in modo concorrente ed elabora la corrispondente porzione di 512 byte
del file F (il 1° processo i byte da 0 a 511, il 2° da 512 a 1023, e così via). L’elaborazione consiste nel
trasformare ogni carattere con codice ASCII pari nel carattere ‘0’ e ogni carattere con codice ASCII
dispari nel carattere ‘1’. Terminata l’elaborazione, ogni figlio scrive sul file F1 la porzione elaborata. La
scrittura deve essere sincronizzata dal processo padre in modo che l’ordine dei blocchi scritti corrisponda
a quello dei blocchi letti. Al termine dell’esecuzione di tutti i figli, il padre deve stampare su standard
output la dimensione del file F1.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>


void SIGUSR1handler(int sign);
int main(int argc,char *argv[]){


if(argc!=3){
	printf("\nUso: nomefile.txt n\n"); fflush(stdout);
	exit(0);
}

int maxFigli,*childpid;
sscanf(argv[2],"%d",&maxFigli);
maxFigli+=1;
childpid=calloc(maxFigli,sizeof(int));

int fdFileR,fdFileW;

if(signal(SIGUSR1,SIGUSR1handler)==SIG_ERR){
	perror("\n"); 
	exit(1);

}

if((fdFileR=open(argv[1],O_RDONLY))<0){
	perror("\n");
	exit(1);
}

printf("\nAperto file da leggere\n"); fflush(stdout);

char *nuovo=calloc(FILENAME_MAX,sizeof(char));
strcpy(nuovo,argv[1]);
strcat(nuovo,".sig");

if(access(nuovo,F_OK)<0 && errno==ENOENT){
	printf("\nIl file %s non esiste gia': OK! Lo creo ora\n",nuovo);
	fflush(stdout);
}
else 	{
	printf("\nIl file in cui scrivere esiste gia' !\n"); 
	exit(0);
}

if((fdFileW=open(nuovo,O_WRONLY | O_CREAT,00777))<0){
	perror("\n");
	exit(1);
	
}

int i;
for(i=0;i<maxFigli;i++){
	childpid[i]=fork();
	if(childpid[i]==0)
		break;
}



if(i!=maxFigli){ //figli
	pause(); //aspetto che il padre mi svegli
	char *buff=calloc(512,sizeof(char));
	char toW;
	int j;
	//leggi e scrivi (CS):
	if(read(fdFileR,buff,512)<0){
		perror("\n"); exit(1);	
	}
	for(j=0;j<strlen(buff);j++)
		if(((int)buff[j]%2)==0){
			toW='0';
			if(write(fdFileW,&toW,1)<0){
				perror("\n"); exit(1);			
			}		
		}
		else{
			toW='1';
			if(write(fdFileW,&toW,1)<0){
				perror("\n"); exit(1);			
			}
					
		}
		
					
	close(fdFileR);
	close(fdFileW);
	

	
	if(kill(getppid(),SIGUSR1)<0){ //risveglio il padre
		perror("\n");
		exit(1);	
	}
	
}

else{ //padre
	close(fdFileR);
	close(fdFileW);
	
	for(i=0;i<maxFigli;i++){
		if(kill(childpid[i],SIGUSR1)<0){ //sveglia un figlio
			perror("\n"); exit(1);		
		}
		pause();	//si addormenta e aspetta che il figlio che ha svegliato, quando termina, risvegli lui
	}

}

}




void SIGUSR1handler(int sign){
printf("\nSono %d e sono stato svegliato ! TOCCA A ME!\n",getpid());
fflush(stdout);
return;



}

