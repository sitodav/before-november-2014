/**SERVER CHAT
UN SERVER CHAT PUO':
--ACCETTARE LE CONNESSIONI E GIRARE I MESSAGGI DI OGNI CLIENT, A TUTTI GLI  NELLA STESSA STANZA SIMULANDO UNA STANZA DI CHAT
--SI SALVA PER SE' I CHAR DEL NICK DI OGNI CLIENT CHE SI CONNETTE, E TIENE UNA LISTA DEI CONNESSI ATTUALMENTE, E QUELLI DISCONNESSI
  LI SALVA IN UN FILE DI LOG
  USA I NICK DEGLI UTENTI ANCHE PER AVVISARE TUTTI GLI ALTRI DI UNA NUOVA CONNESSIONE/DISCONNESSIONE
-NB: AD OGNI UTENTE VENGONO GIRATI SOLO I MESSAGGI DEGLI ALTRI UTENTI CONNESSI ALLA STESSA CHAT, CIOE' PER I QUALI E' ==L'INT CORRISPONDENTE ALLA STANZA
(CIOE' IL 15ESIMO CHAR DEI MESSAGGI CHE OGNI CLIENT INVIA)
**/

  ///Il server manda al client 2 tipi di messaggi. Un messaggio normale, che contiene nei primi 14 char il nick del client del quale stiamo girando il messaggio, il 15esimo e' la stanza
///e i rimanenti sonoil messaggio girato di quel client, e un messaggio di tipo SERVER, ovvero una comunicazione di servizio del server
///che non contiene che possono essere scelti come nickname(combinazione riservata), nello spazio riservato al nick, e nello spazio del messaggio contiene una stringa che deve essere stampata in quanto tale dal client
///Ogni volta che un client riceve quindi un messaggio, controlla se e' di tipo CLIENT o SERVER in base a quello che c'e' nei primi 14 char!


///per il logfile: ogni volta che un utente si connette, si scrive su un file nella cartella dell'exe:
    ///-il fd del socket al quale si e' connesso, la data di connessione, l'ip e la porta del client (che otteniamo non con gli ultimi 2 parametri dell'accept(), bensi' con la
    ///getpeername() ed il nickname (quindi la funzione del log viene lanciata solo nel momento successivo a quello in cui il client scrive il primo messaggio,
    ///poiche' e' in quel momento che sappiamo qual e' il suo nick.
    ///E lo stesso viene fatto quando un client si disconnette (in quel caso pero' non viene preso il suo ip di nuovo, in quanto si presuppone che si fosse gia' connesso, quindi il suo ip fosse gia' nel log per quel fd e quel nick)


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

int clientdisconnesso_salvalog(int connsfd,char nickname[]);
int clientconnesso_getinfo_salvalog(int connsfd,char nickname[]);
void toall_mexSRV_nuovoutente(int connsfd,char nickname[],int iultimofd,int multiplexedfds[],char stanze[],char stanzamitt);
void toall_mexSRV_uscitautente(int connsfd,char nickname[],int iultimofd,int multiplexedfds[],char stanze[],char stanzamitt);
ssize_t full_read(int fd,void *vptr,size_t n);

