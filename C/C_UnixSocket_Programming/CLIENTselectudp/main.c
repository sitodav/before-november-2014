//un app client server, dove il client usa un socket udp non connesso, e il server usa un solo socket udp per ricevere tutti i messaggi
//il client manda uuna stringa, e il server risponde dicendo quanto e' lunga
//il client controlla che il messaggio di risposta sia stato mandato dallo stesso peer(server) a cui ha inviato la stringa iniziale

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <sys/select.h>


int main() {
int portasrv,ret,nostraporta,udpsfd,sizeserver,sizemittenterisposta,risultato;
struct sockaddr_in servind,nostrind,mittenterispostaind;
char servip[INET_ADDRSTRLEN],nostrip[INET_ADDRSTRLEN],*tosend,*toreceive;


printf("\nInserire ip del server: ");
fgets(servip,INET_ADDRSTRLEN,stdin);
servip[strlen(servip)-1]='\0';

printf("\nInserire porta del server: ");
scanf("%d",&portasrv); while(getchar()!='\n');

printf("\nInserire ip (di interfaccia) col quale uscire\n");
fgets(nostrip,INET_ADDRSTRLEN,stdin);
nostrip[strlen(nostrip)-1]='\0';

printf("\nInserire porta dalla quale uscire\n");
scanf("%d",&nostraporta); while(getchar()!='\n');

//riempiamo le struct degli indirizzi dei 2 end (noi e il peer)
nostrind.sin_port=htons(nostraporta);
nostrind.sin_family=AF_INET;
inet_pton(AF_INET,nostrip,&nostrind.sin_addr);

servind.sin_family=AF_INET;
servind.sin_port=htons(portasrv);
inet_pton(AF_INET,servip,&servind.sin_addr);

//creiamo il nostro socket udp, e lo bindiamo
if((udpsfd=socket(AF_INET,SOCK_DGRAM,0))<0){
        perror("\nImpossibile aprire il socket udp:");
        exit(1);
}

if(bind(udpsfd,(struct sockaddr *)&nostrind,sizeof(nostrind))<0){
        perror("Impossibile bindare il socket udp:");
        exit(1);
}

for(;;){
                    //prendiamo la parola da inviare
                    tosend=calloc(300,sizeof(char));
                    printf("\nInserire parola da inviare: ");
                    fgets(tosend,300,stdin);
                    tosend[strlen(tosend)-1]='\0';

                    //la inviamo sul socket udp connectionless
                    sizeserver=sizeof(servind);


                    do ret=sendto(udpsfd,tosend,300,0,(struct sockaddr *)&servind,sizeserver);
                        while(ret<0 && errno==EINTR);
                    if(ret<0){
                        perror("Errore di scrittura sul socket udp: ");
                        exit(1);
                    }

                    printf("\nInviata la parola %s al peer (server)\n...\aAttendiamo risposta...",tosend);
                    free(tosend);

                    //lanciamo la recvfrom che attende finche' non riceve risposta e la impostiamo affinche' riempia la struct mittenterispostaind e il size
                    //con i valori di chi ci ha inviato il messaggio(nb la famiglia della struct dobbiamo impostarla noi nel caso volessimo riutilizzare la struct indirizzo
                    //da cui abbiamo ricevuto messaggio)

                    mittenterispostaind.sin_family=AF_INET;

                    toreceive=calloc(300,sizeof(char));
                    do ret=recvfrom(udpsfd,toreceive,300,0,(struct sockaddr *)&mittenterispostaind,&sizemittenterisposta);
                        while(ret<0 && errno==EINTR);


                    //ora facciamo il controllo che il primo peer a cui abbiamo inviato, e quello che ci ha risposto, abbiano lo stesso ip (per maggiore precisione dovremmo eseguire
                    //il controllo sui domain name ottenuti dalla routine di risoluzione dns
                    if(sizeserver==sizemittenterisposta && memcmp(&servind,&mittenterispostaind,sizeserver)){ //in realta' sarebbe bastata la seconda parte del campo condizione
                        //tiriamo fuori il numero dal messaggio ricevuto e messo su toreceive
                        sscanf(toreceive,"%d",&risultato);
                        free(toreceive);
                        printf("\nRicevuta risposta dallo stesso peer a cui avevamo inviato la parola\nLa parola e' lunga %d char",risultato);
                    }

                    else printf("\nAbbiamo ricevuto una risposta da un peer che non e' lo stesso che avevamo contattato. Risposta scartata\n");

}


}
