#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>

int main(int argc,char *argv[]) {
int udpsfd;

printf("\nAvviato verso server %s\n",argv[1]);
struct sockaddr_in indintercettato;
indintercettato.sin_family=AF_INET;
indintercettato.sin_port=htons(22);
inet_pton(AF_INET,argv[1],&indintercettato.sin_addr);

udpsfd=socket(AF_INET,SOCK_DGRAM,0);

int res=3;
char tosend[300];
sprintf(tosend,"%d",res);
sendto(udpsfd,tosend,300,0,(struct sockaddr *)&indintercettato,sizeof(indintercettato));



}
