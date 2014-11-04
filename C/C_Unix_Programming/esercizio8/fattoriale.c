#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

int main(int argc,char *argv[],char *envp[]){
srand((int)time(NULL));
if(argc!=2) exit(0);

int nFatt=atoi(argv[1]),fattTot=1;
int i;
for(i=1;i<=nFatt;i++)
	fattTot*=i;
printf("\nIl fattoriale calcolato e' %d",fattTot);
//ritorniamo un exit status random tra 0 e 2
exit((rand()%3));

	







}
