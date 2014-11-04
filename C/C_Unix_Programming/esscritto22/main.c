/*Scrivere un programma C in ambiente Linux che calcoli, mediante n processi, il prodotto di due matrici
nxn (n è un parametro passato da riga di comando). Si scriva una procedura che prima del calcolo del
prodotto provveda a riempire le due matrici con numeri casuali compresi tra 1 e 20.
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <time.h>

void fillMatrix(int **a,int n); //uso notazione cosi' perche' non sappiamo di quanto dovra' essere alocato l'array quindi non sappiamo digli il numero di colonne

int main(int argc,char *argv[]){
srand((unsigned int)time(NULL));
if(argc!=2){
	printf("\nUSO: DIMENSIONE MATRICE\n");
	exit(0);
}
int n;
sscanf(argv[1],"%d",&n);
//alloc dinamica array
int **array,**array2;
array=(int **)calloc(n,sizeof(int *));
int i;
for(i=0;i<n;i++)
	array[i]=(int *)calloc(n,sizeof(int));
fillMatrix(array,n);
array2=(int **)calloc(n,sizeof(int *));
for(i=0;i<n;i++)
	array2[i]=(int *)calloc(n,sizeof(int));
fillMatrix(array2,n);

//abbiamo le 2 matrici, ora il processo padre crea n processi, ognuno ereditera'
//copia delle 2 matrici, e in base all'indice iesimo di generazione ogni processo iesimo
//calcolera' il prodotto messo nella prima matrice degli elementi riga i di A x B

int id,*childpid;
childpid=(int *)calloc(n,sizeof(int));

for(id=0;id<n;id++){
	childpid[i]=fork();
	if(childpid[i]<0){
		perror("\n");
		exit(1);	
	}
	if(childpid[i]==0){//figlio	
		break;
	}

}
//siccome l'esercizio non chiede esplicitamente di utilizzare delle fifo o pipe anonime per ripassare da ogni figlio al padre la riga prodotto, facciamo che direttamente ogni processo figlio stampa la sua riga prodotto, cosi' che vengano stampate tutte le righe come se fosse stata stampata un intera matrice risultato

if(id==n){ //padre
	exit(0); 
}

else{
	
	fflush(stdout);
	 //i valori di i dei figli vanno da 0 a n-1, quindi proprio il range degli indici della matrice
	int j;
	for(j=0;j<n;j++)
		array[id][j]*=array2[id][j];
		//sovrascrive gli elementi della riga iesima di A con quelli che sono il 			prodotto di AxB
	//e stampa la riga risultante
	printf("riga %d->",id); //questo serve perche' non e' detto che i processi figli stampio le righe in ordine
	for(j=0;j<n;j++)
		printf("%d ",array[id][j]);
	printf("\n");

}


}

void fillMatrix(int **a,int n){



printf("\nPROCEDURA DI RIEMPIMENTO ARRAY %dx%d\n",n,n);
fflush(stdout);

int i,k;
for(i=0;i<n;i++)
	for(k=0;k<n;k++)
		a[i][k]=1+rand()%20;

printf("\nLa matrice riempita e'\n");
for(i=0;i<n;i++){
	for(k=0;k<n;k++)
		printf("%d ",a[i][k]);
	printf("\n");	
}



}
