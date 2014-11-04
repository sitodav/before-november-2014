#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>

int fdFileR;
int fdPipe1[2];
int fdPipe2[2];

int myMax=0,maxP0=0,maxP1=0; //globali, usate solo da P2 (p0 e p1 myMax la nascondono perche' usano la locale omonima)

void signalHandler(int sign);
void leggiTrasforma(int fdFileR,int porz,int *arrayToFill,int index);

int main(int argc,char *argv[]){
if(argc!=3){
	printf("\nuso: nomefile numElem\n");
	exit(0); 
}



if(pipe(fdPipe1) || pipe(fdPipe2)){
	perror("\nerr 25\n"); 
	exit(1);
}





char *nomeFile=(char *)calloc(FILENAME_MAX,sizeof(char));
int numElem;
int porz;
sscanf(argv[2],"%d",&numElem);
strcpy(nomeFile,argv[1]);
porz=numElem/3;

int childpid[2];
int i;

int *arrayNumeriLetti=(int *)calloc(porz,sizeof(int));


for(i=0;i<2;i++){
	childpid[i]=fork();
	if(!childpid[i])
		break;
}


int k;
int ret;

if(i==2){
	int myMax=0; //locale
	printf("\nSono P0 (padre)\n"); fflush(stdout);
	close(fdPipe2[0]);
	close(fdPipe2[1]);
	close(fdPipe1[0]);

	if((fdFileR=open(nomeFile,O_RDONLY))<0){
		perror("\nerrore 37\n");
		exit(1);	
	}
	leggiTrasforma(fdFileR,porz,arrayNumeriLetti,0);
	
	
	
	for(k=0;k<porz;k++)
		if(myMax<arrayNumeriLetti[k]){
			sleep(2);
			myMax=arrayNumeriLetti[k];
			ret=write(fdPipe1[1],&myMax,sizeof(int));
			if(ret<=0){
				perror("err 81\n");
				printf("\nP2 ha chiuso il suo lato pipe! TERMINO!\n");	
				exit(0);			
			}
			
					
		}
	close(fdPipe1[1]);
	printf("\nSono P0, ho terminato, chiuso pipe e termino!\n");
		
}

else if(i==0){
	int myMax=0; //locale
	close(fdPipe1[0]);
	close(fdPipe1[1]);
	close(fdPipe2[0]);

	printf("\nSono figlio P1\n"); fflush(stdout);
	if((fdFileR=open(nomeFile,O_RDONLY))<0){
		perror("\nerrore 45\n");
		exit(1);	
	}
	leggiTrasforma(fdFileR,porz,arrayNumeriLetti,1);
	
	for(k=0;k<porz;k++)
		if(myMax<arrayNumeriLetti[k]){
			sleep(2);
			myMax=arrayNumeriLetti[k];
			ret=write(fdPipe2[1],&myMax,sizeof(int));
			if(ret<=0){
				perror("err 112\n");
				printf("\nP2 ha chiuso il suo lato pipe! TERMINO!\n");	
				exit(0);			
			}
			
	}
	close(fdPipe2[1]);
	printf("\nSono P1, ho terminato, chiuso pipe e termino!\n");
}

else{
	printf("\nSono figlio P2\n"); fflush(stdout);
	close(fdPipe1[1]);
	close(fdPipe2[1]);
	if((fdFileR=open(nomeFile,O_RDWR))<0){
		perror("\nErrore 53\n");
		exit(1);	
	}
	leggiTrasforma(fdFileR,porz,arrayNumeriLetti,2);
	
	for(k=0;k<porz;k++)
		if(myMax<arrayNumeriLetti[k])
			myMax=arrayNumeriLetti[k];
	
	int chiusa1=0,chiusa2=0;
	while(!chiusa1 || !chiusa2){
		
		if(!chiusa1){
			ret=read(fdPipe1[0],&maxP0,sizeof(int));
			if(ret==0)
				chiusa1=1;
			else if(ret<0){
				perror("errore 135\n");
				exit(1);			
			}		
		}
		
		if(!chiusa2){
			ret=read(fdPipe2[0],&maxP1,sizeof(int));
			if(ret==0)
				chiusa2=1;
			else if(ret<0){
				perror("\nerrore 145\n");
				exit(1);			
			}		
		}
	}
	printf("\nSono P2: P0 e P1 hanno terminato!\n");
	printf("\nIl massimo di P0 e' %d, mentre quello di P1 e' %d, il mio (P2) e' %d\n",maxP0,maxP1,myMax);
	if(maxP0>maxP1)
		maxP1=maxP0;
	if(myMax<maxP1)
		myMax=maxP1;
	printf("\nIl maggiore dei massimi e' %d\n",myMax);
	fflush(stdout);

	fflush(stdout);
	char *buff=(char *)calloc(50,sizeof(char));
	sprintf(buff,"\nIl maggiore di tutti e' %d",myMax);	
	if(write(fdFileR,buff,strlen(buff))<0){
		perror("\nerr 156\n");
		exit(1);	
	}
	
	

}


}


void leggiTrasforma(int fdFileR,int porz,int *arrayToFill,int index){
int i;

if(lseek(fdFileR,porz*index*2,SEEK_SET)<0){
	perror("\nerrore 74\n");
	exit(1);
}

int decine,unita;
char t;
for(i=0;i<porz;i++){
	if(read(fdFileR,&t,sizeof(char))<0){
		perror("\nerrore 81\n");
		exit(1);	
	}
	decine=((int)t-48)*10;	
	
	
	if(read(fdFileR,&t,sizeof(char))<0){
		perror("\nerrore 81\n");
		exit(1);	
	}
	unita=((int)t-48);
	
	*(arrayToFill+i)=decine+unita;

}


}


void signalHandler(int sign){
if(sign==SIGINT){
	printf("\nSONO P2: hai chiesto di interrompere, prima pero' calcolo max parziale, scrivo e poi faccio terminare!\n"); fflush(stdout);
	if(maxP0>maxP1)
		maxP1=maxP0;
	if(myMax<maxP1)
		myMax=maxP1;
	printf("\nIl massimo parziale trovato fino ad ora e' %d\n",myMax);
	char *buff=(char *)calloc(50,sizeof(char));
	sprintf(buff,"max parziale -> %d",myMax);	
	if(write(fdFileR,buff,strlen(buff))<0){
		perror("\nerrore 223\n");
		exit(1);	
	}
	close(fdPipe1[0]);
	close(fdPipe2[0]);
	exit(0);
	
	

}


}
