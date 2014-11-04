//in quest'applicazione lato client, facciamo tutti i controlli sui possibili return delle varie funzioni bloccanti,
//in modo tale da gestire i casi in cui il server vada in crash, o vada in shutdown, oppure si abbia una seg pipe, o un
//RST etc...
//ad esempio parecchie system call se una volta chiamate danno un valore <0 (quindi di errore) nel caso in cui
//errno==EINTR, vanno rilanciate poiche' vuol dire che sono state interrotte da un interrupt. Quindi le si mette in un ciclo
//che continua a lanciarla fin quando il valore restituito e' <0 ma contemporanemanete errno==EINTR
//poi quando uscira' dal ciclo, vuol dire che o ha ritornato un valore >0, corretto, oppure <0 con errno!=EINTR, quindi in quest'ultimo caso
//va interrotto il programma
//questo ciclo funziona poiche' se da errno==EINTR allora la funzione ritorna (quasi sempre sicuramente) <0, mentre se ritorna
//<0 non e' sicuro che errno==EINTR cioe' che sia avvenuto un interrupt

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h> //libreria che usiamo per il controllo sulla variabile globale errno (EINTR e' una costante che indica un intero di errore ed e' definita in errno.h>

int main(int argc,char *argv[]) {
int connsfd;
struct sockaddr_in nostro_end,server_end;
if(argc!=3)
    {printf("\n<IP NOSTRO> <IP SERVER>\n");
     exit(1);
    }
int nostra_porta,server_porta;
printf("\nDa quale porta usciamo ?\n");
scanf("%d",&nostra_porta);
printf("\nQual e' la porta del server ?\n");
scanf("%d",&server_porta);


nostro_end.sin_family=AF_INET;
nostro_end.sin_port=htons(nostra_porta);
inet_pton(AF_INET,argv[1],&nostro_end.sin_addr);

server_end.sin_family=AF_INET;
server_end.sin_port=htons(server_porta);
inet_pton(AF_INET,argv[2],&server_end.sin_addr);

int ret;
if((connsfd=socket(AF_INET,SOCK_STREAM,0))<0) //la system call socket() o va a buon fine o no, non puo' capitare che venga interrotta da un SIGNAL
    {printf("\Impossibile creare il socket \n");
        exit(1);
    }
do {ret=bind(connsfd,(struct sockaddr *)&nostro_end,sizeof(nostro_end)); } //il bind e' facoltativo
     while(ret<0 && errno==EINTR); //lancia la system call bind(). Se va a buon fine (>=0) non fa altro e prosegue.
                                                      //se da un valore di errore, ma errno==EINTR, vuol dire che e' stata interrotta da un signal
                                                      //quindi la rifa'. Se invece da <0 ma errno!=EINTR, allora esce dando messaggio di errore
if(ret<0) {printf("\nImpossibile assegnare la porta da cui uscire\n"); exit(1);} //quando esce dal ciclo, la bind() o ha dato un valore corretto >=0, oppure e' uscito dando <0 con errno!=EINTR, quindi nel secondo caso basta
                                                                                                                      //richiedere come condizione dell'if ret<0, e in tal caso interrompere il processo xke' vuol dire che la bind non e' andata a buon termine ma non e' stato un SIGNAL ad interromperla.


do{ret=connect(connsfd,(struct sockaddr *)&server_end,sizeof(server_end));} //la connect() puo' essere interrotta da un SIGN, quindi noi la continuaimo a lanciare fin quando da un valore <0 con errno==EINTR (che indica che sta venendo interrotta da un sign)
                                                                                                                        //quindi uscira' dal ciclo while e smettera' di richiamarla, quando o avra' dato un valore >0, oppure quando dara' <0 ma con errno!=EINTR (quindi sara' avvenuto un errore vero e proprio)
                                                                                                                        //e in questo secondo caso stampa il messaggio di errore ed esce dal programma
        while(ret<0 && errno==EINTR);
if(ret<0) {printf("\nImpossibile connettere il socket\n"); exit(1); }
printf("\nCONNESSIONE EFFETTUATA\n");

                }
