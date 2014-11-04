#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>

int main(int argc, char **argv)
{
int
listenfd, connfd;
struct sockaddr_in servaddr;
char
buff[4096];
time_t ticks;
///////////////////////////////////////////////////////////////
if ( ( listenfd = socket(AF_INET, SOCK_STREAM, 0) ) < 0 ) {
perror("socket");
exit(1);
}
///////////////////////////////////////////////////////////////
servaddr.sin_family= AF_INET;
servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
servaddr.sin_port= htons(13);
///////////////////////////////////////////////////////////////
if ( bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 ) {
perror("bind");
exit(1);
}
//////////////////////////////////////////////////////////////
if ( listen(listenfd, 1024) < 0 ) {
perror("listen");
exit(1);
}
/////////////////////////////////////////////////////////////
for ( ; ; ) {
////////////////////////////////////////////////////////////
if ( ( connfd = accept(listenfd, (struct sockaddr *) NULL, NULL) ) < 0 ) {
perror("accept");
exit(1);
}
//////////////////////////////////////////////////////////////
snprintf(buff,sizeof(buff),"CIAO GAY");
if ( write(connfd, buff, strlen(buff)) != strlen(buff)) {
perror("write");
exit(1);
}
//////////////////////////////////////
close(connfd);
///////////////////////////////////////
}
}
