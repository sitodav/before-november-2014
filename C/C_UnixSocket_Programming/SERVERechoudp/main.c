#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <arpa/inet.h>

int main() {
int udpsfd,nostraporta;
printf("\nIns nostra porta\n");
scanf("%d",&nostraporta);
while(getchar()!='\n');
struct sockaddr_in nostrind;
struct sockaddr_in clientind;
int sizeclientind;

nostrind.sin_family=AF_INET;
nostrind.sin_port=htons(nostraporta);
nostrind.sin_addr.s_addr=htons(INADDR_ANY);

clientind.sin_family=AF_INET;

if((udpsfd=socket(AF_INET,SOCK_DGRAM,0))<0)
    {
        printf("\nImpossibile creare il socket "); exit(1);
    }

if(bind(udpsfd,(struct sockaddr *)&nostrind,sizeof(nostrind))<0)
    {
        printf("\nImpossibile bindare il socket udp\n");
        exit(1);
    }
int r;
char toreceive[300]="";
//creato il socket udp, bindato, ora dobbiamo solo ricevere
for(;;){

r=recvfrom(udpsfd,toreceive,300,0,(struct sockaddr *)&clientind,&sizeclientind); //dobbiamo salvarci ind e size del mittente, poiche' a quello dobbiamo reinviarlo
perror("\nerrore");
printf("\nletti %d char da client\n",r);
perror("\nerrore");
fflush(stdout);


r=sendto(udpsfd,toreceive,r,0,(struct sockaddr *)&clientind,sizeclientind);
perror("\nerrore");
printf("\nreinviati %d char allo stesso client\n",r);








}




}
