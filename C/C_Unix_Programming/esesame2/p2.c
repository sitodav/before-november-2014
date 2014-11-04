#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>


int main(int argc,char *argv[]){
srand((unsigned int)time(NULL));
int fdFile,n,ret;
char buff[20];

if( (fdFile=open("temp.txt",O_WRONLY | O_CREAT, 00777))<0 ){
	perror("\n");
	exit(1);
}


while(1){
	sleep(1);
	n=1+rand()%100;
	printf("\ngenerato numero random, lo scrivo sul file insieme al mio pid\n");
	fflush(stdout);
	sprintf(buff,"%d,%d\n",getpid(),n); //miopid,n
	ret=write(fdFile,buff,strlen(buff)+1);
	if(ret<0){
		perror("\n");
		exit(1);	
	}
	printf("\nTrascritto\n");
	fflush(stdout);
	
}


return 0;
}
