#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

int main(int argc,char *argv[]) {

int sockfd,n;
char buffer[1025];
struct sockaddr_in strind;

if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0)
    {
        printf("\nERRORE CREAZIONE SOCKET\n");
    }
strind.sin_port=htons(13);
if(inet_pton(AF_INET,argv[1],&strind.sin_addr)<=0)
 printf("\nERRORE NEL CONVERTIRE LA STRINGA IP\n");
strind.sin_family=AF_INET;

if(connect(sockfd,(struct sockaddr *)&strind,sizeof(strind))<0)
    printf("\nERRORE CONNESSIONE");

printf("\nCONNESSIONE RIUSCITA\n");



}
