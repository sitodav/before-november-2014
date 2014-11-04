#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>


//globali da sincronizzare
int fdFileRW;

//glob non da sincronizzare
int n;

pthread_mutex_t mutex1=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond1;


void * thread_scrittore(void *arg);
void * thread_lettore(void * arg);
void * thread_controllore(void * arg);

int main(int argc,char *argv[]){
if(argc!=2){
	printf("\nPassami n\n");
	exit(0);
}

sscanf(argv[1],"%d",&n);
int i,ret;

if((fdFileRW=open("temp.txt",O_RDWR | O_CREAT,00777))<0){
	perror("\n");
printf("\n36");
	exit(1);
}

pthread_t *tid=(pthread_t *)calloc(3,sizeof(pthread_t));




if((ret=pthread_create(tid+0,NULL,thread_scrittore,NULL))!=0){
	printf("\n%s",strerror(ret));
	exit(1);
	
}


if((ret=pthread_create(tid+1,NULL,thread_lettore,NULL))!=0){
	printf("\n%s",strerror(ret));
	exit(1);
	
}

if((ret=pthread_create(tid+2,NULL,thread_controllore,NULL))!=0){
	printf("\n%s",strerror(ret));
	exit(1);
	
}



//li attendo tutti e 3
for(i=0;i<3;i++)
	if((ret=pthread_join(tid[i],NULL))!=0){
		printf("%s",strerror(ret));
		printf("\n65"); 
		exit(1);	
	}

}

void * thread_scrittore(void * arg){
int i;
for(i=0;i<=n;i++){
	sleep(2);
	pthread_mutex_lock(&mutex1);
	lseek(fdFileRW,0,SEEK_SET);
	if(write(fdFileRW,&i,sizeof(int))<0){
		printf("\n79");
		perror("\n");
		exit(1);	
	}
	printf("\nSono thread scrittore, ho scritto %d\n",i);
	fflush(stdout);
	pthread_mutex_unlock(&mutex1);
	
}

pthread_cond_signal(&cond1); //segnalo perche' all'uscita dal for avremo scritto quello che dovevamo scrivere e ora tocca al controllore


}

void * thread_lettore(void * arg){
int t;
while(1){
	
	pthread_mutex_lock(&mutex1);
	lseek(fdFileRW,0,SEEK_SET);
	if(read(fdFileRW,&t,sizeof(int))<0){
		printf("\n101");
		perror("\n");
		exit(1);	
	}
	printf("\nSono thread lettore, letto %d\n",t); 
	fflush(stdout); 
	if(t==n){
		printf("\nSono thread lettore, esco!\n",t);
		fflush(stdout); 
		pthread_mutex_unlock(&mutex1);
		break;	
	}
	
	pthread_mutex_unlock(&mutex1);

}



}

void * thread_controllore(void * arg){
int t=-1;

pthread_mutex_lock(&mutex1);
do{
	lseek(fdFileRW,0,SEEK_SET);
	if(read(fdFileRW,&t,sizeof(int))<0){
		printf("\n128");
		perror("\n");
		exit(1);	
	}
	if(t==n) {
		pthread_mutex_unlock(&mutex1);
		break;
	}

	pthread_cond_wait(&cond1,&mutex1);
} while(1);

printf("\nOperazioni concluse, arrivederci dal thread %u\n",(unsigned int)pthread_self());
fflush(stdout);




}
