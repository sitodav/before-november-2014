//ricreare l'applicazione client server per l'echo che reinvia il conteggio char, col server che continua ad essere concorrente, ma col client
//che gestisce i canali di I/O con multiplexing (select)
//LATO CLIENT:

/**RICAPITOLANDO:
1)fai attenzione che le funzioni delle stringhe, e di i/o in generale tipo la scanf, spesso lasciano caratteri nel buffer stdin:
    questo porta 2 problemi: il buffer risulta pronto anche quando non dovrebbe esserlo+vengono saltate le funzioni che richiedono di premere invio per confermare
                                                                                                                                                nel caso in cui tra i caratteri presenti in stdin rimasti ci sia \n
    Ed inoltre conviene eliminare i caratteri rimasti nel buffer, nel momento subito successivo alla funzione che ce li ha fatti rimanere (utilizzando i metodi scritti sotto)
2)Invece delle read() e write() usate qui, andrebbero usate full_read() e full_write(), soprattutto la full_read, in quanto ci permette di trattare con certezza il caso in cui sia ritornato 0
3)Vanno trattati sempre i casi degli errori distinguendoli dal caso dell'interrupt ogni volta che si usa una system call() che puo' essere interrotta da un signalò
4)Fai attenzione a gestire i cicli del client e del server in modo che siano coordinati. Ad esempio un client che cicla all'infinito utilizzando sempre la stessa connessione,
   da problemi se il server a cui si connette, dopo un tot di operazioni chiude la connessione riaprendone una nuova, in quanto il client fa affidamento solo su quella, ma non inserisce nel ciclo infinito
   anche nuove richieste di connessioni. Quindi rimane appeso!

**/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <string.h>
#include <errno.h>
#include <time.h>

void manda_strconselect(int connsfd);

int main(int argc,char *argv[]) {
if(argc!=2) {printf("\n <IP SERVER>"); exit(1);}
int connsfd,ret,srvport,nchar;
int fdready;
struct sockaddr_in servind;

//per semplicita' usiamo read() e write() invece delle full..., e usiamo direttamente
//le funzioni di stdio (fgets()) col select, anche se sarebbe meglio usare read su stdin inevce della fgets  con la select(vedi libro)
//e sempre per semplicita' non bindiamo il nostro socket
printf("\nIns porta server\n");
scanf("%d",&srvport); ///qui sta ilproblema -> con la scanf premiamo invio, nello stdin buffer rimane \n, ma viene salvato soltanto il numero, quindi quello \n finisce nella prima fgets
while(getchar()!='\n'); ///---> cosi' facendo risolviamo (MA XKE? NON SI RISOLVE SE LO METTIAMO INVECE NELLA FUNZIOEN ?)
/* metodo alternativo_
char buff[12];
fgets(buff,10,stdin); //invece di scanf: fgets + sscanf
sscanf(buff,"%d",&srvport); */

//il proiblema e' che quando si inserisce una stringa, e si preme \n, tutti questi caratteri, compreso l'ultima carattere di invio stringa (\n)
//vanno nel buffer stdin. da qui vengono presi dalla fgets e salvati nel buffer  stringa in cui vogliamo metterli. La fgets si prende anche il \n
//a patto che si rimanga nel max-1 dei caratter iche si e 'indicato di prendere al max.
//Ma il buffer stdin e' n buffer memorizzato. Cioe' i caratteri digitati, finiscono sempre TUTTI nello stdin, e alcune funzioni potrebbero non prenderseli tutti,
//e quindi quelli che rimangono vengono presi erroneamente nella prossima funzione che legge da stdin.
//Questo e' i lcaso di sopra con la scanf: si nserisce un numero e si preme invio: nello stdin ci finiscono il numero della porta, e \n, ma la scanf
//dallo stdin prende soltanto il numero, e l'invio rimane nello stdin (PERCHE' GLI STREAM DI I/O IN C SONO BUFFERIZZATI)
//che viene preso successivamente appena lanciamo la prima fgets della funzione esterna di echo,  e siccome \n fa ritornare la fgets, questo ci impedisce
//di avere il tempo di inserire una stringa nella prima fgets (che viene saltata)

//ATTENZIONE CHE NON HA SENSO FARE UNA SELECT SULLO STDIN, QUANDO NELLO STDIN VENGONO LASCIATI
//DEI CARATTERI BUFFERIZZATI CHE NON SONO STATI PRESI DALL'ULTIMA FUNZIONE DI STRINGHE, IN QUANTO
//LO STDIN IN QUEL CASO RISULTERA' SUBITO IMMEDIATAMENTE PRONTO (IN QUANTO AVRA' CARATTERI AL SUO INTERNO NEL MOMENTO
//IN CUI PARTE LA SELECT!


servind.sin_family=AF_INET;
servind.sin_port=htons(srvport);
inet_pton(AF_INET,argv[1],&servind.sin_addr);

if((connsfd=socket(AF_INET,SOCK_STREAM,0))<0)
    {printf("\nImpossibile creare il socket\n"); exit(1); }


do ret=connect(connsfd,(struct sockaddr *)&servind,sizeof(servind));
    while(ret<0 && errno==EINTR);
if(ret<0)   {printf("\nC'e' stato un errore diverso da un signal! Impossibile connettersi\n"); exit(1);}
printf("\nConnessione effettuata\n");


while(1) manda_strconselect(connsfd);
exit(0);
                }