int main() {
int listensfd,connsfd,ret,r,w,porta,nfdready,i,iultimofd;
char *nickclients[FD_SETSIZE]; //il nick di ogni client, viene messo in un array a parte di stringhe, con indice i uguale a quello che occupa il fd del connsfd di quel client,
                                                    //nb che il nome del client e' conoscibile solo nel momento in cui inizia a parlare in chat, in quanto il nick e' presente nei primi 14 caratteri del messaggio
                                                    //ogni stringa e' allocata dinamicamnete quando occorre
                                                    //Il server tiene il nick del client oltre che per il log file, anche per avvisare tutti gli altri client della connessione di un nuovo client
                                                    //che avviene sul listening socket, ma il messaggio della connessione del client lo inviamo quando luiscrive qualcosa per la prima volta
                                                    //in quanto in quel momento possiamo sapere il suo nick, e per avvisare tutti i client di quando si disconnette
char stanzefds[FD_SETSIZE]; //e un array di char, sui quale (per indice i corrispondente all'indice i di nickclients e multiplexedfds) verra' allocato e assegnato un char
                                        //corrispondente al client con quel fds e quel nick (indice i equivalente per tutti e 3 gli array)
fd_set set1,rset;
int maxfd,j,k;
struct timeval tempo1;
char toreceive[300]="",tosend[300]="";
struct sockaddr_in nostrind;
int multiplexedfds[FD_SETSIZE]; //il server puo' gestire un numero limitato di client, che e' il max numero di fd che possono entrare in un set
printf("\n\t\t\aSERVER APPLICATION PER CHAT\n\t\t\t\t\t Sito Davide\n");
printf("\nSu quale porta vogliamo ricevere ?");
scanf("%d",&porta);
//puliamo stdin dall'ultimo \n inserito con la scanf
while(getchar()!='\n');
if((listensfd=socket(AF_INET,SOCK_STREAM,0))<0)
    {printf("\nImpossibile creare il socket di ascolto\n");
        exit(1); //usciamo direttamente dal programma visto che non c'e' un menu al quale ritornare e stiamo gia' nel main
    }
nostrind.sin_family=AF_INET;
nostrind.sin_port=htons(porta);
nostrind.sin_addr.s_addr=htons(INADDR_ANY);

if(bind(listensfd,(struct sockaddr *)&nostrind,sizeof(nostrind))<0)
    {printf("\nImpossibile bindare il socket di ascolto\n");
    exit(1);}

if(listen(listensfd,1024)<0)
    {printf("\nImpossibile mettere in ascolto il socket\n");
     exit(1);}

//quando arriva una nuova connessione, il listening socket diventa ready, quindi si mette il listening socket nella select
//e qunado diventa ready si lancia l'accept. In questo modo si evita di lasciare il programma bloccato sulla accept ad aspettare
//che arrivi una richiesta sul listening socket

for(i=0;i<FD_SETSIZE;i++) multiplexedfds[i]=-1;
maxfd=listensfd;
multiplexedfds[0]=listensfd;
FD_ZERO(&set1);
FD_ZERO(&rset);
FD_SET(listensfd,&set1);
iultimofd=0;


for(;;)
{

    if(iultimofd==0)    printf("\n----NESSUN CLIENT CONNESSO. IN ATTESA DI CLIENTS...\n");
    rset=set1; //dobbiamo lavorare con un set diverso da quello sul quale aggiungiamo di volta in volta i client che in questo giro si connettono, altrimenti settandoli dopo la select, risultano attivati, ma solo perche' li abbiamo aggiunti, non perche' erano diventati ready sotto la select
    nfdready=select(maxfd+1,&rset,NULL,NULL,NULL); //non controlliamo che la select venga interrotta da un signal

    if(FD_ISSET(listensfd,&rset)) { //IL LISTENING SOCKET ALLORA E' RISULTATO PRONTO

    do connsfd=accept(listensfd,NULL,NULL);
        while(connsfd<0 && errno==EINTR);
    if(connsfd<0) {perror("\nCONNECT():"); continue;}
    printf("\n****UN NUOVO CLIENT SI E' CONNESSO ALLA CHAT !"); ///tuttavia la conn del nuovo client, verra' aggiunta nel log file solo dopo che avra' scritto almeno il primo messaggio (cosi' potremo anche il nick in quel momento e potremo salvare anche quello nel log file)
    fflush(stdout);

    for(i=0;i<FD_SETSIZE;i++) //scorre sull'array, e salva il nuovo connsfd nel primo elemento ancora inizializzato al valore iniziale
            if(multiplexedfds[i]<0) {
                multiplexedfds[i]=connsfd; //salva il fd per quel client sull'array dei fd (non puo' ancora salvare il nick sull'array dei nick,e  la stanza sull'array stanze, in quanto non e' arrivato ancora il primo messaggio testuale)
              break; //ed esce poiche' l'ha salvato

            }
    //per 2 motivi esce dal for:
    //1) perche' ha trovato una posizione a -1 quindi disponibile, e lo mette li',
    //2) perche' ha fatto tutte le posizioni fino all'ultimo elemento, senza trovare  un posto libero
    //nel primo caso all'uscita del for, i avra' un valore <FD_SETSIZE e >=0 (anche se in realta' in indice 0 c'e' sempre il listensfd)
    //nel secondo caso invece, i varra' proprio =FD_SETSIZE
    if(i==FD_SETSIZE) {printf("\nTroppi client\n"); continue;}
                                    //nel caso in cui non sia riuscito a salvaro in nessuno degli FD_SETSIZE elementi dell'array (quindi array pieno di fd)
                                    //si fa tutto il ciclo for, senza uscire mai, quindi i avra' i lvalore FD_SETSISZE di uscita dal for, quindi:
    //in caso contrario prosegue e aggiunge il nuovo fd del nuovo client al set di controllo:
    FD_SET(connsfd,&set1);


    if(connsfd>maxfd) maxfd=connsfd; //se il nuovo fd e' maggiore del precedente max, diventa lui il nuovo massimo da passare alla select
    if(i>iultimofd) iultimofd=i; //ricorda che i mantiene ancora il valore dell'indice in cui abbiamo messo il nuovo connsfd. E non e' possibile che si
                                                        //sia arrivati a questo punto del programma, senza arrivare ad inserire l'elemento, poiche' se l'array e' pieno, allora dal for
                                                        //si usciva per i=FD_SETSIZE, e ci sarebbe stato il continue a far riavviare il ciclo facendoci tornare indietro
                                                        //quindi nel caso in cui il nuovo elemento sia stato inserito dopo quello che precedentemente era l'ultimo, viene aggiornato l'indice di ultimo



    if(--nfdready<=0) continue; //cioe' in questo giro di select, il listenfd e' risultato ready. L'abbiamo trattato, e nel caso in cui diminuendo di 1 il numero tot dei fd ready, esca <=0
                                                        //vuol dire allora che non  ci sono altri socket pronti, quindi e' inutile continuare, quindi riavvia il for

    }  //FINE CONTROLLO SE E' STATO IL LISTENING SOCKET A FAR SCATTARE LA SELECT

    ///ora dobbiamo controllare  se sono scattati in ready anche i connsfd dei client connessi (che stanno nell'array)
   ///IN QUESTO PUNTO DEL PROGRAMMA CI ARRIVA SE LA SELECT E' SCATTATA, MA NON E' STATO IL LISTENING SOCKET, OPPURE E' STATO IL LISTENING SOCKET MA NON E' STATO L'UNICO

    int j;
    for(i=1;i<=iultimofd;i++)   {
            if( (connsfd=multiplexedfds[i]) <0) //cioe' se scorrendo sull'array, l'elemento preso in considerazioone ad indice i ha ancora valore di inizializzazione -1 allora passa avanti
                    continue;
                //altrimenti continua in avanti con la i che riguarda sempre quel connsfd sull'array che ha all'interno un socket che dobbiamo controllare:

            if(FD_ISSET(connsfd,&rset)) { //allora quel dato socket e' ready in lettura, quindi quel dato client ci sta trasmettendo qualcosa

                if( (r=full_read(connsfd,toreceive,300)) ==0) { //leggiamo, e se in tot non leggiamo neanche un carattere, vuol dire che e' arrivato il FIN
                                        //quindi il client ci ha chiuso la connessione. Dobbiamo chiudere la connessione, eliminare quel fd dal set, eliminare il fd dall'array dei fd dei connsfd, e deallocare la stringa
                                        //del nick di quel client (in modo tale che il puntatore dell'array dei nick in posizione i torni ==NULL e possa essere utilizzato da qualche altro client)
                                        //ed inoltre deallocare il char che sull'array delle stanze rappresenta la stanza di quel client
                                        //per prima cosa pero' stampiamo il messaggio che ci dice che quel client si sta disconnettendo (e possiamo stampare il suo nick in quanto sappiamo qual e' l'indice i del client sull'array dei nick che ci ha inviato il fin)

                    ///manda a tutti gli altri client il messaggio di tipo SRV per avvisarli che il client sta uscendo:
                    toall_mexSRV_uscitautente(connsfd,nickclients[i],iultimofd,multiplexedfds,stanzefds,stanzefds[i]) ;  //visto che il client era gia' connesso e aveva gia' inviato 1 messaggio, nickclients[i] non puo' essere un puntatore a NULL

                    //ed inoltre aggiungiamo un evento nel logfile che ci dice che il client si e' disconnesso
                    if(clientdisconnesso_salvalog(connsfd,nickclients[i])<0) {printf("\n***LOG-CORROTTO!\n"); fflush(stdout); }





                    close(connsfd); //chiudiamo quel socket
                    FD_CLR(connsfd,&set1); //lo togliamo dal set
                    multiplexedfds[i]=-1; //lo reinizializziamo (quindi risulta di nuovo disponibile la posizione)
                    free(nickclients[i]);// e deallochiamo la stringa del nick dal puntatore in posizione i dell'array dei nick

                }

                else { //allora non abbiamo letto 0 char, ma abbiamo letto qualcosa, quindi dobbiamo girare quel messaggi oa tutti gli altri client connessi
                            //prima di iniziare a girare il messaggi oa tutti gli altri utenti, controlliamo se e' gia' presente il nick di quell'utente in indice i dell'array dei nick
                            //se non lo e', vuol dire che questa e' la prima volta che il client parla. Quindi e' il momento IN CUI CONOSCIAMO il suo nick e la stanza in cui vuole risultare, e lo dobbiamo mettere
                            //nei rispettivi array, ed e' il momento in cui possiamo avvisare tutti gli altri client del fatto che un nuovo client si e' connesso

                    if(nickclients[i]==NULL) //quindi il client sta mandando il primo messaggio da quando si e' connesso
                        {//quindi in questa fase otteniamo il nick del client, la stanza scelta da lui, avvisiamo tutti gli altri che si sta connettendo, e
                          //salviamo i dati di quel client nel logfile:

                          nickclients[i]=calloc(15,sizeof(char)); //allochiamo la stringa dal puntatore a char in posiz i dell'array
                          strncpy(nickclients[i],toreceive,14); //ci copiamo i primi 14 char del messaggio ricevuto da quel client, ovvero il nick
                          *(nickclients[i]+14)='\0'; //e terminiamo la stringa del nick


                          stanzefds[i]=toreceive[14]; //la stanza e' il quindicesimo char del messaggio ricevuto
                          //ora avvisiamo tutti gli altri client (nella stessa stanza) che quel client (attualmente connsfd)si e' connesso usando una funzione esterna al main,
                          //che manda un messaggio di tipo SERVER (ovvero con la parte riservata del nick vuota)
                          toall_mexSRV_nuovoutente(connsfd,nickclients[i],iultimofd,multiplexedfds,stanzefds,stanzefds[i]);
                          //e adesso salviamo l'ingresso del client nel logfile del server (che sta nella cartella dell'exe e si chiama sempre logfile)
                        if(clientconnesso_getinfo_salvalog(connsfd,nickclients[i])<0) {printf("\n***LOG CORROTTO!"); fflush(stdout); }
                        } //se invece il puntatore a stringa dell'array dei nick non e' NULL, vuol dire che gia' e' presente il nick di quel client(quindi aveva gia' mandato i lsuo primo messaggio in chat)
                        ///per come e' organizzato il programma, il fd,la stanza ed il nick avranno sempre lo stesso indice i sui rispettivi array


                    //qui iniziamo a girare il messaggio dopo il nick (cioe' il messaggio del client vero e proprio)a tutti gli altri:
                    for(j=1;j<=iultimofd;j++) //gira il messaggio letto dal socket del client iesimo, a tutti i socket connessi alla stessa stanza,e validi, tranne che a lui stesso
                        if(multiplexedfds[j]!=connsfd && multiplexedfds[j]>=0 && stanzefds[j]==stanzefds[i]){
                            do w=write(multiplexedfds[j],toreceive,300);
                                    while(w<0 && errno==EINTR);
                            if(w<=0) {perror("\nIMPOSSIBILE GIRARE UN MESSAGGIO AD UNO DEI CLIENT"); continue;
                            }
                        }

                }

            if(--nfdready<=0) break;
            }
    } //chiusura for interno

} //chiusura for esterno

} //chiusura main



