#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>


//simulo con pipeline il cmd : ls -R <dir> | grep <nomefile> | uniq | cat


int main(int argc,char *argv[]){

if(argc!=3){
	printf("\nuso: <dir> <nomefile>\n");
	exit(1);
}

int fdPipe1[2],fdPipe2[2],fdPipe3[2];

if(pipe(fdPipe1)<0 || pipe(fdPipe2)<0 || pipe(fdPipe3)<0){
    perror("\n");
    exit(1);
}


int childpid[3],i;

for(i=0;i<3;i++){
	childpid[i]=fork();
	if(childpid[i]==0) break;
}

if(i==3){
	printf("\nSono padre\n");
	fflush(stdout);

	close(fdPipe1[0]);
	close(fdPipe2[0]);
	close(fdPipe2[1]);
	close(fdPipe3[0]);
	close(fdPipe3[1]);

	if(dup2(fdPipe1[1],1)<0){
		perror("\n");
		exit(1);
	}

	execlp("ls","ls","-R",argv[1],NULL);
	printf("\nexeclp fallita in PADRE\n");



}

else if(i==0){
	printf("\nSono primo figlio\n");
	fflush(stdout);
	close(fdPipe1[1]);
	close(fdPipe2[0]);
	close(fdPipe3[0]);
	close(fdPipe3[1]);

	if(dup2(fdPipe1[0],0)<0){
		perror("\n");
		exit(1);
	}
	if(dup2(fdPipe2[1],1)<0){
		perror("\n");
		exit(1);
	}
	execlp("grep","grep",argv[2],NULL);
	printf("\nexeclp fallita in primo figlio\n");

}

else if(i==1){
	printf("\nSono secondo figlio\n");
	fflush(stdout);
	close(fdPipe1[0]);
	close(fdPipe1[1]);
	close(fdPipe2[1]);
	close(fdPipe3[0]);

	if(dup2(fdPipe2[0],0)<0){
		perror("\n");
		exit(1);
	}
	if(dup2(fdPipe3[1],1)<0){
		perror("\n");
		exit(1);
	}
	execlp("uniq","uniq",NULL);
	printf("\nexeclp fallita in secondo figlio\n");


}

else if(i==2){
	printf("\nSono terzo figlio\n");
	fflush(stdout);

	close(fdPipe1[0]);
	close(fdPipe1[1]);
	close(fdPipe2[0]);
	close(fdPipe2[1]);
	close(fdPipe3[1]);

	if(dup2(fdPipe3[0],0)<0){
		perror("\n");
		exit(1);
	}
	execlp("cat","cat",NULL);
	printf("\nexeclp fallita in terzo figlio\n");

}



}

