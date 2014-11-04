#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

int main(int argc,char *argv[]){
int i;
char **arg=(char **)calloc(7,sizeof(char *));
arg[0]=(char *)calloc(FILENAME_MAX,sizeof(char));
strcpy(arg[0],"./main");
for(i=1;i<6;i++){
	arg[i]=calloc(FILENAME_MAX,sizeof(char));
	printf("\nScrivi directory \n");
	fflush(stdout);
	fgets(arg[i],FILENAME_MAX,stdin);
	arg[i][strlen(arg[i])]='\0';
}
arg[6]=NULL;

execv("./main",arg);
printf("\nExec fallito!\n");
fflush(stdout);
exit(1);


}
