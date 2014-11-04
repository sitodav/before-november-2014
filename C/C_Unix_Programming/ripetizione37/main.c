#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>




int n;

int main(int argc,char *argv[]){

if(argc!=2){
	printf("\nPassami n directory\n");
	exit(1);
}

sscanf(argv[1],"%d",&n);

char **matrix=(char **)calloc(n+2,sizeof(char *));
int i;
for(i=0;i<n;i++){
	*(matrix+i+1)=(char *)calloc(FILENAME_MAX,sizeof(char));
	printf("\nInserisci nome directory\n");
	fgets(*(matrix+i+1),FILENAME_MAX,stdin);
	*(*(matrix+i+1)+strlen(*(matrix+i+1))-1)='\0';

}

*matrix=calloc(FILENAME_MAX,sizeof(char));
strcpy(*(matrix),"./prog");
*(matrix+i+1)=NULL;


for(i=0;i<n;i++){
	printf("\nLavoro con %s\n",*(matrix+i+1));

}


execv("./prog",matrix);
printf("\nerrore nella exec!\n");
exit(1);





}
