#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <sys/select.h>
#include <time.h>
#include <errno.h>

///non ho implementato in realta' un echo, ma il server si limita a leggere e stampare dai client connessi
///e non ho scritto un client apposito (tanto sarebbe elementare)
//il server invece di usare una fork, con un child per ogni client ricevuto, utilizza un I/O multiplexato, con una select che controlla
//l'unico socket di tipo listening, e tutti gli altri socket connessi con i client, utilizzando un array dove memorizza i socket attivati, aggiungendoli
//e levandoli a seconda di quello che succede col listening socket (sul quale avvengono le richieste per i nuovi client)
//e di quello che succede sui socket gia' connessi; (se scrivono, noi leggiamo con la funzione di echo perque' questo fa il server)
//se invece leggendo, leggiamo zero (in realta' dovremm ousare la full_read() ) vuol dire che il client ha mandato un FIN, quindi dobbiamo chiudere la connessione
//e levarlo dall'array dei fd dei socket attivi

//per semplicita' non usiamo la full_read() e la full write ma la read e write


int main() {
int listensfd,multiplexedfd[FD_SETSIZE],i,r,w,ret,port,iultimofd,maxfd;
fd_set set1;
char tosend[100]=" ",toreceive[100]=" ";
//l'array dei fd lo inizializziamo tutto a -1 e FD_SETSIZE e' il massimo numero di fd controllabili dalla select su questa macchina
struct sockaddr_in nostrind;
printf("\nInserire la porta sulla quale ricevere conn\n");
scanf("%d",&port); //eliminiamo il \n che rimane nel buffer perche' non preso dalla scanf
while(getchar()!='\n');

nostrind.sin_port=htons(port);
nostrind.sin_family=AF_INET;
nostrind.sin_addr.s_addr=htons(INADDR_ANY);

for(i=0;i<FD_SETSIZE;i++) multiplexedfd[i]=-1;

if((listensfd=socket(AF_INET,SOCK_STREAM,0))<0)
    {printf("\nImpossibile creare il socket per l'ascolto\n"); exit(1); }
if(bind(listensfd,(struct sockaddr *)&nostrind,sizeof(nostrind))<0)
    {printf("\nImpossibile bindare il socket sulla porta\n"); exit(1); }
if(listen(listensfd,1024)<0)
    {printf("\nImpossibile mettere in ascolto il socket\n"); exit(1); }
//all'interno di un ciclo infinito, il server svolgera' il proprio compito, ed in questo ciclo ci sara 'la select a controllare gli input.
//Ora il socket va messo in ascolto all'esterno di questo ciclo, quindi al di fuori del select, poiche' viene reso di tipo listening solo 1 volta,
//mentre gia' l'accept va messa nel select, poiche' se la mettessimo fuori non sarebbe multiplexata, ma il ciclo si bloccherebbe su di essa, dandole precedenza
//rispetto al select stesso con i vari socket ! (quindi il programma non fornirebbe ad ogni giro, servizio ai client connessi, fino a uqando non si libera dalla accept, cioe' fino a quando
//non riceve un ennesimo client ! ma cosi' non ha senso)

FD_ZERO(&set1);
multiplexedfd[0]=listensfd;
iultimofd=0;
maxfd=listensfd;

while(1){
for(i=0;i<=maxfd;i++)
            if(multiplexedfd[i]!=-1)
                    FD_SET(multiplexedfd[i],&set1);



if((ret=select(maxfd+1,&set1,NULL,NULL,NULL))<0 && errno==EINTR)
    continue;   //si lancia la select, e nel caso in cui sia interrotta da un signal, viene riavviato il ciclo con tutti gli ultimi fd dell'array portati nel set1

if(FD_ISSET(multiplexedfd[0],&set1)) //controlliamo se dopo la select, e' ready il socket di ascolto (sta sempre in indice 0 nel'array)
    {do ret=accept(listensfd,NULL,NULL);   //lancia l'accept nel caso in cui diventi pronto il socket di ascolto
            while(ret<0 && errno==EINTR); //ripetiamo la accept finche' e' interrotta da un signal
      if(ret<0)    { printf("\nErrore (non SIGNAL) nella ACCEPT()\n"); exit(1); } //se era un altro error a farla uscire dal while, quitta
      //altrimenti se la connessione col nuovo client e' valida, il suo fd sara' in ret, quindi incrementiamo l'indice dell'ultimo fd nell'array e gli assegniamo ret
      printf("\nUn nuovo client si e' connesso tramite il list socket ...\n");
      multiplexedfd[++iultimofd]=ret;
      if(multiplexedfd[iultimofd]>maxfd) maxfd=multiplexedfd[iultimofd]; //se il fd assegnato al nuovo client e' maggiore del precedente max, diventa lui il nuovo max
    }

i=1;
while(i<=iultimofd)  //e controlliamo se sono ready i socket di connessione con i client che stanno nell'array dall'indice 1 a iultimofd (compreso)
    {if(FD_ISSET(multiplexedfd[i],&set1)) //..allora l'iesimo connsfd e' pronto per noi IN LETTURA (usiamo solo set1 in lettura in questo server)
            {do r=read(multiplexedfd[i],toreceive,100); //per correttezza (vedi altri es...) andrebbe usata la full_read()
                while(r<0 && errno==EINTR); //continuiamo a lanciare la read() se viene interrotta da un signal
              if(r<0 /*errore non SIGNAL */) {printf("\nErrore in lettura\n"); exit(1);}
              if(r==0 /* questa condizione e' interpretabile solo con la full_read in realta' e non con la sola read() */)
                    {printf("\nIl client n %d ha chiuso la connessione (ricevuto FIN)\n...",i);
                      close(multiplexedfd[i]); //chiudiamo il socket del client, a livello del kernel del nostro server
                      //e nell'array, al posto del fd del client che si e' disconnesso, ci spostiamo l'ultimo arrivato, reinizializziamo l'ultimo, e decrementiamo l'indice dell'ultimo
                      multiplexedfd[i]=multiplexedfd[iultimofd];
                      multiplexedfd[iultimofd--]=-1;
                    }

              else
                    printf("\nIl client %d ci scrive: %s",i,toreceive);
            }
     i++;
    }

        }

}