ssize_t full_read(int fd,void *vptr,size_t n) {
size_t nleft;
ssize_t nread;
char *ptr;

ptr=vptr;
nleft=n;
while(nleft>0) {
    if( (nread=read(fd,ptr,nleft)) <0) {
        if (errno==EINTR)
            nread=0;
        else
            return (-1);


    }
   else if (nread==0) //per arrivare qui, deve essere la read ad avere restituito 0, se invece lo poniamo noi nread==0 per l'EINTR, non arriva qui perche' c'e' else
                    break;
    nleft-= nread;
    ptr += nread;

}
return (n-nleft);

}


void toall_mexSRV_nuovoutente(int connsfd,char nickname[],int iultimofd,int multiplexedfds[],char stanze[],char stanzamitt)    {
int j,w;
//questa funzione avvisa tutti dell'ingresso di un nuovo client
///inviamo a tutti i client connessi un tipo di messaggio SERVER, ovvero che segue lo standard
///per il quale il campo riservato al nick e' vuoto, mentre nel campo del messaggio c'e' la stringa che deve
///stampare il client.
//siccome questa e' una "comunicazione di servizio" dobbiamo costruire il messaggio secondo il nostro standard, adeguatamente

char tosend[300]="**************";
strcat(tosend," IL CLIENT ");
strncat(tosend,nickname,strlen(nickname));
strcat(tosend," SI E' CONNESSO ALLA STANZA DI CHAT");

///
printf("\nINVIO MESSAGGIO BROADCAST AGLI UTENTI NELLA STANZA %c",stanzamitt);
fflush(stdout);
///

for(j=1;j<=iultimofd;j++)
    if(multiplexedfds[j]!=connsfd && multiplexedfds[j]>=0 && stanzamitt==stanze[j] ){
       do w=write(multiplexedfds[j],tosend,300);
            while(w<0 && errno==EINTR);
        if(w<=0) {perror("\nIMPOSSIBILE GIRARE UN MESSAGGIO SRV AD UN CLIENT"); continue;
        }
    }

}



