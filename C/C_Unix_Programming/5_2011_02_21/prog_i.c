#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


int main(int argc,char *argv[]){
int toSleep;

if(argc!=2){
	printf("\n  n\n");
	exit(0);
}
sscanf(argv[1],"%d",&toSleep);
printf("\nsono Prog_i-->Aspetto %d secondi e termino!\n",toSleep);
fflush(stdout);
sleep(toSleep);
printf("\nIl programma Prog_i ha terminato\n");
fflush(stdout);
return 0;


}
