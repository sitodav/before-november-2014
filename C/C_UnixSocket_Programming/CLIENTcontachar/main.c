//il client manda una parola al server, e questo rimanda indietro il numero di caratteri di cui e' composta la parola
//I/O Blocking di default, server concorrente

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <time.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

int main(int argc,char *argv[]) {
int connsfd,serverporta,ret;
struct sockaddr_in serverind;
char tosend[200];
char torece[200];
if(argc!=2)
    {printf("\n<IP SERVER> \n");
     exit(1);
    }
printf("\nIns porta del server\n");
scanf("%d",&serverporta);

serverind.sin_family=AF_INET;
serverind.sin_port=htons(serverporta);
inet_pton(AF_INET,argv[1],&serverind.sin_addr);

if((connsfd=socket(AF_INET,SOCK_STREAM,0))<0) //la socket non puo' essere interrotta da un sign, quindi se ritorna -1, e' sicuramente un vero errore, quindi si quitta
    {printf("\nImpossibile creare il socket\n");
     exit(1);
    }
do{ret=connect(connsfd,(struct sockaddr *)&serverind,sizeof(serverind));} //la connect puo' essere interrotta da un signal (ritornando -1) quindi deve continuare a lanciarla finche' ritorna <0 con errno=EINTR (che e' il codice di errore per i SIGNAL che interrompono la system call())
    while(ret<0 && errno==EINTR); //quindi smette di chiamarla o quando ritorna un valore di successo (conn stabilita) oppure quando ritorna un valore di errore (ma con errno!=EINTR quindi non e' stato per un SIGNAL che ha interrotto)
//quindi dobbiamo vedrere quale dei due casi e' stato, e nel secondo dobbiamo interrompere il processo client
if(ret<0) {printf("\nImpossibile completare \"3 way handshake\" col server\n");  exit(1);}
printf("\n(3 way handshake completato)-->CONNESSIONE STABILITA\n");

//per semplicita' non usiamo la full_write e la full_read, ma semplici write e read()

//per sicurezza usiamo l'algoritmo per pulire il buffer dal char di invio \n
while(getchar()!='\n'); //ora e' pulito
printf("\nInserisci la parola da inviare\n");

fgets(tosend,200,stdin);   //il processo si blocca qui, per leggere dall'input stdin

do {ret=write(connsfd,tosend,200); } //anche la write puo' essere interrotta da un signal, ed in quel caso ritorna <0 ed errno==EINTR e va riavviata
   while(ret<0 && errno==EINTR); //esce dal ciclo smettendo di richiamarla finche' ritorna un valore corretto oppure un errore ma con errno!=EINTR
    //quindi controlliamo in quale caso siamo
    if(errno==EPIPE) {printf("\nERRORE DI SIG PIPE: il server aveva chiuso la connessione mentre stavamo inserendo la parola\n"); exit(1);}
                                //all'uscita dal ciclo se errno==EPIPE vuol dire che c'e' stato un errore di sig pipe
    else if(ret<=0) {printf("\nImpossibile scrivere sul socket per altro errore(!)\n"); exit(1);} //se la write ritorna 0 o -1 con errno!=EINTR (lo sara' sicuramente qui essendo usciti dal ciclo) allora e' un errore vero
    //else facltv
do { ret=read(connsfd,torece,200);} //lo stesso per la read(), solo che la read puo' ritornare 0, in tal caso vuol dire che il server ha chiuso la connessione o il file e' terminato
        while(ret<0 && errno==EINTR);
if(ret==0)
    printf("\nFINE file /conn chiusa\n");
else if(ret<0) {printf("\nAltra tipologia d'errore nella lettura\n"); exit(1); }
printf("\n%s\n",torece);

 exit(0);


                }
