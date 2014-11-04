#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <signal.h>




struct child{
	int maxDiagonale;
	int childpid;
} *childs;

void *** allocaArrayMatriciQuadre(int nMatrici,int sizeMatrice,int sizeElemento);
void sigusrHandler(int sign);

int main(int argc,char *argv[]){
srand((unsigned int)time(NULL));
if(signal(SIGUSR1,sigusrHandler)==SIG_ERR){
	perror("\n");
	exit(1);
}

printf("\nInserisci il numero di matrici desiderato!\n"); 
int n;
scanf("%d",&n);
while(getchar()!='\n');

int ***matrice=(int ***)allocaArrayMatriciQuadre(n,3,4);
int i,j,k;


for(k=0;k<n;k++)
	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
			matrice[k][i][j]=rand()%10;

for(k=0;k<n;k++){
	printf("\nMatrice %d-esima\n",k);
	for(i=0;i<3;i++){
		for(j=0;j<3;j++)
			printf("%d ",matrice[k][i][j]);
		printf("\n");
	}
}


int fdpipe[2];
if(pipe(fdpipe)){
	perror("\n");
	exit(1);
}

struct child *childs=(struct child *)calloc(n,sizeof(struct child));

for(k=0;k<n;k++){
	childs[k].childpid=fork();
	if(!childs[k].childpid)
		break;
}

if(k==n){ //padre
	sleep(5);
	printf("\nSono padre, aspetto i massimi delle diagonali\n");
	fflush(stdout);
	close(fdpipe[1]);
	for(k=0;k<n;k++){
		if(kill(childs[k].childpid,SIGUSR1)!=0){
			perror("73\n");
			exit(1);		
		}
		if(read(fdpipe[0],&(childs[k].maxDiagonale),sizeof(int))<0){
			printf("Errore di read per figlio %d-esimo\n",k);
			exit(1);		
		}
		printf("\nIl figlio %d-esimo mi ha mandato %d\n",k,childs[k].maxDiagonale);
		fflush(stdout);
		if(wait(NULL)!=childs[k].childpid){
			perror("\nerrore 83\n");
			exit(1);		
		}
			
			
	}
	printf("\nTutti i figli hanno terminato il loro lavoro!\n");
	int maxMax=0;
	for(k=0;k<n;k++)
		if(childs[k].maxDiagonale>maxMax)
			maxMax=childs[k].maxDiagonale;
	printf("\nIl massimo dei massimi e' %d\n",maxMax);
}

else {
	//figlio k-esimo
	int myMax=0;
	for(i=0;i<3;i++)
		if(matrice[k][i][i]>myMax)
			myMax=matrice[k][i][i];

	printf("\nSono figlio %d-esimo!Attendo risveglio da padre\n",k);
	fflush(stdout);
	pause();
	fflush(stdout);
	close(fdpipe[0]);
	if(write(fdpipe[1],&myMax,sizeof(myMax))<0){
		perror("\nerrore 104\n");
		exit(1);	
	}
	
	
}

return ;

}

void *** allocaArrayMatriciQuadre(int nMatrici,int sizeMatrice,int sizeElemento){
void *** matrix=(void ***)calloc(nMatrici,sizeof(void **));
int k,i,j;

for(k=0;k<nMatrici;k++){
	
	*(matrix+k)=(void **)calloc(sizeMatrice,sizeof(void *));
	for(i=0;i<sizeMatrice;i++){
		*(*(matrix+k)+i)=(void *)calloc(sizeMatrice,sizeElemento);
		
	
	}

}

return matrix;

}

void sigusrHandler(int sign){
	return;
}
