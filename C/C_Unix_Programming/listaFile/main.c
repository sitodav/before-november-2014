//elenca la tipologia dei file passati da riga di comando

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

int main(int argc,char *argv[]){
if(argc==1){
	printf("\nerr usage: path/filename\n");
	exit(0);
}

int i=1;
struct stat buf;
char *result;
while(i<argc){
	if(lstat(argv[i],&buf)<0){
		perror("\n");
		exit(1);
	}
	
	if(S_ISLNK(buf.st_mode)) 
		result=" e' un soft link\n";
	else if(S_ISREG(buf.st_mode))
		result=" e' un regular file\n";
	else if(S_ISDIR(buf.st_mode))
		result=" e' directory\n";
	else if(S_ISCHR(buf.st_mode))
		result=" e' un char device (es stream)\n";
	else if(S_ISSOCK(buf.st_mode))
		result=" e' un socket";
	else result=" e' un tipo sconosciuto!\n";
	
	printf("\nil file %s e' %s",argv[i],result);

	i++;
}




return 0;
}
