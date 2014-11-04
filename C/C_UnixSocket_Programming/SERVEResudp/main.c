#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>

void sessione_server(int porta);

int main() {
int porta;
printf("\nInserire la porta su cui ricevere\n");
scanf("%d",&porta);
while(getchar()!='\n');
sessione_server(porta);
return 0;

}

void sessione_server(int porta) {
int udpsfd,ret,sizestructclient;
struct sockaddr_in clientind,nostrind;


nostrind.sin_family=AF_INET;
nostrind.sin_port=htons(porta);
char nostroip[]="127.0.0.1";
inet_pton(AF_INET,nostroip,&nostrind.sin_addr);

if((udpsfd=socket(AF_INET,SOCK_DGRAM,0))<0) {
    perror("\nImpossibile creare il socket udp:");
    exit(1);
}

if(bind(udpsfd,(struct sockaddr *)&nostrind,sizeof(nostrind))<0) {
    perror("\nImpossibile bindare il socket di ricezione su ip/porta decisa:");
}


sizestructclient=sizeof(clientind);
char toreceive[300]="";
do ret=recvfrom(udpsfd,toreceive,300,0,(struct sockaddr *)&clientind,&sizestructclient);
    while(ret<0 && errno==EINTR);
if(ret<0) {
    perror("Errore sulla recvfrom sul socket udp:");
    exit(1);
}

int k=0,cont=0;
while(toreceive[k]!='\0')
    {cont++;
      k++;
    }
int portaclient=ntohs(clientind.sin_port);

printf("\nLa parola ricevuta dal client e' lunga %d\n",cont);
printf("il client ha porta %d",portaclient);

char tosend[300]="";
sprintf(tosend,"%d",cont);
do ret=sendto(udpsfd,tosend,300,0,(struct sockaddr *)&clientind,sizestructclient);
    while(ret<0 && errno==EINTR);
if(ret<0){
    perror("\nErrore con la sendto() sul socket udp\n");
    exit(1);
}

printf("\nRisposta inviata al client\n");




}
