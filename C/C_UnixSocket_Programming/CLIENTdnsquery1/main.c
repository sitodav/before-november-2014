#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>

//questo programma prende in input un hostname.
//lancia una routine del resolver (gethostbyname) ottenendo indietro l'ip del server in netorder (salvato in una struct di tipo struct hostent)
//Da li' lo copia pari pari (con memcpy) nella struct sin_addr della struct di tipo sockaddr_in e si connette usando il socket a quella struttura.
//pero' usa anche la inet_ntop su quell'indirizz oottenuto dalla query dns, trasformandolo in presentation format (dotted decimal) salvandolo in una stringa
//cosi' stampa visivamente la stringa del dotted decimal IPv4 corrispondente al server a cui ci stiamo connettendo

int main() {
char server_name[30];
char stringaipserver[INET_ADDRSTRLEN];
printf("\nInserire il nome del server (domain name)\n");
fscanf(stdin,"%s",server_name); //NB: non usiamo gets o fgets perche' mettono l'\n alla fine della stringa, ma in questo modo passandola alla gethostbyname non riesce a tradurlo!
                                                    //questo problema sorge soltanto quando vogliamo inserire il nome da input di tastiera a runtime, se invece lo inseriamo in command-line all'avvio del programma, ovviamente nell'array argv le stringhe vengono salvate per bene automaticamente
struct hostent *query_result;
int connsfd;
struct sockaddr_in indserver;

//risolviamo l'host name usando la subroutine del resolver
if((query_result=gethostbyname(server_name))==NULL) //query result ora punta ad una struct di tipo struct hostent contenente h_name,alias e ip del server
    {printf("\nERRORE:%s\n",hstrerror(h_errno));
      exit(1);
    }
inet_ntop(AF_INET,*(query_result->h_addr_list),stringaipserver,sizeof(stringaipserver)); //mettiamo in stringaipserver l'ip ottenuto con la query al dns (quindi salvato in query result, ma e' in network order, quindi lo traduciamo con la inet_ntop)
printf("\nL'ip del server e': %s",stringaipserver);

//noi per semplicita' di esercizio ci connettiamo soltanto ad uno degli IP risultanti (il server potrebbe averne piu' di 1)

indserver.sin_port=htons(13); //la porta lasciamo che venga stabilita dal programmatore, non inserita via input
indserver.sin_family=AF_INET;
memcpy(&(indserver.sin_addr),*(query_result->h_addr_list),query_result->h_length);


//NB:il campo h_addr_list della struct (tipo struct hostent) e' un puntatore al primo elemento di un array di puntatori
//dove ogni puntatore punta ad una struct del tipo sin_addr che come unico campo un campo con l'indirizzo
//ottenuto dalla query del dns, tenuto in network order.
//quindi dobbiamo prendere questa struct puntata, pari pari (visto che gia' e' in network order) e riempirci lo spazio
//di memoria
//della struct (tipo struct in_addr) sin_addr che e' campo della struct (tipo struct sockaddr_in) di indirizzo al quale connetterci col socket
//Tuttavia, anche se queste 2 struct (sin_addr e quella puntata da *h_addr_list) sono dello stesso tipo, non si puo' fare
//una semplice uguaglianza in quanto sono campi di struct diverse ed il compilatore da errore
//quindi si fa una memcpy, che copia come size quella espressa dal campo h_length (di struct hostent) che dice proprio
//la lunghezza dell'IP in network order ottenuto dalla query, e questa porzione la copiamo nel campo
//sin_addr di sockaddr_in (in realta' sistruct hostent puo' copiare anche in s_addr di sin_addr di sockaddr_in poiche'
//sia la struct puntata da *h_addr_list in hostent sia la sin_addr di sockaddr_in contengono un solo campo, quindi o copiamo
//tutta una struct nel campo dell'altra, o tutta una struct in tutta un'altra, il risultato e' sempre quello)






if((connsfd=socket(AF_INET,SOCK_STREAM,0))<0)
        {printf("\nImp aprire il socket per la connessione\n");
         exit(1);
        }
//non ci interessa bindarlo su un ip o porta di ricezione in particolare

if(connect(connsfd,(struct sockaddr *)&indserver,sizeof(indserver))<0)
    {printf("\nImpossibile connettersi al server\n");
    exit(1);
    }

printf("\nConnessione stabilita\n");



                }
