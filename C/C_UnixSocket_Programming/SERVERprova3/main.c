#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

int main() {
int listensockfd,connsockfd;
struct sockaddr_in indric;
char buffer[100]=" ";
int nr,nw;
char tosay[100]=" ",tohear[100]=" ";

int porta;
printf("\nInserire la porta su cui ricevere connessioni\n");
scanf("%d",&porta);

indric.sin_port=htons(porta);
indric.sin_family=AF_INET;
indric.sin_addr.s_addr=htons(INADDR_ANY);

if((listensockfd=socket(AF_INET,SOCK_STREAM,0))<0)
    {
        printf("\nIMPOSSIBILE CREARE IL SOCKET DI ASCOLTO\n");
    }
else printf("\nSOCKET RICEZIONE CREATO\n");

if(bind(listensockfd,(struct sockaddr *)&indric,sizeof(indric))<0)
    {
        printf("\nIL SOCKET DI ASCOLTO NON RIESCE AD ACCEDERE ALLA PORTA DI RICEZIONE\n");
        exit(1);
    }
else printf("\nIL SOCKET DI ASCOLTO HA AVUTO ACCESSO ALLA PORTA DI COMUNICAZIONE %d\n",porta);



if(listen(listensockfd,128)<0)
    {
        printf("\nIL SOCKET NON RIESCE A METTERSI IN ASCOLTO\n");
        exit(1);

    }
else printf("\nSOCKET IN ASCOLTO!\n");

struct sockaddr_in cliaddr;
socklen_t len=sizeof(struct sockaddr);
char stripclient[8];
int portaclient;
for(;;)
    {
        printf("\nIN ATTESA DI CONNESSIONI...\n");
        connsockfd=accept(listensockfd,(struct sockaddr *)&cliaddr,&len);

        printf("\nCONNESSIONE STABILITA\n");

    //
       inet_ntop(AF_INET,&indric.sin_addr,stripclient,len);
       portaclient=ntohs(cliaddr.sin_port);
       printf("\nCONNESSIONE DA IP:%s PORTA:%d\n",stripclient,portaclient);


    //

        close(connsockfd);
    }


}
