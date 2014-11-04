#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

int main() {

int listensfd,connsfd,ourport;
char ip_altro[9]; //stringa per contenere l'ip del client
struct sockaddr_in our_addr; //struttura per i nostri parametri (noi binderemo il socket su questa)
struct sockaddr_in altro_addr; //struttura per contenere i parametri del client ottenuti lanciando la getsockname()
socklen_t len=sizeof(altro_addr);
printf("\nInserisci la porta TCP sulla quale ricevere connessioni\n");
scanf("%d",&ourport);


if((listensfd=socket(AF_INET,SOCK_STREAM,0))<0)
    {printf("\nErrore creazione listen socket\n");
     exit(1);
    }

our_addr.sin_port=htons(ourport);
our_addr.sin_family=AF_INET;
our_addr.sin_addr.s_addr=htons(INADDR_ANY);

if(bind(listensfd,(struct sockaddr *)&our_addr,sizeof(our_addr))<0)
    {printf("\nImpossibile assegnare la porta inserita al socket di ascolto\n");
     exit(1);
    }

if(listen(listensfd,1024)<0)
    {printf("\nIl socket non riesce a mettersi in ascolto sulla sua porta\n");
     exit(1);
    }

//il programma qui va in pausa fino a quando non arriva almeno una connessione e questa non venga accettata correttamente
char nickmittente[15]="";
char tosend[300]="";
char toreceive[300]="";
printf("\nIn attesa di connessioni\n");
        if((connsfd=accept(listensfd,NULL,NULL))<0) //i dati del client connesso non li recuperiamo con la accept, ma tramite la getsockname()
            {
                printf("\nErrore nella connessione\n");
            }
for(;;)
    {


                  //recupera informazioni del client
                  strcpy(tosend,"masteradmin");
                  strcpy(tosend+14,"TI SPAMMO PAROLA CIAO");
                  tosend[36]='\0';
                  write(connsfd,tosend,300);







                  //////////////lavora col client/////////////




        }

}
