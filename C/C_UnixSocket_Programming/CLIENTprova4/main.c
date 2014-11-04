#include <stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

int main(int argc,char *argv[]) {
int connsfd;
int nostraporta,portaserver;
struct sockaddr_in ind_server;
struct sockaddr_in nostroind_tobind; //in questa struct ci sono ip e porta che vogliamo che ci siano assegnati dal kernel per quel socket in dialogo col srv
struct sockaddr_in nostroind_verifica; //qui mettiamo ip e porta nostri ottenuti lanciando gethostname()
if(argc!=3)
    {printf("\n./COMMAND NAME <IP srv> <IP nostro>\n");
     exit(1);
    }
printf("\nInserire la porta del server\n");
scanf("%d",&portaserver);
printf("\nInserire la porta dalla quale uscire\n");
scanf("%d",&nostraporta);


if((connsfd=socket(AF_INET,SOCK_STREAM,0))<0)
    {
        printf("\nImpossibile creare il socket per la connessione\n");
        exit(1);
    }

inet_pton(AF_INET,argv[1],&ind_server.sin_addr); //riempiamo la struct con i dati del server che useremo per connetterci a questo
ind_server.sin_family=AF_INET;
ind_server.sin_port=htons(portaserver);

//e riempiamo la struct che riguarda i dati che vogliamo che siano assegnati al nostro socket (cioe' l'ip di interfaccia
//sul quale risulteremo all'esterno e la porta dalla quale vogliamo uscire sulla rete)

inet_pton(AF_INET,argv[2],&nostroind_tobind.sin_addr);
nostroind_tobind.sin_port=htons(nostraporta);
nostroind_tobind.sin_family=AF_INET;

//ora diamo un indirizzo e porta scelti da noi al nostr osocket usando la struct nostroind_tobind, e poi lo facciamo
//collegare all'end point identificato da IP e PORTA contenuti nella struct ind_server

if(bind(connsfd,(struct sockaddr *)&nostroind_tobind,sizeof(nostroind_tobind))<0)
    {
        printf("\nImpossibile assegnare la porta attraverso cui uscire al processo socket\n");
        exit(1);
    }

if(connect(connsfd,(struct sockaddr *)&ind_server,sizeof(ind_server))<0)
    {printf("\nImpossibile collegarsi all'IP:PORTA (endpoint esterno) specificati\n");
     exit(1);
    }
else {printf("\nCONNESSIONE RIUSCITA");
        //lanciamo la getsockname() per verificare che l'indirizzo e la porta che abbiamo bindato sul socket (dal nostro lato)
        //corrispondano ai valori effettivamente assunti dal socket
        socklen_t len=sizeof(nostroind_verifica);
        char nostroip[9];
        getsockname(connsfd,(struct sockaddr *)&nostroind_verifica,&len);
        printf("\nSTIAMO USCENDO DALLA PORTA:%d\n",ntohs(nostroind_verifica.sin_port));
        inet_ntop(AF_INET,&nostroind_verifica.sin_addr,nostroip,sizeof(nostroind_verifica)); //nb che nell'inet_XX si passa il valore della dimensione della struct nella quale salvare i valori
        printf("IL NOSTRO IP IN USCITA:%s",nostroip);                                                                                                                        //mentre per la getXXname() bisogna passare una variabile per puntatore!
        printf("\nPROTOCOLLO:%d\n",nostroind_verifica.sin_family);


       }
    exit(0); }
