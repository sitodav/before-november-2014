#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[],char *envp[]){
int actualI;
int childpid;
char actualStringI[20];

actualI=atoi(argv[1]);
sprintf(actualStringI,"%d",++actualI);
printf("\n%d",actualI);
childpid=fork();
if(childpid==0){
	execl(argv[0],argv[0],actualStringI,NULL); //esegui me' stesso (argv[0]) e fai diventare l argv0 di me' stesso proprio argv[0] che ho adesso, e con ulteriore parametro in input actualStringI 

	printf("\nexecl fallita!\n");

}




}
