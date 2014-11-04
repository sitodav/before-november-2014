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
#include <time.h>



int main() {
struct sockaddr_in mittind,nostrind;
int udpsfd,sizemitt,nostraporta,i,cont,ret;
char *toreceive,*tosend;
struct timeval tempo;
fd_set set1; FD_ZERO(&set1);

printf("\nInserire la porta sulla quale ricevere\n");
scanf("%d",&nostraporta);

//riempiamo la struct del nostro indirizzo
nostrind.sin_family=AF_INET;
nostrind.sin_port=htons(nostraporta);
nostrind.sin_addr.s_addr=htons(INADDR_ANY);

//creiamo il socket e lo bindiamo
if((udpsfd=socket(AF_INET,SOCK_DGRAM,0))<0){
    perror("\nImpossibile creare il socket udp di ricezione: ");
    exit(1);
}

if(bind(udpsfd,(struct sockaddr *)&nostrind,sizeof(nostrind))<0){
    perror("\nImpossibile bindare il socket udp di ricezione");
    exit(1);
}

//utilizziamo una select per stampare ogni 10 secondi un messaggio che ci dice se non sta succedendo niente

for(;;){
        tempo.tv_sec=5;
        tempo.tv_usec=0;
        FD_SET(udpsfd,&set1);
        ret=select(udpsfd+1,&set1,NULL,NULL,&tempo);
        if(ret==0){ //cioe' se la select ritorna 0,quindi perche' e' andata in timeout:
                printf("\nIn attesa di ricevere messaggi da client\n");
                continue;
        }
        else if(FD_ISSET(udpsfd,&set1)){ //in realta' e' solo 1 il socket passato alla select. Quindi potremmo omettere l'FD_ISSET(...) in quanto
                                                                //se la select scatta, ma non in timeout, e' perche' sicuramente e' diventato ready prima dei 5 secondi il socket udp in lettura (quindi abbiamo ricevuto messaggio)
                printf("\nRicevuto messaggio da un client\n");
                toreceive=calloc(300,sizeof(char));
                cont=0;
                do ret=recvfrom(udpsfd,toreceive,300,0,(struct sockaddr *)&mittind,&sizemitt);
                    while(ret<0 && errno==EINTR);
                if(ret<0){
                    perror("Errore in lettura dal socket con la recvfrom:");
                    exit(1);
                }
                printf("\nRicevuto messaggio da client con indirizzo ip %s",inet_ntoa(mittind.sin_addr));
                                                            //inet_ntoa prende in input una struct sin_addr con un indirizzo in network order e restituisce
                                                            //una stringa di char con l'indirizzo in presentation
                i=0;
                while(toreceive[i]!='\0'){
                cont++;
                i++;
                }
                printf("\nLa parola ricevuta dal client e' lunga %d char",cont);
                free(toreceive);
                sprintf(tosend,"%d",cont);
                do ret=sendto(udpsfd,tosend,300,0,(struct sockaddr *)&mittind,sizemitt);
                    while(ret<0 && errno==EINTR);
                if(ret<0){
                    perror("Errore in scrittura sul socket udp:");
                    exit(1);
                }
                free(toreceive);


        }




}



}
