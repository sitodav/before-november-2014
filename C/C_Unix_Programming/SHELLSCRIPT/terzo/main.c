#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(){
int fdFile;
if((fdFile=open("prova.txt",O_CREAT | O_WRONLY,00777))<0){
	perror("\n");
	exit(1);

}


int i=0; char c;
c='a';
while(i<4000){
	if(write(fdFile,&c,1)<0){
		perror("\n");
		exit(1);	
	}
	i++;

}

printf("prova.txt"); fflush(stdout);


}