void toall_mexSRV_uscitautente(int connsfd,char nickname[],int iultimofd,int multiplexedfds[],char stanze[], char stanzamitt)  {
//questa funzione avvisa tutti dell'uscita di un client mandando un messaggio di tipo SERVER (vedi sopra)

int j,w;
char tosend[300]="**************";

strcat(tosend," IL CLIENT ");
strncat(tosend,nickname,strlen(nickname));
strcat(tosend," SI E' DISCONNESSO DALLA SESSIONE DI CHAT!");

///
printf("\nINVIO MESSAGGIO BROADCAST AGLI UTENTI NELLA STANZA %c",stanzamitt);
fflush(stdout);
///


for(j=1;j<=iultimofd;j++)
    if(multiplexedfds[j]!=connsfd && multiplexedfds[j]>=0 && stanzamitt==stanze[j] ){
       do w=write(multiplexedfds[j],tosend,300);
            while(w<0 && errno==EINTR);
        if(w<=0) {perror("\nIMPOSSIBILE GIRARE UN MESSAGGIO SRV AD UN CLIENT"); continue;
        }
    }


}

int clientdisconnesso_salvalog(int connsfd,char nickname[]) {
FILE *pfile;
char percorso[FILENAME_MAX];
time_t datacodif;
struct tm *dataleggibile;
//recuperiamo data
time(&datacodif);
dataleggibile=localtime(&datacodif);
//ora nei campi della struct puntata da dataleggibile ci sono tutte le info sulla data e ora




getcwd(percorso,sizeof(percorso));
strcat(percorso,"/");
strcat(percorso,"logfile");
strcat(percorso,".txt");
if((pfile=fopen(percorso,"a"))==NULL)
    return -1; //non riesce ad aprire il file
else{
        if(fprintf(pfile,"\n//%d:%d:%d %d-%d-%d// l'utente %s  si e' disconnesso ",dataleggibile->tm_sec,dataleggibile->tm_min,dataleggibile->tm_hour,
                   dataleggibile->tm_year,dataleggibile->tm_mon,dataleggibile->tm_mday,nickname)<0)
                return -1; //non riesce a salvare il log
        fflush(pfile);
        fclose(pfile);
        printf("\n***LOG DISCONNESSIONE UTENTE (A SEGUITO DI FIN) SALVATO\n");
        fflush(stdout);
        return 0;
}

}


int clientconnesso_getinfo_salvalog(int connsfd,char nickname[]) {
time_t datacodif;
struct tm *dataleggibile;
FILE *pfile;
char percorso[FILENAME_MAX],ipclient[INET_ADDRSTRLEN];
int portaclient;
struct sockaddr_in infoclient;
socklen_t sizestruct=sizeof(infoclient);
//recuperiamo data
time(&datacodif);
dataleggibile=localtime(&datacodif);
//ora nei campi della struct puntata da dataleggibile ci sono tutte le info sulla data e ora

//e ip/porta client connesso al socket
getpeername(connsfd,(struct sockaddr *)&infoclient,&sizestruct);
inet_ntop(AF_INET,&infoclient.sin_addr,ipclient,sizeof(ipclient));
portaclient=ntohs(infoclient.sin_port);

getcwd(percorso,sizeof(percorso));
strcat(percorso,"/");
strcat(percorso,"logfile");
strcat(percorso,".txt");
if((pfile=fopen(percorso,"a"))==NULL)
    return -1;



else{

        if(fprintf(pfile,"\n//%d:%d:%d %d-%d-%d// l'utente %s (fd:%d  ip:%s  porta:%d) si e' connesso",dataleggibile->tm_sec,dataleggibile->tm_min,dataleggibile->tm_hour,
                   dataleggibile->tm_year,dataleggibile->tm_mon,dataleggibile->tm_mday,nickname,connsfd,ipclient,portaclient)<0)
        fflush(pfile);
        fclose(pfile);
        printf("\n***LOG CONNESSIONE UTENTE (A SEGUITO DI PRIMO MESSAGGIO) SALVATO");
        fflush(stdout);
        return 0;

}

}


























/**CLIENT CHAT
UN CLIENT PUO :
--IMPOSTARE I PARAMETRI TCP/IP D ICONNESSIONE E PUO'INVIARE MESSAGGI DI CHAT TRAMITE IL SERVER
--SUL SUO PC VENGONO SALVATE (NELLA STESSA CARTELLA DEL FILE EXE, CON NOME IL NICK COL QUALE IL CLIENT SI E' CONNESSO)
I FILE DELLA CRONOLOGIA DELLE SUE SESSIONI DI CHAT (di cui manda una copia al server) **/

  ///Il server manda al client 2 tipi di messaggi. Un messaggio normale, che contiene nei primi 14 char il nick del client del quale stiamo girando il messaggio,
///e i rimanenti sonoil messaggio girato di quel client, e un messaggio di tipo SERVER, ovvero una comunicazione di servizio del server
///che non contiene che possono essere scelti come nickname(combinazione riservata), nello spazio riservato al nick, e nello spazio del messaggio contiene una stringa che deve essere stampata in quanto tale dal client
///Ogni volta che un client riceve quindi un messaggio, controlla se e' di tipo CLIENT o SERVER in base a quello che c'e' nei primi 14 char!

///In ogni messaggio in generale,, i primi 14char sono riservati al nick, il 15esimo e' la stanza, e poi viene il testo dal 16esimo incluso in poi



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



/** NB: il while(getchar!='\n') per svuotare il buffer stdin, va fatt oSOLO QUANDO SIAMO SICURIi che nel buffer ci sono tot caratteri
piu' un '\n' altrimenti il programma si blocca finche' uno non preme invio, inserend quindio un '\n' nello stdin **/

int salva_cronologia(char *dasalvare,char *nickname,char *percorsofinale); // si passa pure il nickname perche' il nick usato per chattare diventa il nome del file della cronologia
ssize_t full_read(int fd,void *vptr,size_t n);
int chat(struct sockaddr_in *servind, struct sockaddr_in *nostrind,char *nickname,char *percorso);
int tcpipset(struct sockaddr_in *servind, struct sockaddr_in *nostrind,char *nostroip,char *serverip,char *nickname);

