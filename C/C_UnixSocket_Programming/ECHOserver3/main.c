//server concorrente di eco, con modello I/O di default su blocking
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>

void funzione_eco(int connsfd);

int main() {
int listensfd,connsfd,porta;
struct sockaddr_in nostro_end;
int ret,pid;

printf("\nSu quale porta riceviamo ? \n");
scanf("%d",&porta);

nostro_end.sin_family=AF_INET;
nostro_end.sin_port=htons(porta);
nostro_end.sin_addr.s_addr=htons(INADDR_ANY);

listensfd=socket(AF_INET,SOCK_STREAM,0);
do{ret=bind(listensfd,(struct sockaddr *)&nostro_end,sizeof(nostro_end));
     if(ret<0 && errno!=EINTR) {printf("\nImpossibile bindare il socket\n"); exit(1); }
    }
    while(ret<0);
do{ret=listen(listensfd,1024);
     if(ret<0 && errno!=EINTR) {printf("\nImpossibile mettere in ascolto il socket\n"); exit(1); }
    }
    while(ret<0);

for(;;)       //il programma si blocca finche' non ritorna l'accept.
                //Siccome l'accept, anceh se ritorna un errore, e' perche' il client ha abortito, non dobbiamo uscire dal programma, in quanto non ha senso spegnere un proc server nel caso in cui il clien tno nriesca a connettersi
                //Quindi piuttosto che gestire esplicitamente i casi in cui ritorna erroneamente la accept(), siccome e' una funzione che deve comunque ciclare sempre, la mettiamo in un ciclo

    {   if((connsfd=accept(listensfd,NULL,NULL))<0) continue; //quindi se l'accept non va a buon fine, invece di portare a termine il for esterno, riparte da zero dall'inizio di questo (evitando di creare una fork inutilmente)


        if((pid=fork())!=0)
        { //padre
           close(connsfd); //una volta chiuso il socket di connessione se siamo nel padre, il flusso arrivera' alla fine del for, e ricomincera' da capo per un nuovo client in attesa di essere accettato

        }
      else { //figlio che eredita tutti i fd e le variabili dal padre
                close(listensfd);
                funzione_eco(connsfd); //lanciamo dal figlio la funzione che si occupa dell'eco
                close(connsfd); //qui e' davvero chiuso il socket di connessione
                exit(0); //se siamo nel processo figlio, quando ha fatto quello che deve fare, deve essere abortito
                            //non gestiamo il SIGCHILD con la wait per evitare che il figlio diventi zombie

             }
    }

                }

void funzione_eco(int connsfd) {
char buffer[200]=" ";
int w,r;
//non usiamo una full_read, ma per semplicita' una read semplice
r=read(connsfd,buffer,200);
w=write(connsfd,buffer,200);
printf("letti:%d\nscritti:%d\n",r,w);
                                               }
