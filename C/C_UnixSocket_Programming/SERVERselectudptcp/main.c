//il server tiene multiplexati un array di fd a socket tcp di connessione, uno tcp di listening, e un udp socket in modo tale da
//poter rispondere sia a client che mandano stringhe attraverso il protocollo tcp, sia attraverso quello udp

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <time.h>
#include <errno.h>

void sessione_server(int portatcp,int portaudp);
int main() {
int portatcp,portaudp;
printf("\nInserire porta TCP su cui ricevere\n");
scanf("%d",&portatcp);
while(getchar()!='\n');
printf("\nInserire porta UDP sucui ricevere\n");
scanf("%d",&portaudp);
while(getchar()!='\n');

sessione_server(portatcp,portaudp);
return 0;
}

void sessione_server(int portatcp, int portaudp) {
struct sockaddr_in nostrind,nostrind2;
struct sockaddr_in *indclient_suudp;
fd_set set1;
FD_ZERO(&set1);
//metteremo il listening socket in indice 0, l'udp in 1 e ogni socket di connessione aperta con client sui restanti
//come si nota, l'udp e' uno per qualunque socket, come il listening socket ,solo che il listening socket e' uno ma poi passa ad un connsock che e' uno
//per ogni client
int multiplexed_fd[FD_SETSIZE],i,ret,nfdready,maxfd,size_indclientudp,j;
for(i=0;i<FD_SETSIZE;i++)
    multiplexed_fd[i]=-1;
char *tosend,*toreceive;
struct timeval tempo1;

//riempiamo la struct con l'indirizzo di ricezione del server
nostrind.sin_family=AF_INET;
nostrind.sin_port=htons(portatcp);
nostrind.sin_addr.s_addr=htons(INADDR_ANY);

//apriamo sull'array in posizione 0 il socket di ascolto (tcp) bindandolo al nostrind e mettendolo poi in ascolto
if((multiplexed_fd[0]=socket(AF_INET,SOCK_STREAM,0))<0){
            perror("\nErrore nell'apertura del socket di ascolto (tcp)\n");
            exit(1);
}

if(bind(multiplexed_fd[0],(struct sockaddr *)&nostrind,sizeof(nostrind))<0){
         perror("\nImpossibile bindare il socket di ascolto sull'ip/porta assegnati\n");
         exit(1);
}

if(listen(multiplexed_fd[0],1024)<0) {
         perror("\nImpossibile mettere in ascolto il socket tcp");
         exit(1);
}

//ora creiamo il socket udp e lo mettiamo in posizione 1, e lo bindiamo su una porta differente
if((multiplexed_fd[1]=socket(AF_INET,SOCK_DGRAM,0))<0){
            perror("\nErrore nell'apertura del socket udp\n");
            exit(1);
}

//proviamo a modificare la struct usata per il tcp, cambiando la porta ,e bindando l'udp su quello
/// /////
nostrind.sin_port=htons(portaudp);
if(bind(multiplexed_fd[1],(struct sockaddr *)&nostrind,sizeof(nostrind))<0){
    perror("\nImpossibile bindare il socket udp\n");
    exit(1);
}

if(multiplexed_fd[0]<multiplexed_fd[1]) maxfd=multiplexed_fd[1];
    else maxfd=multiplexed_fd[0];

for(;;){
    tempo1.tv_sec=5;
    for(i=0;i<FD_SETSIZE;i++)
        if(multiplexed_fd[i]!=-1)
                FD_SET(multiplexed_fd[i],&set1);

    nfdready=select(maxfd+1,&set1,NULL,NULL,&tempo1);
    //SI BLOCCA SULLA SELECT...

    //...RIPRENDE
    if(nfdready==0){
            printf("\nIn attesa...\n");
            continue;
    }

    if(FD_ISSET(multiplexed_fd[0],&set1)) { //allora e' ready il listening socket, quindi siamo pronti per completare una richiesta di connessione
            i=2; //parte dal 2, tanto sull'1 e sullo ' ci sono udp socket e tcp socket che non vengono mai eliminati
            while(i<FD_SETSIZE) {
                if(multiplexed_fd[i]==-1) break;
                i++;
            }
            //all'uscita del while:
            if(i==FD_SETSIZE){
                printf("\nTroppi client gia' connessi con protocollo TCP\n");
                continue;
            }

            if((multiplexed_fd[i]=accept(multiplexed_fd[0],NULL,NULL))<0){
                perror("errore nella ricezione dell'ACK dal client (passo 3 del 3wh, errore sull'accept())\n");
                exit(1);
            }
            //arrivati qui, avremo in multiplexed_fd[i] il fd del socket di connessione aperto col nuovo client
            printf("\nUn nuovo client si e' connesso (protocollo TCP)\n");
            if(nfdready--<=0) continue;
    }

    if(FD_ISSET(multiplexed_fd[1],&set1)){ //allora e' pronto in lettura il socket udp
            toreceive=calloc(300,sizeof(char));
            indclient_suudp=calloc(1,sizeof(struct sockaddr_in));
            do ret=recvfrom(multiplexed_fd[1],toreceive,300,0,(struct sockaddr *)indclient_suudp,&size_indclientudp);
                while(ret<0 && errno==EINTR);
            if(ret<0){
                perror("errore sulla lettura di dati dal socket udp:");
                exit(1);
            }
            fprintf(stdout,"Il client ci ha inviato (attraverso prot UDP) la parola %s\nGliela rinviamo in echo\n",toreceive);
            do ret=sendto(multiplexed_fd[1],toreceive,300,0,(struct sockaddr *)indclient_suudp,size_indclientudp);
                while(ret<0 && errno==EINTR);
            if(ret<0){ //NB che sia la sendto() che la recvfrom possono tranquillamente scrivere/ricevere 0, senza che questo sia un errore (come nel caso della write) o che indichi un FIN (come nel caso della read() anche xke' il FIN non esiste su udp, neanche quando si connette il socke tudp)
                perror("\nErrore di scrittura sul socket udp");
                exit(1);
            }
            free(toreceive);
            free(indclient_suudp);
            if(nfdready--<=0) continue;
    }

    for(i=2;i<FD_SETSIZE;i++)
        if(multiplexed_fd[i]!=-1 && FD_ISSET(multiplexed_fd[i],&set1)){ //allora e' pront in lettura uno dei socket di connessione aperto per ognuno dei client connessi
                toreceive=calloc(300,sizeof(char));
                do ret=read(multiplexed_fd[i],toreceive,300);
                    while(ret<0 && errno==EINTR);
                if(ret<0){
                    perror("\nErrore nella lettura da socket TCP connesso\n");
                    exit(1);
                }

                else if(ret==0){ // in realta' questo caso, scritto cosi', e' valido solo per la full_read etc
                            printf("Il client si e' disconnesso\n");
                            if(multiplexed_fd[i]==maxfd){
                                maxfd=multiplexed_fd[0];
                                for(j=1;j<FD_SETSIZE;j++)
                                    if(multiplexed_fd[j]>maxfd)
                                            maxfd=multiplexed_fd[j];
                            }
                            close(multiplexed_fd[i]);
                            multiplexed_fd[i]=-1;
                }

                else{
                            printf("\nIl client (connesso attraverso protocollo TCP) ci ha inviato la parola %s\nGliela rimandiamo\n",toreceive);
                            do ret=write(multiplexed_fd[i],toreceive,300);
                                while(ret<0 && errno==EINTR);
                            if(ret<=0){
                                perror("\nErrore scrittura sul socket UDP");
                                exit(1);
                            }
                }

                free(toreceive);
                if(nfdready--<=0) break;

        }



}



}
