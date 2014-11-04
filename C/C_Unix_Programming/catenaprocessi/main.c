/*prog che crea una catena di n-1 processi figli passandogli il numero n da riga di comando */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc,char *argv[]){
if(argc<=1)
	exit(0);
int i,n=atoi(argv[1]),mychildid;
for(i=1;i<n;i++)
	if((mychildid=fork())!=0) //cioe' se facendo la fork, non sei il figlio, esci dal for
		break;

printf("\nmyPID:%d myrealUser:%d myeffectiveUser:%d \nmyRealGroup%d  myEffectiveGroup:%d  myPPID: %d \n\n\n",getpid(),getuid(),geteuid(),getgid(),getegid(),getppid());

return 0;









}
