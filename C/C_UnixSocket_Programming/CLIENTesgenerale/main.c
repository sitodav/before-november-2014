//IIl client puo' decidere di mandare messaggi attraverso protocollo udp, sia di stabilire una connessione vera e propria con socket TCP
//e inviare messaggi su quella connessione, finche' non viene chiusa da noi o dal server
//(nb rendiamo con connessione anche l'udp socket)
//La funzione che invia messaggi udp, crea il socket, lo connette (connessione udp, non 3wh del tcp)
//invia il messaggio, e disconnette UDP

//Implementiamo in modo tale che per ogni messaggio udp che si manda, (udp connesso) si lancia ogni volta
//una sessione di invio, e si ritorna al menu.
//Mentre se invece si utilizza il protocollo tcp, si inizia una sessione che rimane su finche' non decidiamo di inserire
//una stringa riservata che rappresenta un FIN, e in quel caso viene chiusa la sessione e si ritorna al menu
//per esercizio, l'udp lo usiamo solo per inviare messaggi, mai per ricevere (quindi il server lo utilizzera' solo per ricevere)


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <netdb.h>

int sessione_tcp(int tcpport_srv);
int sessione_udp(int udpport_srv);

int main() {
int udpport_srv,tcpport_srv,ris,ret;
//decidiamo la porta del server al quale inviare (con tcp o udp) ma non decidiamo con quale ip/porta uscire
while(1){
    printf("\n//\n[1]Stabilisci connessione TCP e invia messaggi al server attraverso TCP\n[2]Invia messaggio attraverso protocollo UDP al server\n");
    scanf("%d",&ris);
    while(getchar()!='\n');
    if(ris==1){
            printf("\nInserisci porta TCP del server\n");
            scanf("%d",&tcpport_srv);
            while(getchar()!='\n');
            ret=sessione_tcp(tcpport_srv);
            if(ret==0) printf("\nConnessione Terminata - Il server ha chiuso la connessione\n");
            else if(ret>0 )printf("\nConnessione Terminata - Abbiamo chiuso la connessione\n");

    }

    else if(ris==2){
            printf("\nInserisci porta UDP del server\n");
            scanf("%d",&udpport_srv);
            while(getchar()!='\n');
            ret=sessione_udp(udpport_srv);

    }

    if(ret<0) printf("\nConnessione Terminata - Avvenuto errore!\n");




}

}



int sessione_udp(int udpport_srv) {
struct sockaddr_in servind;
int udpsfd,ret;
char serverip[INET_ADDRSTRLEN],*tosend;

printf("\nInserisci ip del server\n");
fgets(serverip,INET_ADDRSTRLEN,stdin);
serverip[strlen(serverip)-1]='\0';

//riempiamo la struct indirizzo per il server
servind.sin_family=AF_INET;
servind.sin_port=htons(udpport_srv);
inet_pton(AF_INET,serverip,&servind.sin_addr);

//creiamo il socket udp (non lo bindiamo perche'non ci interessa scegliere una porta/ip di uscita)
if((udpsfd=socket(AF_INET,SOCK_DGRAM,0))<0){
            perror("\n");
            return -1;
}

//connettiamo il socket UDP, in modo tale che se il peer dall'altro lato non esiste ,possiamo ricevere l'ICMP (se il sistema operativo lo permette)
do ret=connect(udpsfd,(struct sockaddr *)&servind,sizeof(servind)); ///PERCHE' LA CONNECT() NON RITORNA ICMP NEL CASO DI SERVER INESISTENTE?
    while(ret<0 && errno==EINTR);
if(ret<0){
        perror("\n");
        return -1;
}

printf("\nDIGITA MESSAGGIO da inviare a:\nIP:%s    porta:%d\n",serverip,udpport_srv);
tosend=calloc(300,sizeof(char));
fgets(tosend,300,stdin); //lasciamo la \n come ultimo char da inviare
do ret=write(udpsfd,tosend,sizeof(tosend)+1); //usiamo la write (sarebbe ancora meglio la full_write) visto che il socket udp e' connesso
    while(ret<0 && errno==EINTR); //anche se il protocollo UDP puo' essere connesso, non prevede FIN, ACK ETC, quindi non effettuiamo ad es controlli su un possibile sigpipe generato da una write eseguita 2 volte su un socket chiuso
if(ret<0){
    perror("\n");
    return -1;
}
printf("\nMessaggio inviato correttamente- SCRITTI %d CARATTERI\n",ret);
printf("\n%s",tosend);

//ora possiamo ciudere la connessione per il socket udp, chiudiamo il socket e deallochiamo il buffer del messaggio da inviare

//per disconnettere un socket udp ci sono 2 modi: rilanciare un connect su quel socket udp, passando NULL come struct di indirizzo e come size
//                                                                               oppure rilanciare una connect, ma passare come struct indirizzi una struct azzerata con sin_family su AF_UNSPEC
//il secondo metodo e' migliore perche' piu' portabile
//USIAMO IL SECONDO METODO:
bzero(&servind,sizeof(servind)); //la bzero riempie l'argomento passato di tutti zero
servind.sin_family=AF_UNSPEC;
connect(udpsfd,(struct sockaddr *)&servind,sizeof(servind)); //non facciamo un controllo sul valore ritornato dalla connect, perche' potrebbe
//ritornare errore anche nel caso in cui la disconnessione sia andata a buon fine ((?? lo dice il manuale))

close(udpsfd); //il close in generale provoca la chiusura del file descriptor, e se questo ha indice <=1, viene chiuso definitivamente il file/buffer=socket del kernel.
                        //nel caso del tcp, quando viene chiuso il socket, viene generato un FIN, pero' nel caso dell'udp (che in ogni caso in questo punto e' disconnesso)
                        //chiude semplicemente il socket udp
free(tosend);
}