int main(int argc,char *argv[]) {
int ris,chatvalue;
char percorso[FILENAME_MAX];
struct timeval tempo1;
int allset=0; //variabile logica booleana
char nostroip[INET_ADDRSTRLEN]="",serverip[INET_ADDRSTRLEN]="",nickname[15]="";
struct sockaddr_in servind,nostrind;
printf("\t\t\aPROGRAMMA DI CHAT CLIENT-SERVER TCP/IP\n \n\t\t\t\t\t\t\tSito Davide");
do { //menu
printf("\nCosa desideri fare ?\n");
printf("[1]Entra nella chat\n[2]Modifica Impostazioni TCP/IP\n");
printf("[0]Esci dal programma\n");
scanf("%d",&ris);
while(getchar()!='\n'); //liberiamo lo stdin
if(ris!=1 && ris!=2 && ris!=0) {printf("\n\aHai inserito un carattere di risposta non valido!\n"); continue;}

if(ris==1)
{if(!allset)
{printf("\n\aDurante l'attuale esecuzione, non sono stati ancora definiti i parametri TCP/IP per la connessione (o quelli inseriti non sono validi)\nVerra' lanciata la configurazione...");
ris=2; //quindi passa alla scelta 2 del menu'
sleep(2);
}

else { //inizio sessione di chat, viene lanciata la funzione per la chat, e il flusso del programma rimane intrappolato li', finche' il client non decide di uscire, o il server non chiude col client (perche' e' il server a chiudere o
//perche' il client e' kickato (nel primo caso arriva solo il fin (close(socket)) quindi il client legge 0, nel secondo invece legge sempre zero, ma prima gli viene inviato un messaggio dal server che lo avvisa del kick imminente!
printf("\nTi stai connettendo ad una sessione di chat verso (%s-%d) con nickname %s\nScrivi quit per USCIRE\n",serverip,ntohs(servind.sin_port),nickname);
tempo1.tv_sec=2; tempo1.tv_usec=0; printf("\a");

select(NULL,NULL,NULL,NULL,&tempo1); //attendiamo qualche secondo (tanto per...)

//prima di lanciare la chat, dobbiamo recuperare il percorso della cartella in cui si trova l'exe, che passeremo alla funzione chat, che passera' alla funzione
//della cronologia, per salvare nella stessa cartella il file delle conversazioni passate.
getcwd(percorso,sizeof(percorso)); //questa funzione salva in percorso l'indirizzo (escluso il nome dell'exe) in cui si trova l'exe



chatvalue=chat(&servind,&nostrind,nickname,percorso);
//la funzione chat ritorna quando e' stata chiusa la sessione di chat, e quindi la connessione al server
//quindi stampiamo un messaggio che in base al valore ritornato da chat, ci dice cosa e' successo
if(chatvalue==-1)
//errore di qualche tipo
{perror("\n>>PERROR:"); //lafunzione perror stampa (stringa di messaggio) che e' contenuta  nello stderr (che come errno, viene impostato dopo l'ultimo errore, e pulito dopo l'ultima esecuzione andata a buon fine)
printf("\nC'e' stato un errore di qualche tipo. Ricontrollare i parametri TCP/IP forniti, e controllare che il SERVER sia in esecuzione\n");
}
else if(chatvalue==0)
//il server ha chiuso la connessione
printf("\nIl server ha interrotto la connessione\n");
else if(chatvalue==1)
//noi abbiamo chiuso per primi
printf("\nAbbiamo interrotto la connessione\n");
}

}
if(ris==2)
allset=tcpipset(&servind,&nostrind,nostroip,serverip,nickname); //inseriamo porte e ip dei 2 end point


}
while(ris!=0);

printf("\n\aPROGRAMMA TERMINATO\n");
return 0;


}


