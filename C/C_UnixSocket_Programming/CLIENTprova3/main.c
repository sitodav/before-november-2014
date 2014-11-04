#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

int main(int argc,char  *argv[]) {
int porta;
int nw,nr;
char tosay[100]=" ",tohear[100]=" ";
char buffer[100];
if(argc!=2)
    {
        printf("<IP>"); exit(1);
    }
int connsockfd;
if((connsockfd=socket(AF_INET,SOCK_STREAM,0))<0)
    {
        printf("\nERRORE CREAZIONE SOCKET\n"); exit(1);
    }
printf("\nINSERISCI PORTA\n");
scanf("%d",&porta);
struct sockaddr_in strind;
strind.sin_port=htons(porta);
strind.sin_family=AF_INET;
inet_pton(AF_INET,argv[1],&strind.sin_addr);

char ipnoi[]="192.168.0.54";
struct sockaddr_in struttnoi;
struttnoi.sin_family=AF_INET;
struttnoi.sin_port=htons(15);
inet_pton(AF_INET,ipnoi,&struttnoi.sin_addr);
bind(connsockfd,(struct sockaddr *)&struttnoi,sizeof(struttnoi));

if(connect(connsockfd,(struct sockaddr *)&strind,sizeof(strind))<0)
    {
        printf("\nImpossibile stabilire la connessione\n");
        exit(1);
    }
else printf("\nCONNESSIONE STABILITA\n");

//



//

}
