//il client manda una parola al server, e questo rimanda indietro il numero di caratteri di cui e' composta la parola
//I/O Blocking di default, server concorrente

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>

int main() {
int connsfd,listensfd,porta,ret;
struct sockaddr_in nostroind;
printf("\nSu quale porta riceviamo ? \n");
scanf("%d",&porta);

nostroind.sin_family=AF_INET;
nostroind.sin_port=htons(porta);
nostroind.sin_addr.s_addr=htons(SINADDR_ANY);

if((listensfd=socket(AF_INET,SOCK_STREAM,0))<0) //la socket non puo' essere interrotta per un SIGNAL, quindi se ritorna <0 e' sicuramente un vero e proprio errore e non ha senso riavviarla
    {printf("\nImpossibile creare il socket\n");
     exit(1);
    }

do{ret=bind(listensfd,(struct sockaddr *)&nostroind,sizeof(nostroind)); }  //la bind() puo' essere interrotta da un SIGNAL, quindi :
    while(ret<0 && errno==EINTR  );


                }