int chat(struct sockaddr_in *servind, struct sockaddr_in *nostrind,char *nickname,char *percorso) {
char cronologiatemp[1200]="\n********INIZIO SESSIONE********\n\0"; //questa inizializzazione viene riportata nel file di cronologia, solo 1 volta per ogni start del programma. Quindi ci indica proprio quando inizia la sessione!!!
FILE *pfile;
int stanza;
//se la funzione di chat ritorna 0, allora la connessione e' stata chiusa dal server (per un kick, o perche' il server si e' spento)
//se la funzione ritorna 1, allora e' stato il client a decidere di chiudere la connessione e quindi la chat
//se ritorna -1 c'e' stato un qualunque altro errore (nb mentre negli esempi facevamo exit(1) qui dobbiamo tornare al menu' principale, non uscire dal programma,
//quindi facciamo ritornare -1)
char *tosend,*toreceive,*testo;  //i buffer di memoria dell'applicazione che usiamo per mantenere i messaggi li allochiamo e deallochiamo ad ogni ciclo, in modo tale da evitare caratteri residui dei precedenti messaggi sul toreceive e sul tosend

int connsfd,ret,keep4us=1,keep4srv=1;
int r,w;
char nickmittente[15]="";
fd_set set1;

//RIEMPIAMO col nickname (cioe' il nome del file che dovra' essere creato per la cronologia) gia da ora
//il percorso completo del file della cronologia, altrimenti se lo facesse la funzione della cronologia,
//finirebbe per aggiungere nick al percorso ogni volta che viene chiamata
strcat(percorso,"/");
strcat(percorso,nickname);
strcat(percorso,".txt");

struct timeval tempo2;
if((connsfd=socket(AF_INET,SOCK_STREAM,0))<0)
{printf("\nImpossibile creare il socket per la connessione\n"); return -1;}


if(bind(connsfd,(struct sockaddr *)nostrind,sizeof(*nostrind))<0) //nostrind e' gia' puntatore all'indirizzo della struct perche' e' stato passato come tale alla funzione chat
{printf("\nImpossibile assegnare al socket di connessione, IP e/o porta stabiliti. Ricontrollare le impostazioni!\n");
 //se nel precedente tentativo di connessione avevamo gia' bindato su una data porta, e ora riproviamo sulla stessa senza rimodificarla e prima
 //CHE SIA PASSATO IL TIME_WAIT MINIMO che deve passare dopo un close(socket) prima che venga liberata davvero la porta di quel socket,
 //otteniamo errore di addr already in use che ha come errno==98. Quindi trattiamo questo caso:
 if(errno==98) printf("\nOccorre un tempo minimo (TIME_WAIT) affinche' la porta bindata al precedente tentativo, venga davvero liberata e possa essere riutilizzata.\nCambia porta!\n");
 return -1;}

do ret=connect(connsfd,(struct sockaddr *)servind,sizeof(*servind)); //servind e' gia l'indirizzo della struct
while(ret<0 && errno==EINTR); //cosi' riavvia se la system call() connect e' interrotta da un signal
if(ret<0) {printf("\nImpossibile portare a termine la connessione (3WH) col SERVER\n"); return -1;} //cosi' gestisce un errore che non sia un SIGNAL
printf("\nConnessione stabilita\n\n");
fflush(stdout); ///scegliamo la stanza (l numero viene convertito in char e diventa il 15esimo char del messaggio)
do{printf("\nIn che stanza vuoi entrare(1-9) ?\n");
      scanf("%d",&stanza); ///la stanza puo' essere un numero da 1 a 9, in modo tale che il numero possa diventare un char che indica proprio il numero sommandogli 48

}
while(stanza<=0 || stanza>=10);
printf("\nSEI NELLA STANZA %d\n",stanza);
fflush(stdout);
while(getchar()!='\n');//puliamo il buffer dall'ultima scanf
//ora che il client ha connesso il socket al server, multiplexiamo l'io tra lo stdin in lettura e il socket di connessione
//lo stdin risulta ready in lettura (e fa scattare la select) nel momento esatto in cui vengono digitati dei char e viene premuto invio IN UN PRECEDENTE MOMENTO
//DI BLOCCO DEL PROGRAMMA . Quindi se il blocco del programma avviene per una select, se noi durante questo blocco, digitiamo char e premiamo invio, lo stdin
//si e' riempito indipendentemente da una funzioen di stringhe che potrebbe stare piu' avanti, arrivando sulla quale, avendo gia' riempito lo stdin durante il blocco della select,
//si limitera' a prendere i char presenti in questo, invece di ribloccare di nuovo flusso

//il client cicla all'infinito, finche' non switcha una variabile booleana che indica che lui vuole uscire dalla sessione di chat o una variabile
//boleana che indica che il server gli ha chiuso la connessione (keep4us e keep4srv)
do{
printf("\n***[[SCRIVI QUI]]:");
fflush(stdout); ///il messaggio va stampato prima della select, poiche' quando parte la select, si blocca il programma,e  quello che inseriamo
///premendo poi enter, gia' va nello stdin, rendendolo ready (e poi si sblocchera' il programma e la fgets prendera' i char che avevamo messo nello stdin)
///Quindi se mettessimo il messaggio dopo che si e' sbloccato lo stdin, apparirebbe nel momento SUCCESSIVO a quando il messaggio l'abbiamo gia' inserito.
///Inoltre non basta metterlo prima della select, in quanto printf manda il messaggi onello stdout, ma prima che lo stdout
///venga scaricato a schermo, la SELECT E' PIU' "VELOCE" e viene eseguita (bloccando il programma) prima che il messaggio dallo stdout venga scaricato a schermo!
///Quindi non solo dobbiamo mandare la scritta "SCRIVI QUI--->" nello stdout con la printf, ma dobbiamo forzare anche
///lo scaricamento dello stdout sullo schermo con la fflush(stdout) prima che venga eseguita la select (la fflush e' immediata e anticipa la select anche)


FD_ZERO(&set1);
tempo2.tv_sec=1500; //ad ogni nuovo ciclo, viene settato un timer a 1500 secondi, scaduti i quali il client si disconnette automaticamente
FD_SET(fileno(stdin),&set1);
FD_SET(connsfd,&set1);


if(select(connsfd+1,&set1,NULL,NULL,&tempo2)==0) //si blocca sulla select, se la select va in timeout, quindi ritorna zero, allora dobbiamo disconnetterci automaticamente
{printf("\n\aSIAMO STATI INATTIVI PER TROPPO TEMPO (1500s). AUTODISCONNESSIONE IN CORSO...\n");
close(connsfd); keep4us=0; //quindi uscira' ritornando 1 e viene cancellato il set
}



if(FD_ISSET(connsfd,&set1)) //si e' sbloccato in lettura il socket (vuol dire che il server ci sta inviando qualcosa
{
toreceive=calloc(300,sizeof(char));

r=full_read(connsfd,toreceive,300);


//il caso del signal e' gestito all'interno della full_read()
if(r<0) {printf("\nImpossibile leggere dal socket\n");
             //indipendentemente da come sia andata l'ultima read(), nel buffer cronologia temp ci potrebbero essere precedenti messaggi
             //non ancora scaricati sul file, in tal caso li scarichiamo
              if(strlen(cronologiatemp)>0) // && if
                if(salva_cronologia(cronologiatemp,nickname,percorso)<0)  printf("\nImpossibile salvare parte della cronologia");
              free(toreceive);
              return -1;
             }
if(r==0) {keep4srv=0; //se leggiamo 0, vuol dire che il server ha chiuso la connessione
                free(toreceive);
                }
else {
///abbiamo ricevuto un messaggio. Ora dobbiamo capire se e' una comunicazione DI SERVIZIO (mex server) dal server, o un messaggio
///che il server ci ha girato ma che e' stato inviato da un altro client, e per fare questo guardiamo ai primi 14 char del mess ricevuto, cioe' ai char riservati al nick

if(strncmp("**************",toreceive,14)==0) //cioe' se nei primi 14 char del mess ricevuto c'e' la comb riservata
           {printf("\n---SERVER:%s",toreceive+14); //allora stampa direttamente tutta la stringa che viene dal 15esimo char inclouso in poi
            fflush(stdout);
}
else{ //allora in tal caso abbiamo ricevuto un messaggi oche era stato girato dal server da parte di un altro client
//sappiamo che i primi 14 char di toreceive sono tutti dedicati al nick, poi dal 15esimo parte il messaggio.
//Quindi ci portiamo fuori il nick, e lo terminiamo con \0 in quanto non e' terminato all'interno di toreceive, altrimenti
//il client leggerebbe solo ilnick e penserebbe di aver incontrato l'eof
    strncpy(nickmittente,toreceive,14);
    nickmittente[14]='\0';
    printf("\n<<:%s \nscrive:>>%s",nickmittente,toreceive+14);
    fflush(stdout);
}
//sia i messaggi normali che quelli di servizio sono salvati nella cronologia
    if( (1200-strlen(cronologiatemp))<strlen(toreceive) ) //cioe' se nel buffer dove di volta in volta copiamo il contenuto di toreceive/tosend, prima di deallocarli,
            if(salva_cronologia(cronologiatemp,nickname,percorso)<0) //i 2 if possono andare tranquillamente in &&
                printf("\nImpossibile salvare parte della cronologia");//non c'e' piu' abbastanza spazio per l'ultimo contenuto di toreceive che vogliamo metterci, trasferisci il contenuto di cronologiatemp
                                                                                                      //nel file sul disco dove salviamo di volta in volta TUTTA la cronologia totale
    //ed in ogni caso poi copia il contenuto di toreceive in cronologiatemp (che eventualmente e' stato svuotato)
    //la copia la facciamo con una strcat, dopo aver pero' fatto una strcat tra cronologiatemp e la string "\n" in modo tale che ogni nuovo contenuto
    //di toreceive stia su un nuovo rigo all'interno di cronologia temp e del file vero e proprio della cronologi

    strcat(cronologiatemp,"\n");
    strcat(cronologiatemp,toreceive); //conserviamo anche la parte del nick, cosi' recuperando una cronologia si sa chi dice cosa

}

if(keep4srv) //lo svuotamento avviene solo nel caso in cui non sia gia' stato effettuato quando e se il server ci ha disconnessi precedentemente
    free(toreceive); //altrimenti avverrebbe un seg fault
}

int f;
if(FD_ISSET(fileno(stdin),&set1))  //si e' sbloccato lo stdin in lettura
/** I CARATTERI VENGONO MANDATI AL BUFFER STDIN, NON SEMPLICEMENTE DIGITANDOLI SULLA TASTIERA, MA DIGITANDOLI
E PREMENDO INVIO. OGNI QUAL VOLTA IL PROGRAMMA SI TROVA IN UNA FASE DI STASI/PAUSA/BLOCCO, QUINDI
NON SOLO DURANTE LE PAUSE DELLE FUNZIONI DI STRINGHE TIPO SCANF.
ANZI, LE FUNZIONI DI STRINGHE VANNO IN PAUSA SOLO QUANDO LO STDIN NON E' STATO PRECEDENTEMENTE RIEMPITO ED E' VUOTO.
QUINDI INNANZITUTTO SE IL BUFFER STDIN E' "SPORCO" C'E' SEMPRE ALLA FINE UN '\N' CHE E' L'INVIO USATO PER MANDARLI ALLA STDIN
POI DALLA STDIN VENGONO AD ESEMPIO PRESI SE VIENE LANCIATA UNA FUNZIONE DI STRINGHE (NOI PERO' LANCIANDO LA FUNZIONE DI STRINGHE
 NON CI ACCORGIAMO DI TUTTO QUESTO, POICHE' LA FUNZIONE DI STRINGHE BLOCCA IL PROGRAMMA SE LO STDIN E' VUOTO, CI FA DIGITARE I CHAR, PREMIAMO ENTER, QUESTI VANNO ALLO STDIN, E LA FUNZIONE DI STRINGHE
 LE PRENDE DALLO STDIN (UQINDI NOI PENSIAMO CHE SIA LA FUNZIONE DI STRINGHE A RENDERE READY LO STDIN...MA E' SBAGLIATISSIMO!)
 QUINDI NON HA SENSO PRINTARE DEI MESSAGGI NEL CASO IN CUI SI SIA SBLOCCATA LA STDIN DOPO UNA SELECT POICHE:
 I MESSAGGI VERRANNO STAMPATI SOLTANTO NEL MOMENTO SUCCESSIVO RISPETTO A QUELLO IN CUI AVREMO DIGITATO LA STRINGA E AVREMO PREMUTO ENTER
 ANCHE SE LA FUNZIONE DI STRINGHE STA MESSA DOPO QUESTI MESSAGGI, PROPRIO PERCHE' IL BUFFER STDIN SI PUO 'RIEMPIRE
 OGNI VOLTA CHE IL PROGRAMMA SI BLOCCA. QUINDI DURANTE UNA SELECT SI BLOCCA, NOI DIGITIAMO DEI CARATTERI, E PREMENDO ENTER QUESTI VANNO A RIEMPIRE LO STDIN (A PATTO CHE NON FOSSE GIA' "SPORCO"
QUINDI IN QUESTO MOMENTO PARTE IL FD_ISSET DELLA STDIN, CON AD ESEMPIO IL MESSAGGIO "SCRIVI MESSAGGIO" CHE PERO' VERRA STAMPATO
SOLTANTO DOPO CHE NOI IL MESSAGGIO L'AVREMO GIA' SCRITTO.
UNA SOLUZIONE POTREBBE ESSERE QUELLA DI PRINTARE UN : SCRIVI MESSAGGIO, PRIMA CHE PARTA LA SELECT !**/

{
tosend=calloc(300,sizeof(char));
strncpy(tosend,nickname,15); //copiandone 15, nel caso in cui i lnickname sia i lmassimo possibile (14) ci portiamo dietro in ogni caso l'\0
f=0;
while(f<15) //eliminiamo tutti i newline e i \0 presenti nel nick...
    {if(tosend[f]=='\n' || tosend[f]=='\0')
        tosend[f]=' ';
        f++;
    }
testo=calloc(285,sizeof(char));
fgets(testo,285,stdin);
tosend[14]=(char)(stanza+48); //l'int della stanza, convertito in char, occupa il quindicesimo char del messaggio inviato
tosend[15]='\0'; //...per mettere un solo terminatore dopo l'ultimo char riservato(l'ultimo riservato e' il 15esimo, cioe' il char che corrisponde all'int della stanza)in questo modo
//NB: il terminatore va messo affinche' si possa fare la strcat col testo che gli appenderemo. Se rimanesse, verrebbe inviato solo il nick (EOF)
strcat(tosend,testo); //la strcat attacca testo sul \0 che viene dopo il nickname, e nel totale il tosend (nick+stanza+testo) sara' chiuso dalla stringa che prima era quella di terminazione di testo, che era inserita dalla fgets automaticamente




  if(strcmp(tosend+15,"quit\n")==0) {
                printf("\nChiusura sessione chat in corso\n");
                free(tosend); free(testo); keep4us=0;

  }

  else{do w=write(connsfd,tosend,300); //nel caso in cui invece scegliamo di NON chiudere la sessione di chat, provvediamo a scrivere sul socket
            while(w<0 && errno==EINTR);

            if(w<=0) {printf("\nImpossibile scrivere sul socket\n"); perror("\nERRORE:\n");
                               if(strlen(cronologiatemp)>0) // && if
                                        if(salva_cronologia(cronologiatemp,nickname,percorso)<0)
                                                printf("\nImpossibile salvare parte della cronologia");
                                free(testo); free(tosend); return -1;
            }


         if( (1200-strlen(cronologiatemp))<strlen(tosend) )
                if(salva_cronologia(cronologiatemp,nickname,percorso)<0)
                    printf("\nImpossibile salvare parte della cronologia");

        strcat(cronologiatemp,"\n");
        strcat(cronologiatemp,tosend);



  }


//arrivati qui dobbiamo liberare in ogni caso testo e tosend dalla memoria. Tuttavia, se avevamo scritto quit,
//la variabile keep4us e' diventata 0 ma il free lo avevamo gia' fatto . Quindi se lo rifacessimo di nuovo, darebbe seg fault.
//Quindi lo facciamo solo nel caso in cui non sia stata gia' impostata su 0 keep4us

if(keep4us)
    {free(testo); free(tosend);
    }

}


    }
        while(keep4us && keep4srv); //finche' ne' il server ne' noi stacchiamo

//il server e' stato staccato o da noi prima che facessimo una write, o dal server dopo che abbiamo fatto una read(ricevuto fin)
//questo vuol dire che non abbiamo eseguito istruzioni per svuotare i char rimanenti nel buffer di cronologia, quindi:

if(strlen(cronologiatemp)>0) // && if
        if(salva_cronologia(cronologiatemp,nickname,percorso)<0)  printf("\nImpossibile salvare parte della cronologia");



if(!keep4us) {close(connsfd); return 1;} //staccato da noi (non basta fare return per chiudere il socket, in quanto il connsfd che e' un descrittore dichiarato nella funzione, viene chiuso ritornando dalla funzione, ma il socket che e' un file/processo/buffer del kernel, rimane aperto, quindi il server non sia ccorge della chiusura!
if(!keep4srv) {close(connsfd); return 0;} //staccato dal server

}


