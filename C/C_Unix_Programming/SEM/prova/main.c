#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
#define SEM_MUTUAL "/sem5"
#define SEM_MUTUAL2 "/sem4"


sem_t *sem1;
sem_t *sem2;
int tot=0;

void * thread_start(void *arg);
void * thread2_start(void *arg);
int main(int argc,char *argv[]){

if((sem1=sem_open(SEM_MUTUAL,O_CREAT | O_EXCL,00777,0))==SEM_FAILED){
	perror("\n");
	exit(1);
}

if((sem2=sem_open(SEM_MUTUAL2,O_CREAT | O_EXCL,00777,1))==SEM_FAILED){
	perror("\n");
	exit(1);
}

pthread_t uno,due;
int ret;

if((ret=pthread_create(&uno,NULL,thread_start,NULL))!=0){
	printf("%s",strerror(ret));
	exit(1);
}

if((ret=pthread_create(&due,NULL,thread2_start,NULL))!=0){
	printf("%s",strerror(ret));
	exit(1);
}

if((ret=pthread_join(uno,NULL))!=0 || (ret=pthread_join(due,NULL))!=0){
	printf("%s",strerror(ret));
	exit(1);
}

}

void * thread_start(void *arg){

while(1){
	sleep(2);
	sem_wait(sem1);
	tot++;
	sem_post(sem2);
	
}

}

void * thread2_start(void *arg){

while(1){
	sem_wait(sem2);
		
	printf("%d",tot);
	fflush(stdout);
	sem_post(sem1);
	

}

}
