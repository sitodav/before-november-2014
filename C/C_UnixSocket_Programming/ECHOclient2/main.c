#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>

void funzecho(int sockt, FILE *io);
int main(int argc,char *argv[]) {
if(argc!=3)
    {printf("\n<IP nostro> <IP server>\n");
     exit(1);
    }
int ourport,srvport,connsfd;
struct sockaddr_in srvind,ourind;
printf("\nDa che porta vuoi uscire ?\n");
scanf("%d",&ourport);
printf("\nIns porta del server\n");
scanf("%d",&srvport);

srvind.sin_port=htons(srvport);
srvind.sin_family=AF_INET;
inet_pton(AF_INET,argv[1],&srvind.sin_addr);

ourind.sin_port=htons(ourport);
ourind.sin_family=AF_INET;
inet_pton(AF_INET,argv[2],&ourind.sin_addr);

if((connsfd=socket(AF_INET,SOCK_STREAM,0))<0)
    {
        printf("\Impossibile inizializzare il socket\n");
        exit(1); }

if(bind(connsfd,(struct sockaddr *)&ourind,sizeof(ourind))<0)
    {
        printf("\Impossibile assegnare al socket la porta/ip scelti\n");
        exit(1);
    }

if(connect(connsfd,(struct sockaddr *)&srvind,sizeof(srvind))<0)
    {
        printf("\nImpossibile stabilire la connessione\n");
        exit(1); }

printf("\nConnessione stabilita\n");

funzecho(connsfd,stdin); //scriviamo una funzione apposita per la gestione delle operazioni output/input sul socket per parlare col server
exit(0);




                }

void funzecho(int connsfd, FILE *io) {
char tosend[50]=" ",torcv[50]=" ";
int w,r;

while(getchar() != '\n');

while(fgets(tosend,50,io)!=NULL) //il blocco avviene sulla funz di input fgets (si sblocca o se vengono inseriti dati e viene premuto invio, oppure se non viene inserito niente e viene premuto invio
        {do {w=write(connsfd,tosend,50); }//ora dobbiamo far bloccare sulla funzioen di output sul socket (sblocca o se vengono correttamente scritti n caratteri (quindi la write >0) oppure se ritorna errore (e in quel caso bisogna controllare se e' un errore normale o un interrupt)
                while(w==-1 && errno==EINTR);
          do {r=read(connsfd,torcv,50);}
                while(r==-1 && errno==EINTR);
          if(r==0)
                {
                    printf("\nIl server ha terminato la connessione\n");
                    exit(0);
                }

          printf("\nMessaggio ricevuto dal server:\n%s",torcv);


        }

                                                   }
