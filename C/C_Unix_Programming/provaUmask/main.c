#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

int main(){
	char nomefile[FILENAME_MAX];
	printf("\nInserisci nome file da creare\n");
	fgets(nomefile,FILENAME_MAX,stdin);
	
	
	int fdfile1;
	if((fdfile1=open(nomefile,O_CREAT | O_RDWR,S_IRUSR | S_IWUSR ))<0){
		perror("\n:");
		exit(1);	
	}


}
