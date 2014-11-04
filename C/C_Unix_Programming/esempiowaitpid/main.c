#include "mioheader.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

void handler(int signal);


int main(int argc,char *argv[]){

signal(SIGCHLD,handler);

if(fork()!=0)
	printf("\nSONO IL PADRE\n");
	
else return 12;

sleep(5);
return 0;






}


void handler(int signal){
printf("\nRICEVUTO SIG_CHLD");
fflush(stdout);


}
