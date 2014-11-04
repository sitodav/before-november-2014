#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>


int main(int argc,char *argv[]){

srand((unsigned int)time(NULL));

if(argc!=2){
	printf("\npassami size matrix quadrata\n"); 
	exit(1);
}

int n;
sscanf(argv[1],"%d",&n);

int **matrix=(int **)calloc(n,sizeof(int *));
int i,j;
for(i=0;i<n;i++){
	*(matrix+i)=(int *)calloc(n,sizeof(int));
	for(j=0;j<n;j++){
		*(*(matrix+i)+j)=1+rand()%2;	
		printf("%d ",matrix[i][j]);	
	}
	printf("\n");

}

int *childpid=(int *)calloc(n,sizeof(int));
for(i=0;i<n;i++){
	childpid[i]=fork();
	if(*(childpid+i)==0)
		break;	

}
if(i==n){
	int myMax=0;
	printf("\nSono padre\n");
	fflush(stdout);
	int *res=(int *)calloc(n,sizeof(int));
	for(j=0;j<n;j++){
		if(waitpid(childpid[j],res+j,0)<0){
			perror("\n");
			exit(1);		
		}
		*(res+j)=WEXITSTATUS(res[j]);
		printf("\nprod %d-esimo:-->%d",j,res[j]);
		if(res[j]>myMax)
			myMax=res[j];
		fflush(stdout);
		
	}
		
	printf("\nMax %d\n",myMax);
	fflush(stdout);
	return;
}

else{
	printf("\nSono figlio %d-esimo, calcolo il mio prodotto!\n",i); 
	fflush(stdout);
	int myProd=1;
	int j;
	for(j=0;j<n;j++)
		myProd*=matrix[i][j];
	exit(myProd);

}


}
