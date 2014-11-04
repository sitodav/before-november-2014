#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>



int main(int argc,char *argv[]){

if(argc!=2){
	printf("\npassami n\n"); 
	exit(1);
}

int n;

sscanf(argv[1],"%d",&n);

int i,j;
int **matrix=(int **)calloc(n,sizeof(int *));
for(i=0;i<n;i++){
	matrix[i]=(int *)calloc(n,sizeof(int));
	for(j=0;j<n;j++){
		*(*(matrix+i)+j)=rand()%10;
		printf("%d ",matrix[i][j]);
		
	}
	printf("\n");
}

int childpid[4];

for(i=0;i<4;i++){
	childpid[i]=fork();
	if(childpid[i]==0)
		break;
}


if(i==4){
	int *results=(int *)calloc(4,sizeof(int));
	for(i=0;i<4;i++){
		waitpid(childpid[i],results+i,0);
		results[i]=WEXITSTATUS(results[i]);
		printf("->%d",results[i]);
		fflush(stdout);
	}
	int max=results[0];
	for(i=1;i<4;i++)
		if(*(results+i)>max)
			max=results[i];
	printf("\nIl massimo e' %d\n",max);
	fflush(stdout);
	return;

}

else if(i==0){
	
	int mySum=0;
	for(i=0;i<n;i++)
		mySum+=matrix[n/2][i];
	exit(mySum);

}

else if(i==1){
	
	int mySum=0;
	for(i=0;i<n;i++)
		mySum+=matrix[i][n/2];
	exit(mySum);
}

else if(i==2){
	
	int mySum=0;
	for(i=0;i<n;i++)
		mySum+=matrix[i][i];
	exit(mySum);
}

else if(i==3){
	
	int mySum=0;
	for(i=0;i<n;i++){
		
		mySum+=matrix[i][n-1-i];
	}
	exit(mySum);
}



}
