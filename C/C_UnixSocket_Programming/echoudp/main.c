#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <arpa/inet.h>

int main() {

int udpsfd;
struct sockaddr_in servind;
int portasrv;
char ipsrv[INET_ADDRSTRLEN];
printf("\nIns ip server\n");
fgets(ipsrv,INET_ADDRSTRLEN,stdin);
ipsrv[strlen(ipsrv)-1]='\0';
printf("\nIns porta server\n");
scanf("%d",&portasrv);
while(getchar()!='\n');
char tosend[300]="",toreceive[300]="";
int sizestructsrvind;


servind.sin_port=htons(portasrv);
inet_pton(AF_INET,ipsrv,&servind.sin_addr);
servind.sin_family=AF_INET;
sizestructsrvind=sizeof(servind);

if((udpsfd=socket(AF_INET,SOCK_DGRAM,0))<0  )
        {printf("\nImpossibile aprire il socket\n"); exit(1); }

printf("\nInserire messaggio da inviare\n");
fgets(tosend,300,stdin);
char prova[INET_ADDRSTRLEN];
inet_ntop(AF_INET,&servind.sin_addr,prova,INET_ADDRSTRLEN);
printf("\nStiamo inviando all'indirizzo %s\n",prova);
fflush(stdout);

int i=0;
while(i<3)
    {
        sendto(udpsfd,tosend,300,0,(struct sockaddr *)&servind,sizestructsrvind);

        recvfrom(udpsfd,toreceive,300,0,NULL,NULL); //non ci interessa salvare l'ind del server in ricezione, visto che gia' lo sappiamo

        printf("\nRicevuto:%s",toreceive);
        fflush(stdout);
        i++;
    }



return 0;

}
