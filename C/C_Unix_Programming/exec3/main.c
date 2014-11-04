#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int main(int argc,char *argv[],char *envp[]){
char *new_argv[argc+1];
int childpid,i=1;
int j;

if(argv[1]==NULL) 
		exit(0);

if((childpid=fork())<0){
	perror("\n"); exit(1);
}

else if(childpid==0){
	printf("\nExec per :%s \n",argv[1]);
	execlp(argv[1],argv[1],NULL);
	printf("\nExec per %s fallita\n",argv[1]);

}

else{	
	for(j=0;j<argc+1;j++)
		new_argv[j]=calloc(FILENAME_MAX,sizeof(char));
	
	strcpy(new_argv[0],argv[0]);
	new_argv[0][strlen(new_argv[0])]='\0';	
	
	while(argv[i+1]!=NULL){
				
		strcpy(new_argv[i],argv[i+1]);
		new_argv[i][strlen(new_argv[i])]='\0';
		i++;
	}
	
	
	if(i==1)
		exit(0);
	
	new_argv[i]=NULL;	
	printf("\nLancio exec di me' stesso (sono il padre)\n");
	execve(argv[0],new_argv,envp);
	printf("\nFallita exec per richiamare ricorsivamente me stesso\n");	

}


return 0;
}
