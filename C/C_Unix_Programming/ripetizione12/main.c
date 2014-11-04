#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>




float miaPow(float base,int exp);
int main(int argc,char *argv[]){

if(argc!=2){
	printf("\nNMAX\n"); exit(1);
}

int nMax;
sscanf(argv[1],"%d",&nMax);


int actualLvl=0;
int childpid[3],i;


while(actualLvl!=nMax){
	for(i=0;i<3;i++){
		childpid[i]=fork();
		if(childpid[i]==0)
			break;
	}
	if(i==3) //padre
		break;
	actualLvl++;


}


printf("\nSono processo pid %d e sto al livello %d\n",getpid(),actualLvl);
fflush(stdout);

for(i=0;i<3;i++)
	wait();

sleep(actualLvl);
printf("\n%f",miaPow((getpid()/100),i));
fflush(stdout);
exit(123);


}



float miaPow(float base,int exp){


int tot=1;

int i=exp;
while(i>0){
  tot*=base;
  i--;
}

return tot;

}
