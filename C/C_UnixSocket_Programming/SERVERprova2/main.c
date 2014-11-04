#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

int main(int argc,char *argv[])
{
 int listensockfd,connsockfd;
 struct sockaddr_in struttind;
 if((listensockfd=socket(AF_INET,SOCK_STREAM,0))<0)
        {
            printf("\nERRORE APERTURA SOCKET DI ASCOLTO\n");
            exit(1);
        }
 struttind.sin_port=htons(13);
 struttind.sin_family=AF_INET;
 struttind.sin_addr.s_addr=htons(INADDR_ANY);

 if(bind(listensockfd,(struct sockaddr *)&struttind,sizeof(struttind))<0)
    {
        printf("\nERRORE NELL'ASSEGNAZIONE PORTA e/o IP AL SOCKET DI ASCOLTO\n");
        exit(1);
    }
 if(listen(listensockfd,5)<0)
        {printf("\nERRORE: IL SOCKET NON RIESCE A METTERSI IN ASCOLTO\n");
         exit(1);
        }
 while(1)
    {
       printf("\nIN ATTESA DI RICHIESTE DI CONNESSIONE...\n");
       if((connsockfd=accept(listensockfd,NULL,NULL)) <0)
        {printf("\nCONNESSIONE ARRIVATA, MA IMPOSSIBILE COMPLETARLA\n");
         exit(1);
        }
       printf("\nCONNESSIONE RIUSCITA\n..CHIUSURA CONNESSIONE IN CORSO\n");
       close(connsockfd);



    }

}