int tcpipset(struct sockaddr_in *servind, struct sockaddr_in *nostrind,char *nostroip,char *serverip,char *nickname) {
int ris,nostraporta=-1,serverporta=-1; //il bind del nostro socket per ip e porta di uscita e' obbligatorio come per quello del server
do{ //menu
printf("\nPremi:\n[1]Per definire le porte TCP\n[2]Per definire gli IP\n[3]Per il nickname\n[0]Per salvare le modifiche e uscire\n");
scanf("%d",&ris);
while(getchar()!='\n');
if(ris==1) {
printf("\nInserisci la porta dalla quale \"uscire\": ");
scanf("%d",&nostraporta);
while(getchar()!='\n'); //liberiamo lo stdin
printf("\nInserisci la porta del server: ");
scanf("%d",&serverporta);
while(getchar()!='\n');
}

if(ris==2) {
printf("\nInserisci ip che ci identifica: ");
fgets(nostroip,INET_ADDRSTRLEN,stdin); //la fgets non dovrebbe dare problemi di caratteri rimasti nello stdin, a patto di non digitarne piu' dei max richiesti
nostroip[strlen(nostroip)-1]='\0'; //tuttavia la fgets salva nella stringa destinazione anche l'invio, quindi dobbiamo toglierlo altrimenti la inet_pton non riesce a tradurre

printf("\nInserisci ip del server: ");
fgets(serverip,INET_ADDRSTRLEN,stdin);
serverip[strlen(serverip)-1]='\0';

}

if(ris==3)
{printf("\nChe nickname vuoi usare?: (max 13 char) ");
 fgets(nickname,15,stdin); //se mettessimo 14, ne potremmo arrivare ad inserire 14, ma il 14esimo verrebbe sovrascritto dalla \0 che e' inserita in auto dalla fgets
                                            //quindi al piu' chiediamo di inserirne 15, cosi' se ne inseriamo 14, il quindicesimo e' la chiusura \0

}
if(ris==0)
{
nostrind->sin_family=AF_INET;
nostrind->sin_port=htons(nostraporta);
if(inet_pton(AF_INET,nostroip,&(nostrind->sin_addr))<1) {printf("\n\aSINTASSI NOSTRO IP NON VALIDA, MODIFICHE NON SALVATE\n"); return 0;} //e a livello del menu' non permettera' di iniziare una chat poiche' allset sara' 0

servind->sin_family=AF_INET;
servind->sin_port=htons(serverporta);
if(inet_pton(AF_INET,serverip,&(servind->sin_addr))<1) {printf("\naSINTASSI SERVER IP NON VALIDA, MODIFICHE NON SALVATE\n"); return 0;}

if(strcmp(nickname,"")==0) {printf("\nNon e' stato deciso un nickname. Modifiche non salvate. Riprova\n");  return 0;}
}

}
while(ris!=0);
printf("\n\aMODIFICHE SALVATE CORRETTAMENTE!\n");
return 1;
}


