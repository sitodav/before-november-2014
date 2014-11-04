#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#define fifo1 "fifo1"
#include <errno.h>
#include <time.h>
#include <fcntl.h>
#include <signal.h>

/questo esercizio funzionava con gli int, poi ho messo con i float ma ho combinato un casino e non va piu'. Tuttavia lo svolgimento e' corretto!

//uso una sola fifo, quindi occorre sincronizzarne l'accesso da parte dei figli

//un errore che facevo era mandare dal padre al figlio il segnale per sbloccarlo, ma
//molto prima che il processo si fosse bloccato (quando ancora stava calcolando la matrice)
//quindi il figlio riceveva il segnale, non gli cambiava niente perche' ncora doveva andarsi a bloccare, e poi si bloccava, e non veniva piu' sbloccato. Il padre, convinto che fosse sbloccato, attendeva che gli inviasse il risultato minimo matrice, ma il figlio bloccato non glielo mandava--> deadlock---> quindi per risolvere, i pause() nei figli mettili sempre tra le primissime istruzioni eseguite nei figli subito dopo la fork() !!!


struct child{
	int childpid;
	float minimo;
} *childs;


void SIGUSR1handler(int sign);
int main(int argc,char *argv[]){

if(argc!=2){
	printf("\nUso: n\n");
	exit(0);
}

if(signal(SIGUSR1,SIGUSR1handler)==SIG_ERR){
	perror("\n");
	exit(1);

}



int n,i,totMin,fdFifoR,fdFifoW;
sscanf(argv[1],"%d",&n);
childs=(struct child *)calloc(n,sizeof(struct child));

if(access(fifo1,F_OK)<0 && errno==ENOENT){
	printf("\nLa FIFO non esiste. La creo\n"); fflush(stdout);
	if(mkfifo(fifo1,00777)<0){
		perror("\n");
		exit(1);
	}
}

for(i=0;i<n;i++){
	childs[i].childpid=fork();
	if(childs[i].childpid==0)
		break;
}

if(i==n){
	printf("\nSono padre\n"); fflush(stdout);
	if((fdFifoR=open(fifo1,O_RDONLY))<0){
		perror("\n");
		exit(1);
	}
	for(i=0;i<n;i++){ //sveglio 1 alla volta i figli e leggo da lui

		if(kill(childs[i].childpid,SIGUSR1)<0){
			perror("\n");
			exit(1);
		}
		if(read(fdFifoR,&(childs[i].minimo),sizeof(float))<0){
			perror("\n");

		}
		printf("\n--->%f",childs[i].minimo); fflush(stdout);

	}
	for(i=0;i<n;i++){
		printf("\nind%d min%f\n",i,childs[i].minimo); fflush(stdout);
	}






}


else{
	srand((unsigned int)i*10);
	int a=10+rand()%50,b=a,j,k;
	float **matrix;

	pause();

	matrix=(int **)calloc(a,sizeof(float *));
	for(j=0;j<a;j++){
		matrix[j]=calloc(b,sizeof(float));
		for(k=0;k<b;k++)
			matrix[j][k]=(217/i)+rand()%(3192/28);
	}

	printf("\nSono processo %d : la mia matrice e'\n",i); fflush(stdout);
	for(k=0;k<a;k++){
		for(j=0;j<b;j++){
			printf(" %f",matrix[k][j]);
			fflush(stdout);
		}
		printf("\n");

	}

	float min=matrix[0][0];

	for(j=0;j<a;j++)
		if(matrix[j][j]<min)
			min=*(*(matrix+j)+j);
	if((fdFifoW=open(fifo1,O_WRONLY))<0){
		perror("\n");
		exit(1);
	}
	if(write(fdFifoW,&min,sizeof(min))<0){
		perror("\n");
		exit(1);
	}




}

}




void SIGUSR1handler(int sign){

printf("\nSono stato sbloccato\n"); fflush(stdout);

}
