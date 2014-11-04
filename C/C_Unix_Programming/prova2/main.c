#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>


ssize_t full_read(int fd,void *vptr,size_t n) {
size_t nleft;
ssize_t nread;
char *ptr;

ptr=vptr;
nleft=n;
while(nleft>0) {
    if( (nread=read(fd,ptr,nleft)) <0) {
        if (errno==EINTR)
            nread=0;
        else
            return (-1);


    }
   else if (nread==0)
                       break;
    nleft-= nread;
    ptr += nread;

}
return (n-nleft);

}




void main(){
int childpid;
int fdpipe[2],ret;
pipe(fdpipe);
char buff[100];
fgets(buff,100,stdin);

childpid=fork();
if(childpid==0){
	full_read(fdpipe[0],buff,100);
	printf("\nSono figlio e son ouscito dal while\n");
	fflush(stdout);
}


else 
printf("\nScrivo\n");

write(fdpipe[1],buff,strlen(buff));
while(1);


}
