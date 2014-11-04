//il server gestisce il protocollo TCP utilizzando un listening socket tcp, e girando per ogni client la connessione
//su u nconn socket. Mentre utilizza per tutti i client un socket udp senza connessione.
//Tutti i socket sono multiplexati su un array di controllo, ed inoltre il server riceve messaggi da ogni client, e puo' inviarli a tutti i client
//il server pero' non puo' inviare udp, xke' abbiamo impostato i client che non ricevono
//NB: siccome questo e' solo un esercizio, ogni messaggio ricevuto da un client, non lo giriamo a tutti gli altri
//Ma implementiamo un'applicazione che puo' ricevere da ognuno e inviare a TUTTI
//(quindi non una chat)
//Il server inoltre indica da chi e' arrivato il messaggio, stampando il domain name ottenuto con una query al dns


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

int session_server(int udpport,int tcpport);

int main() {
int udpport,tcpport;
printf("\nInserisci la porta udp su cui ricevere\n");
scanf("%d",&udpport);
while(getchar()!='\n');
printf("\nInserisci la porta tcp su cui ricevere\n");
scanf("%d",&tcpport);
while(getchar()!='\n');
session_server(udpport,tcpport);
}


int session_server(int udpport,int tcpport) {
int multiplexedfd[FD_SETSIZE],i,maxfd,ret,nfdready,sizemittente,sizerichiedente;
struct sockaddr_in nostrindudp,nostrindtcp,*mittente,*destinatario,*richiedenteconnessionetcp;
struct hostent *query;
char *tosend,*toread,*domain_name;
fd_set set1;


//inizializziamo l'array dei socket e il set
for(i=0;i<FD_SETSIZE;i++)
multiplexedfd[i]=-1;
FD_ZERO(&set1);

nostrindudp.sin_family=AF_INET; //struct indirizzo per il socket udp di ricezione
nostrindudp.sin_port=htons(udpport);
nostrindudp.sin_addr.s_addr=htons(INADDR_ANY);

nostrindtcp.sin_family=AF_INET;
nostrindtcp.sin_port=htons(tcpport);
nostrindtcp.sin_addr.s_addr=htons(INADDR_ANY);

//creiamo il socket udp e quello di listen tcp
if((multiplexedfd[0]=socket(AF_INET,SOCK_DGRAM,0))<0){ //in pos 0 c'e' il socket udp
        perror("\n");
        exit(1);
}

if((multiplexedfd[2]=socket(AF_INET,SOCK_STREAM,0))<0){ //in 2 quello tcp di ascolto
        perror("\n");
        exit(1);
}

//quello udp dobbiamo solo bindarlo
if(bind(multiplexedfd[0],(struct sockaddr *)&nostrindudp,sizeof(nostrindudp))<0){
    perror("\n");
    exit(1);
}

//quello tcp dobbiamo bindarlo e metterlo in ascolto
if(bind(multiplexedfd[2],(struct sockaddr *)&nostrindtcp,sizeof(nostrindtcp))<0){
    perror("\nImpossibile bindare il socket tcp ");
    exit(1);
}

if(listen(multiplexedfd[2],1024)<0){
    perror("\nImpossibile mettere in ascolto li socket tcp");
    exit(1);
}

//in 1 mettiamo lo stdin
multiplexedfd[1]=fileno(stdin);

//cosi' l'array contiene per ora tutti i fd da multiplexare, ora prepariamo il set



if(multiplexedfd[0]>multiplexedfd[2]) maxfd=multiplexedfd[0];
else maxfd=multiplexedfd[2];

for(;;){

    for(i=0;i<FD_SETSIZE;i++)
        if(multiplexedfd[i]!=-1)
                FD_SET(multiplexedfd[i],&set1);

    nfdready=select(maxfd+1,&set1,NULL,NULL,NULL);
    if(nfdready==-1){
            perror("\n");
            exit(1);
    }
    //la select non va in timeout, quindi se non da errore ritorna almeno 1
    if(FD_ISSET(multiplexedfd[0],&set1)){ //allora e' pronto in lettura il socket udp, quindi abbiamo ricevuto qualcosa tramite protocollo UDP
            toread=calloc(300,sizeof(char));
            mittente=calloc(1,sizeof(struct sockaddr_in));
            do ret=recvfrom(multiplexedfd[0],toread,300,0,(struct sockaddr *)mittente,&sizemittente); //usiamo la recvfrom xke' il socket udp viene lasciato di default cioe' connectionless
                    while(ret<0 && errno==EINTR);
            if(ret<0){
                    perror("\n");
                    exit(1);
            }
            //eseguiamo la query per il domain name passandogli il valore col quale e' stato riempita la struct mittente dalla recvfrom
            // e il risultato della query verra' dato riempiendo una struct di tipo hostent. Da li' lo metteremo nella stringa domain_name e lo stampiamo
            domain_name=calloc(100,sizeof(char));
            query=calloc(1,sizeof(struct hostent));
            query=gethostbyaddr(&(mittente->sin_addr),sizeof(mittente->sin_addr),AF_INET);
            strcpy(domain_name,query->h_name);
            printf("\nRicevuto messaggio UDP da (DOMAIN NAME) %s\n%s\n",domain_name,toread);
            fflush(stdout);



    }

    if(FD_ISSET(multiplexedfd[1],&set1)){ //allora e' pronto lo stdin (xke' abbiamo digitato caratteri e abbiamo premuto enter
            //il server scrive a tutti i client, solo attraverso il protocollo TCP, quindi scorriamo sull'array per i tcp conn sock e inviamo a quelli
            tosend=calloc(300,sizeof(char));
            fgets(tosend,300,stdin);
            for(i=3;i<FD_SETSIZE;i++)
                if(multiplexedfd[i]!=-1){
                    do ret=write(multiplexedfd[i],tosend,300);
                        while(ret<0 && errno==EINTR );
                    if(ret<=0){
                        perror("\n");
                        exit(1);
                    }
                }
             printf("\nMessaggio inviato a tutti i client(su prot TCP)!\n");
             free(tosend);

    }

    if(FD_ISSET(multiplexedfd[2],&set1)){ //allora e' pronto in lettura il listening socket, quindi qualcuno si vuole connettere
            printf("\nRicevuta richiesta di connessione da un client\n");
            i=3;
            while(i<FD_SETSIZE){
                    if(multiplexedfd[i]==-1) break;
                    i++;
            }
            if(i==FD_SETSIZE){
                    printf("\nTroppi Client connessi! Richiesta rifiutata");
                    continue;
            }
            richiedenteconnessionetcp=calloc(1,sizeof(struct sockaddr_in));
            do multiplexedfd[i]=accept(multiplexedfd[2],(struct sockaddr *)richiedenteconnessionetcp,&sizerichiedente);
                while(multiplexedfd[i]<0 && errno==EINTR);

            if(multiplexedfd[i]<0){
                    perror("\n");
                    exit(1);
            }

            if(multiplexedfd[i]>maxfd) maxfd=multiplexedfd[i];

            //con l'indirizzo ottenuto dalla accept() del client connesso, facciamo una query al dns e stampiamo il domain name del client
            query=calloc(1,sizeof(struct hostent));
            query=gethostbyaddr(&(richiedenteconnessionetcp->sin_addr),sizeof(richiedenteconnessionetcp->sin_addr),AF_INET);
            printf("\nIl client %s si e' connesso\n",query->h_name);



    }

    //ora controlla in letura tutti i connsock tcp dei vari client per vedere se ci dicono qualcosa
    for(i=3;i<FD_SETSIZE;i++)
            if(multiplexedfd[i]!=-1 && FD_ISSET(multiplexedfd[i],&set1)){ //allora il client iesimo ci ha inviato qualcosa via tcp
                        //quindi lo stampiamo (non lo rigiriamo a tutti gli altri client visto che non ci interessa per l'esercizio)
                        printf("\nUn client ci ha inviato qualcosa\n");
                        fflush(stdout);
                        toread=calloc(300,sizeof(char));
                        do ret=read(multiplexedfd[i],toread,300);
                            while(ret<0 && errno==EINTR);
                        if(ret<0){
                            perror("\n");
                            exit(1);
                        }



                       else  if(ret==0){
                           query=calloc(1,sizeof(struct hostent));
                        mittente=calloc(1,sizeof(struct sockaddr_in)); //la getpeername riempie una struct sockaddr_in con l'ip del client su quel socket
                        domain_name=calloc(100,sizeof(char));

                        getpeername(multiplexedfd[i],(struct sockaddr_in *)mittente,sizeof(mittente));
                        query=gethostbyaddr(&(mittente->sin_addr),sizeof(mittente->sin_addr),AF_INET);
                        strcpy(domain_name,query->h_name);
                            //questo controllo e' corretto in realta' solo per la full_read
                            //il client allora si vuole disconnettere
                            printf("\nIl client (DOMAIN NAME) %s si e' disconnesso\n",domain_name);
                            fflush(stdout);

                            //eliminiamo il client dalle strutture dati aggiornando il max fd
                            if(multiplexedfd[i]==maxfd){
                                maxfd=multiplexedfd[0];
                                for(i=1;i<FD_SETSIZE;i++)
                                    if(multiplexedfd[i]>maxfd)
                                            maxfd=multiplexedfd[i];
                            }
                            close(multiplexedfd[i]);
                            multiplexedfd[i]=-1;

                        }

                        else {//allora abbiamo letto >0 char, quindi abbiam oricevuto un messaggio

                                query=calloc(1,sizeof(struct hostent));
                        mittente=calloc(1,sizeof(struct sockaddr_in)); //la getpeername riempie una struct sockaddr_in con l'ip del client su quel socket
                        domain_name=calloc(100,sizeof(char));

                        getpeername(multiplexedfd[i],(struct sockaddr_in *)mittente,sizeof(mittente));
                        query=gethostbyaddr(&(mittente->sin_addr),sizeof(mittente->sin_addr),AF_INET);
                        strcpy(domain_name,query->h_name);

                        printf("\nIl client (DOMAIN NAME) %s ci scrive:%s\n",domain_name,toread);
                        fflush(stdout);

                        }




                        if(nfdready--<=0) break;


            }




}
}

