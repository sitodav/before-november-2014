#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(int argc,char *argv[]){

int status;
int pid_deadchild;

if(fork()==0){
	printf("\nSono il figlio nasco, e muoio!\n");
	exit(12);
}
pid_deadchild=wait(&status);
printf("\nE' morto il mio processo figlio con pid %d\n",pid_deadchild);
if(WIFEXITED(status))
	printf("\nE' terminato volontariamente con stato di terminazione %d",WEXITSTATUS(status));
if(WIFSIGNALED(status))
	printf("\nE' stato interrotto dal segnale %d",WTERMSIG(status));
	


return 0;}

