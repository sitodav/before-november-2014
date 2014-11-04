#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>

int main() {
//questo programma accetta una connessione da un client e ne stampa il nome
//per stamparne il domain name, dobbiamo risolvere una query col dns passandogli l'ip del client.
//Pero' per ottenere l'ip del client, o usiamo l'output della chiamata alla accept nel momento in accettiamo una connessione,
//oppure utilizziamo la getpeername, e poi passeremo il tutto alla gethostbyaddr

struct sockaddr_in nostroind,clientind; //struct indirizzo sul quale binderemo il nostro socket, e struct ind nella quale metteremo i dati risultanti dal getpeername (cioe' ip e porta dell'altro end point connesso al socket)
struct hostent *ris_querydns; //qui invece metteremo i valori restituiti dalla gethostbyaddr
int connsfd,listensfd;

if((listensfd=socket(AF_INET,SOCK_STREAM,0))<0)
    {
        printf("\nImpossibile aprire il socket di ascolto\n");
        exit(1);
    }
//ora riempiamo la struct dei nostri dati (ip e porta) e ci bindiamo sopra il nostro socket

nostroind.sin_family=AF_INET;
nostroind.sin_port=htons(13);
nostroind.sin_addr.s_addr=htons(INADDR_ANY);

//bindiamo
if(bind(listensfd,(struct sockaddr *)&nostroind,sizeof(nostroind))<0)
    {
        printf("\nImpossibile assegnare al socket di ascolto la porta di ricezione\n");
        exit(1);
    }

//e mettiamo in ascolto il socket
listen(listensfd,1024);
while(1)
    {
        printf("\nIn attesa...\n");
        if((connsfd=accept(listensfd,NULL,NULL))<0) //non usiamo l'accept per ottenere l'ip del client
            {
                printf("\nImpossibile concludere la connessione ricevuta\n");
                exit(1);
            }
        printf("\nConnessione stabilita\n");
        socklen_t len1=sizeof(clientind);
        getpeername(connsfd,(struct sockaddr *)&clientind,&len1);
        socklen_t len=sizeof(clientind.sin_addr);
        ris_querydns=gethostbyaddr(&(clientind.sin_addr),len,AF_INET);
        printf("\nIl client a noi connesso ha domain name: ");
        printf("%s",ris_querydns->h_name);



    }


                }
