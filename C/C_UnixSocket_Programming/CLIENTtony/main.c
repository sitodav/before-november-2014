#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
int main(int argc,char **argv)
{
    int sockfd,n;
    char buffer[50];
    struct sockaddr_in servaddr;
    if (argc!=2)
    {
        fprintf(stderr,"usage:%s<IPaddress>",argv[0]);
        exit(1);
    }

if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0)
{
    perror("socket"); exit(1);
}


servaddr.sin_family=AF_INET;
servaddr.sin_port=htons(10);
if(inet_pton(AF_INET,argv[1],&servaddr.sin_addr)<=0)
{
    fprintf(stderr,"inet_pton error for %s\n",argv[1]);
}

if (connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)
{
    fprintf(stderr,"connect error\n");
    exit(1);
}

printf("\nInserisci stringa\n");
if (fgets(buffer,50,stdin)==NULL)
{
    perror("fgets"); exit(1);
}

if(write(sockfd,buffer,strlen(buffer))!=strlen(buffer))
{
    perror("write"); exit(1);
}

if(read(sockfd,&n,sizeof(int))<0)
{
    perror("read"); exit(1);
}

printf("La lunghezza della stringa%s e' %d",buffer,n);
exit(0);
}
