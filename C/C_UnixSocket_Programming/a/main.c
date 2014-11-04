#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <fcntl.h>
#define port 1236
#define SIZE_RCV_BUFF 1000


int main(){
int udpsock=socket(AF_INET,SOCK_DGRAM,0);
struct sockaddr_in nostrind,clientind;
int sizeclient=sizeof(clientind);

nostrind.sin_family=AF_INET;
nostrind.sin_port=htons(1234);
nostrind.sin_addr.s_addr=htons(INADDR_ANY);
bind(udpsock,(struct sockaddr *)&nostrind,sizeof(nostrind));
int fdfile;
int ret,tot=0;
char *buff;
while(1){
	buff=calloc(SIZE_RCV_BUFF,sizeof(char));
	fdfile=open("file.txt",O_CREAT | O_WRONLY | O_APPEND,00777);
	//bzero(buff,SIZE_RCV_BUFF);
	ret=recvfrom(udpsock,buff,SIZE_RCV_BUFF,MSG_WAITALL,(struct sockaddr *)&clientind,&sizeclient);
	tot+=ret;
	printf("\n%dBYTES",tot);fflush(stdout);
			
	write(fdfile,buff,strlen(buff));
	close(fdfile);
	free(buff);
}

}
