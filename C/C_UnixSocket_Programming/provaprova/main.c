#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>


void fai_eco(int connsfd);

int main() {

int pid,listensfd,connsfd;

struct sockaddr_in nostroind;
int nostraporta;
printf("\nInserire la porta sulla quale ricevere le connessioni\n");
scanf("%d",&nostraporta);

nostroind.sin_family=AF_INET;
nostroind.sin_port=htons(nostraporta);
nostroind.sin_addr.s_addr=htons(INADDR_ANY);

if((listensfd=socket(AF_INET,SOCK_STREAM,0))<0)
    {printf("\nIMPOSSIBILE APRIRE IL SOCKET DI ASCOLTO\n");
     exit(1);
    }
if(bind(listensfd,(struct sockaddr *)&nostroind,sizeof(nostroind))<0)
    {printf("\nIMPOSSIBILE BINDARE IL SOCKET DI ASCOLTO\n");
     exit(1);
    }
if(listen(listensfd,1024)<0)
    {printf("\nIL SOCKET NON RIESCE A METTERSI IN ASCOLTO\n");
     exit(1);
    }
while(1)
    {
        printf("\nIN ATTESA DI CONNESSIONI \n");
        if((connsfd=accept(listensfd,NULL,NULL))<0)
         {printf("\nERRORE NELL'ACCETTAZIONE DELLA CONNESSIONE\n");
            exit(1);
         }
       if((pid=fork())==0) //allora in tal caso siamo figlio
            { close(listensfd); //se siamo il figlio chiudiamo il listen socket che non ci serve

               /////qui viene fornito il servizio del server, ovvero dal figlio, il padre si limita a rimanere in ascolto e a delegare ogni client a nuovi figli
               //ilservizio di lettura e rinvio dei dati ricevuti (echo) lo deleghiamo ad una funzione esterna

               fai_eco(connsfd);
               close(connsfd);
               exit(0); //nb il figlio qui dobbiamo ucciderlo esplicitamente per forza, altrimenti continua a rimanere nel while, e ogni figlio generera' altri figli all'infinito

            }

       else //altrimenti siamo padre, quindi chiudiamo il connsock(in realta' diminuiamo il numero di occorrenza di 1, ma per essere davver ochiuso deve raggiungere 0, quindi quando verra' chiuso anche dal figlio
              close(connsfd);
              //non va ucciso il processo del padre, che deve continuare a stare in ascolto all'infinito (e' proprio compito del server)



         }

                    }



void fai_eco(int connsfd) {

//la funzione prende in input il filedescriptor del socket sul quale il processo server figlio ha accettato la connessione col client
//legge i dati che il client ha inviato sul socket(quindi li legge con una read) e li mette in un buffer. Poi li riscrive sul socket
//quindi fa un rimbalzo (eco)

char buffer[100];
printf("\nLEGGO DAL SOCKET\n");
read(connsfd,buffer,10);
printf("\nRIMETTO SUL SOCKET LA STRINGA RICEVUTA CHE E' %s\n",buffer);
write(connsfd,buffer,10);
exit(0);


                                      }
