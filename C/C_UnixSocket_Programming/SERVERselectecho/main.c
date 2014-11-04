///NB questo non e' l'esercizio col server che usa il select. Quello e' SERVERechoselect
//ricreare l'applicazione client server per l'echo che reinvia il conteggio char, col server che continua ad essere concorrente, ma col client
//che gestisce i canali di I/O con multiplexing (select)
//LATO SERVER:

/// LEGGI I COMMENTI DEL FILE DEL CLIENT PER LA TEORIA! (CLIENTselectecho)

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

void count_eco(int connsfd);

int main() {
int connsfd,listensfd,porta,ret,pid;
printf("\n<Receiving Port?:> ");
scanf("%d",&porta);
struct sockaddr_in ourind;

ourind.sin_family=AF_INET;
ourind.sin_port=htons(porta);
ourind.sin_addr.s_addr=htons(INADDR_ANY);

if((listensfd=socket(AF_INET,SOCK_STREAM,0))<0)  //sicuramente il valore <0 non puo' essere dovuto ad un signal, quindi non va riavviata in caso
    {printf("\nImpossibile creare il socket di ascolto\n");
     exit(1);
    }
do  ret=bind(listensfd,(struct sockaddr *)&ourind,sizeof(ourind));
      while(ret<0 && errno==EINTR);
if(ret<0) {printf("\nImpossibile bindare (err != SIGNAL)"); exit(1); }


if(listen(listensfd,1024)<0)
    {printf("\nImpossibile mettere in ascolto il socket\n");
     exit(1);
    }

for(;;)
    { if((connsfd=accept(listensfd,NULL,NULL))<0)
        continue; //nel caso in cui un'accept dia valore incorretto (sia per un errore vero qualunque, che per un SIGNAL) riavvia il ciclo, facendo una nuova accept
                      //sul client successivo disponibile in coda
      if((pid=fork())==0)
            {//figlio
             close(listensfd);
             count_eco(connsfd);
             exit(0);
            }
      else
           //padre
           close(connsfd);

    }
                    }


void count_eco(int connsfd) {
int nchar=0;
char toreceive[200]="\0";
int r;
int i;
//non usiamo la full_read, ma per semplicita' la read () semplice (anche se non sarebbe corretto etc...)
while((r=read(connsfd,toreceive,200))>=0)
{printf("\nSTIAMO FORNENDO SERVIZIO AL CLIENT:\n");
    i=0;
    nchar=0;

    while(i<200) //contiamo i caratteri della parola
        {printf("-%c",toreceive[i]);
        if(toreceive[i]!='\0')
            nchar++;
        i++;
        }
    nchar--; //(perche' conta anche il \n)
if(nchar==0) {printf("\nServizio Terminato per il client!\n"); return;} //la condizione di uscita si verifica quando il client manda una stringa senza caratteri (la read non puo' mai leggere 0 poiche' sicuramente c'e' \n quindi la condizione la facciamo qui)
//per inviare numeri attraverso la rete, li trasformiamo in stringa (con snprintf) e scriviamo la stringa con la write() sul socket
//Il client riceve la stringa, la legge co nread() dal socket, e la ritrasforma in numero (con sscanf)
    printf("\nla parola ricevuta e' composta da %d char\nInviamo la risposta al client\n...",nchar);

    char tosend[200]="\0";
    snprintf(tosend,200,"%d",nchar); //l'algoritmo trova il numero, noi lo convertiamo in stringa, e lo scriviamo nel socket

    if(write(connsfd,tosend,200)<=0)
        {printf("\nImpossibile scrivere sul socket\n");
        return;
        }

    }
                }
