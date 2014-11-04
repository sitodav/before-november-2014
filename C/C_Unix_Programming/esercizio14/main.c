#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <string.h>
#define miafifo "/tmp/miafifo"
#include <signal.h>


void SIGUSR1handler(int sign);

struct figli{
int childpid;
int fdMioLatoFifo;
char fifoDedicata[FILENAME_MAX];

} child[8];

int main(int argc,char *argv[]){

if(signal(SIGUSR1,SIGUSR1handler)==SIG_ERR){ //installiamo il segnale anche se verra' usato dai figli
	perror("\n");
	exit(1);
}


if(argc!=2){
	printf("\nuso: <this> <a or b>\n");
	exit(0);
}

char buff[20]="";
int n=8,i,ret,j;

//faccio gli n figli e per ogni figlio creo anche la fifo dedicata senza fare il controllo dell'esistenza, inutile,(il nome della fifo e' costruito sul pid del figlio) ed e' tutto 
//salvato nella struct figli

for(i=0;i<8;i++){
	child[i].childpid=fork();
	if(child[i].childpid==0)
		break;
	sprintf(buff,"%d",child[i].childpid);
	strcpy(child[i].fifoDedicata,miafifo);
	strcat(child[i].fifoDedicata,buff);
	
	if(mkfifo(child[i].fifoDedicata,0777)<0){
		perror("\n");
		exit(1);	
	}
	
	
}

//i figli arriveranno qui con i:0..7 mentre il padre con i==8
if(i==8){ //padre
	 //apro in scrittura le fifo dedicate per ogni figlio
	for(i=0;i<8;i++){
		printf("\nSono padre e apro fifo %s\n",child[i].fifoDedicata); fflush(stdout);
		if( (child[i].fdMioLatoFifo=open(child[i].fifoDedicata, O_WRONLY)) <0  ){
			perror("\n");
			exit(1);		
		}
		printf("\nAperta\n");
	}
	
	
	//mando l'array di tutti i pid, ad ogni figlio sulla sua FIFO dedicata
	
	for(i=0;i<8;i++)
		for(j=0;j<8;j++){
			ret=write(child[i].fdMioLatoFifo,&(child[j].childpid),sizeof(int));
			if(ret<0){
				perror("\n"); exit(1);			
			}		
		}
	//il padre non deve fare piu' niente ora...
	//lo lasciamo in pausa dopo aver chiuso tutte le fifo visto che non ci serve piu' 		comunicare coi figli
	for(i=0;i<8;i++)
		close(child[i].fdMioLatoFifo);
	pause();
	
					

}


else{	int myid=i; //il mio id i e' proprio uguale al valore dell'i dell'oordine con cui sono stato forkato e salvato sull'array
	int fdMioLatoR;
	char miaFifo[FILENAME_MAX]="";
	int arrayFigli[8]; //qui salvo i pid di tutti i figli (ricevuto dal padre sulla pipe 		dedicatami che io leggo sull'estremita' fdMioLatoR) compreso me

	sprintf(buff,"%d",getpid());	
	strcpy(miaFifo,miafifo);
	strcat(miaFifo,buff);
	printf("\nSono figlio e apro fifo %s\n",miaFifo); fflush(stdout);
	if( (fdMioLatoR=open(miaFifo,O_RDONLY))<0){
		perror("\n"); exit(1);	
	}
	//leggiamo gli 8 int che sono i pid di tutti i figli del padre (compreso me che saro' 		il pid i-esimo)
	for(i=0;i<8;i++){
		ret=read(fdMioLatoR,arrayFigli+i,sizeof(int));
		if(ret<0){
			perror("\n"); exit(1);		
		}	
	}
	
	//ora che i figli hanno ricevuto l'array contenente pid ordinati di tutti gli altri 		figli non hanno piu' bisogno di dialogare col padre attraverso la FIFO, quindi la 		chiudono
	close(fdMioLatoR);
	//in base al valore di arg1 ora...
	
	if(argv[1][0]!='a' && argv[1][0]!='b'){
		printf("\nIl valore inserito da cmd line e' errato!\n");
		exit(0);
	}
		 
		
	else if(argv[1][0]=='a'){
	//allora solo i processi figli con id (id non pid ma id ordine di fork()) pari mandano un SIGUSR1 a quelli con i uguale al loro +1
		
		if(myid%2==0){	
			if(kill(arrayFigli[myid+1],SIGUSR1)<0){		
				perror("\n");
				exit(1);			
			}
			printf("\nSono proc con id:%d e pid:%d->Segnale mandato a processo con id:%d  e pid %d\n",myid,getpid(),myid+1,arrayFigli[myid+1]);
			fflush(stdout);		
		}	
	}
				
	else if(argv[1][0]=='b'){
		if(myid<4){
			
			if(kill(arrayFigli[myid+4],SIGUSR1)<0){
				perror("\n"); exit(1);			
			}
		        printf("\nSono processo con id:%d e pid:%d e ho mandato segnale a proc con id %d e pid %d\n",myid,getpid(),myid+4,arrayFigli[myid+4]);
			fflush(stdout);
		}
			
		

	}
	
	
	
	
	
		

}

return 0;
}




void SIGUSR1handler(int sign){
printf("\nSono processo %d e ho ricevuto SIGUSR1\n",getpid());
fflush(stdout);
return;


}