void manda_strconselect(int connsfd) {
int nchar;
char tosend[200]="\0",toreceive[200]="\0";
fd_set set1;
struct timeval tempo;
tempo.tv_sec=15;
tempo.tv_usec=0;
int nfdready;
int w,r;
FD_ZERO(&set1);
FD_SET(fileno(stdin),&set1);
FD_SET(connsfd,&set1);
nfdready=select(connsfd+1,&set1,NULL,NULL,&tempo); //il processo si blocca qui
if(nfdready==0)
    {//allora e' andato in timeout la select, quindi la struct tempo conterra' il tempo azzerato, ma a noi non interessa perche' in caso
      //di timeout vogliamo che venga abortita la funzione di echo, stampando un messaggio, ma tanto la funzione ripartira' (poiche' nel main e' in un while(1))
      printf("\nTempo scaduto (15s)\n");
      return;

    }

if(FD_ISSET(fileno(stdin),&set1))   {
    printf("\nInserisci\n");

    fgets(tosend,200,stdin);
    do w=write(connsfd,tosend,200);
        while(w<0 && errno==EINTR);
    if(w<=0) {printf("\nImpossibile scrivere sul socket\n"); exit(1);} //nel caso di errori veri e propri, chiudiamo il programma, poiche' anche rilanciando la funzione non andrebbe (penso, boh roba di SO)

                                                        }

if(FD_ISSET(connsfd,&set1))         { //il socket in lettura diventa ready se: il server chiude la connessione (mentre non siamo bloccati in una funzione I/O tipo fgets)
                                                                                                                        //il server scrive sul socket
do r=read(connsfd,toreceive,200);
    while(r<0 && errno==EINTR);
if(r<0) {printf("\nImpossibile leggere dal socket\n"); exit(1);}
if(r==0)    {if(strcmp(tosend,"")==0) {printf("\nAbbiamo chiuso NOI la connessione\n"); exit(0);} //se non leggiamo niente dal server, ma perche' precedentemente gli avevamo mandato una stringa vuota (la strcmp non paragona ne' il terminatore stringa, ne' il \n, quindi se premiamo solo enter, come stringa abbiamo inserito "")
                                                                                                                                                                //(facendogli chiudere la connessione) vuol dire che
                                                                                                                                                                //la connessione e' stata chiusa per nostra decisione (stringa vuota)
                    else {printf("\nIl server ha chiuso la connessione\n"); exit(0);}} //altrimenti e' per decisione del server
                    //nb: se la conn e' chiusa dal server, dobbiamo chiudere proprio il programma, non possiamo uscire dalla funzione e farla rilanciare nel ciclo while
                    //poiche' non viene rifatta una nuova connessione in questa funzione (cosa che sarebbe necessaria in quanto il server ha buttato giu' la precedente)
                    //altrimenti il nostro client continua a girare a vuoto sul while(1) ma su una connessione che non esiste piu'!

sscanf(toreceive,"%d",&nchar); //il numero arriva come stringa dal socket, la leggiamo, e lo riconvertiamo in int
printf("\nIL SERVER DICE CHE LA PAROLA INSERITA E' LUNGA: %d\n",nchar);
                                                        }
}
