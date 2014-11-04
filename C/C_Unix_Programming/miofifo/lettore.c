#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>



int main(int argc,char *argv[]){

char toRead[100]="";
int fdFifoR,a,b;
if(argc!=2){
	printf("\nErrore input from line\n");
	exit(0);
}


if(access(argv[1],F_OK)==0)
	printf("\nLa fifo esiste !!!\n");
fflush(stdout);

if( (fdFifoR=open(argv[1],O_RDONLY))<0 ){
	perror("\n");
	exit(1);
}

printf("\nSOno lettore, aperta fifo su %d\n",fdFifoR);

int ret,parz=0;
do{
	ret=read(fdFifoR,toRead+parz,100);
	if(ret==-1 && errno==EINTR)
		continue;
	else if(ret==-1) perror("\n");
	parz+=ret;
}
	while(ret>0);

printf("\nSono lettore e ho letto \n%s\n",toRead);
sscanf(toRead,"%d,%d",&a,&b);
printf("\nTermino, e il mio exit status sara' il risultato di a+b\n"); fflush(stdout);
exit(a+b);








}
