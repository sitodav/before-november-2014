#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>




int main(int argc,char *argv[]){

char toWrite[100]="";
int fdFifoW,a,b;
if(argc!=2){
	printf("\nErrore input from line\n");
	exit(0);
}


if(access(argv[1],F_OK)==0)
	printf("\nTutto ok, la fifo era stata correttamente creata dal padre\n");

if( (fdFifoW=open(argv[1],O_WRONLY))<0 ){
	perror("\n");
	exit(1);
}

printf("\nInserire primo numero\n"); fflush(stdout);
scanf("%d",&a);
printf("\nInserire secondo numero\n"); fflush(stdout),
scanf("%d",&b); while(getchar()!='\n');

sprintf(toWrite,"%d,%d",a,b);
printf("\nInvio %d,%d\n",a,b); fflush(stdout);
printf("\nInviati %d bytes\n",write(fdFifoW,toWrite,strlen(toWrite)));
fflush(stdout);

close(fdFifoW);
return 0;








}
