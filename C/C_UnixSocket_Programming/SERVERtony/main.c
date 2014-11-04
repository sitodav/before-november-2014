 #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
int main(int argc,char **argv)
{
    int listenfd,connfd,lenght=0;
    struct sockaddr_in servaddr;
    char buff[50];
    if((listenfd=socket(AF_INET,SOCK_STREAM,0))<0)
    {
        perror("socket"); exit(1);
    }
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    servaddr.sin_port=htons(10);
    if (bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)
    {
        perror("bind"); exit(1);
    }

if (listen(listenfd,1024)<0)
{
    perror("listen"); exit(1);
}

if((connfd=accept(listenfd,(struct sockaddr*)NULL,NULL))<0)
{
    perror("accept"); exit(1);
}

if(read(connfd,buff,50)<0)
{
     perror("read"); exit(1);
}
lenght=strlen(buff);

if (write(connfd,&lenght,sizeof(int))<0)
{
    perror("write"); exit(1);
}

close(connfd);
exit(0);

}
