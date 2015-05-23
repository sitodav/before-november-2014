#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

int main() {

int connsfd1,connsfd2,porta=1025;
struct sockaddr_in noi1,noi2;

connsfd1=socket(AF_INET,SOCK_STREAM,0);
connsfd2=socket(AF_INET,SOCK_STREAM,0);

noi1.sin_family=AF_INET;
noi1.sin_port=htons(porta);
inet_pton(AF_INET,"127.0.0.1\0",&noi1.sin_addr);

noi2.sin_family=AF_INET;
noi2.sin_port=htons(porta);
inet_pton(AF_INET,"127.0.0.1\0",&noi2.sin_addr);

if(bind(connsfd1,(struct sockaddr *)&noi1,sizeof(noi1))<0) perror("");
    else printf("ok");
if(bind(connsfd2,(struct sockaddr *)&noi2,sizeof(noi2))<0) perror("");
    else printf("ok2");





}