ssize_t full_read(int fd,void *vptr,size_t n) {
size_t nleft;
ssize_t nread;
char *ptr;

ptr=vptr;
nleft=n;
while(nleft>0) {
    if( (nread=read(fd,ptr,nleft)) <0) {
        if (errno==EINTR)
            nread=0;
        else
            return (-1);


    }
   else if (nread==0) //per arrivare qui, deve essere la read ad avere restituito 0, se invece lo poniamo noi nread==0 per l'EINTR, non arriva qui perche' c'e' else
                    break;
    nleft-= nread;
    ptr += nread;

}
return (n-nleft);

}


int salva_cronologia(char *dasalvare,char *nickname,char *percorsofinale) {
//il nickname diventa parte del nome del file

time_t datacodificata;
struct tm *data;
time(&datacodificata);
data=localtime(&datacodificata);


FILE *pfile;
if((pfile=fopen(percorsofinale,"a"))==NULL)
       {
          return -1;
       }

else{
        if(fprintf(pfile,"\n\n%d:%d:%d   %d/%d/%d\n",data->tm_sec,data->tm_min,data->tm_hour,data->tm_year,data->tm_mon,data->tm_mday)<0) return -1; //ogni volta che scarichiamo il buff temp della cronologia sul file, scriviamo anche prima la data
        if(fprintf(pfile,"%s",dasalvare)<0) return -1;
        fflush(pfile); // svuotamento del pnt a file prima di chiuderlo
        fclose(pfile);
        return 0;
}


}
