#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int main(int argc,char *argv[]) {
int connsfd;
int porta;
struct sockaddr_in strind;
if(argc!=2)
    {
        printf("\nINS <IP>\n");
        exit(1);
    }
printf("\nIns porta\n");
scanf("%d",&porta);
strind.sin_family=AF_INET;
strind.sin_port=htons(porta);
inet_pton(AF_INET,argv[1],&strind.sin_addr,sizeof(strind.sin_addr));
if((connsfd=socket(AF_INET,SOCK_STREAM,0))<0)
    {printf("\nImpossibile creare il socket\n");
     exit(1);
    }
if(connect(connsfd,(struct sockaddr *)&strind,sizeof(strind))<0)
    {printf("\nImpossibile connettersi all'end point\n");
     exit(1);
    }
printf("\nCONNESSO\n");

char buffer[300];
int n;
n=read(connsfd,buffer,299);
printf("\ncar letti %d\n",n);
puts(buffer);

                }
