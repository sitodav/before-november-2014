#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <pthread.h>


int n;

int main(int argc,char *argv[]){
srand((unsigned int)time(NULL));
printf("\ninserisci n\n");
scanf("%d",&n); while(getchar()!='\n');


int **matrix=(int **)calloc(n,sizeof(int *));
int i,j,ret;
for(i=0;i<n;i++){
	*(matrix+i)=(int *)calloc(n,sizeof(int));	
	for(j=0;j<n;j++){
			*(*(matrix+i)+j)=rand()%11;
			printf("%d ",matrix[i][j]);
	}
	printf("\n");

}



}