int sessione_tcp(int tcpport_srv) {
//il processo del client, per il tcp, siccome mentre si inserisce un messaggio (e si possono inserire piu' messaggi finche' non decidiamo di chiudere la connessione)
//possono arrivare pacchetti tipo FIN o RST sul socket connesso, deve multiplexare con una select()
struct sockaddr_in servind;
int ret,tcpconnsfd, quit4us,quit4srv,nfdready;
fd_set set1;
char serverip[INET_ADDRSTRLEN],*tosend,*toreceive;

printf("\nInserire ip del server\n");
fgets(serverip,INET_ADDRSTRLEN,stdin);
serverip[strlen(serverip)-1]='\0';

//riempiamo la struct indirizzo del server
servind.sin_port=htons(tcpport_srv);
servind.sin_family=AF_INET;
inet_pton(AF_INET,serverip,&servind.sin_addr);

//creiamo il socket e lo connettiamo
if((tcpconnsfd=socket(AF_INET,SOCK_STREAM,0))<0){
        perror("\n");
        return -1;
}

do ret=connect(tcpconnsfd,(struct sockaddr *)&servind,sizeof(servind));
    while(ret<0 && errno==EINTR);
if(ret<0){
    perror("\n");
    return -1;
}

printf("\nConnessione al server completata.\n");

FD_ZERO(&set1);
quit4us=0;
quit4srv=0;
//viene  ciclato finche' quit4us e quit4srv rimangono su 0, altrimenti esce dicendo al main chi ha interrotto la connessione

do {
        //creiamo il set di controllo per il client (la select dovra' controllare lo stdin e il socket in ricezione)

        FD_SET(fileno(stdin),&set1);
        FD_SET(tcpconnsfd,&set1);

        printf("\nSCRIVI MESSAGGIO DA INVIARE-> ");
        fflush(stdout);
        nfdready=select(tcpconnsfd+1,&set1,NULL,NULL,NULL);
        //ret non puo' essere 0, in quanto la select, non avendo un timeout dato da una struct passatagli, ritorna solo quando almeno un fd e 'ready

        if(FD_ISSET(fileno(stdin),&set1)){ //allora abbiamo inserito caratteri da tastiera, e dal buffer di questa li abbiamo inviati (premendo invio=\n)
                                                                //al buffer stdin(lo stdin e' pronto in lettura)
                tosend=calloc(300,sizeof(char));
                fgets(tosend,300,stdin);
                if(strncmp(tosend,"exit",strlen("exit"))==0) //allora abbiamo scritto exit, quindi vogliamo uscire
                                quit4us=1;
                else{
                                do ret=write(tcpconnsfd,tosend,sizeof(tosend)+1);
                                    while(ret<0 && errno==EINTR);
                                if(ret<=0){
                                    perror("\n");
                                    return -1;
                                }
                                else{
                                    printf("\nInviati %d char\n",ret);
                                }
                }

                if(nfdready--<=0 && !quit4us)
                                continue;
        }

        if(FD_ISSET(tcpconnsfd,&set1)) { //allora e' pronto in lettura il socket di connessione al srv= ci sono dati da leggere sul socket, quindi il server ci ha inviato qualcosa
                    toreceive=calloc(300,sizeof(char));
                    do ret=read(tcpconnsfd,toreceive,300);
                        while(ret<0 && errno==EINTR);
                    if(ret<0){
                            perror("\n");
                            return -1;
                    }

                    if(ret==0) //allora abbiamo ricevuto un FIN o un RST //in realta' questo controllo e' corretto nel caso della full_read() etc
                        quit4srv=1;

                   else {
                        printf("\nMessaggio ricevuto dal server:\n");
                        fprintf(stdout,"%s",toreceive);

                   }




        }

}
    while(quit4us==0 && quit4srv==0);



if(quit4us) return 1;
if(quit4srv) return 0;



}
