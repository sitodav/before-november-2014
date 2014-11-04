#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>




void main(){

int i,childpid;
for(i=0;i<4;i++){

	childpid=fork();
	if(childpid==0)
		break;
}

printf("\nSono processo con pid %d in attesa di segnale\n",getpid());
pause();




}
