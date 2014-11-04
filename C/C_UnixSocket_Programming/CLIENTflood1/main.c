//vogliamo scrivere un programma che dato in input da tastiera in command line (in modo tale che si possa
//chiamare questo programma da un altro programma, lanciandolo col nome host come parametro
//il nome di dominio di un host, ne ottiene tutti gli indirizzi ip, e prova a connettersi per ognuno di questi
//su tutte le porte esistenti, riempiendo un array con le porte che ha trovato aperte
//poi stampa l'array con le porte aperte su ogni ip di interfaccia di quell'host

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <netdb.h>

int main(int argc,char *argv[]) {
struct hostent  *query_result; //questo lo facciamo puntare alla struct risultante dalla query al DNS
struct sockaddr_in  strind;      //questa di volta in volta la riempiamo con la porta e l'ip a cui vogliamo connetterci (gli ip son opresi dalla lista h_addr_list del risultato della query al DNS, le porte iinvece sono iterate)
int connsfd;
if(argc!=2)
            {
                printf("\nIns <NOME HOST>\n"); exit(1);
            }
struct  allowed {   //usiamo un array di struct  per contenere i dati delle coppie IP-PORTE aperte. Ogni elemento dell'array e' una struct con un campo per l'ip e un array di porte disponibili a quell'ip (porte NON in network order)
               char dotted_ip[INET_ADDRSTRLEN]; //un array per la stringa presentation dell'ip (non lo teniamo in network order in modo tale che sia possibile stampare al volo dall'array
               int nonet_ports[1000]; //l'array  per le porte non in network order che sono state trovate aperte
                                            };
struct allowed *ptr_allowed=calloc(100,sizeof(struct allowed));

if((query_result=gethostbyname(argv[1]))==NULL)
    {printf("\nERRORE NELLA QUERY AL DNS:%s\n",hstrerror(h_errno));
     exit(1);
    }
char **ptr_list=query_result->h_addr_list;
int i=0;
while( *(ptr_list+i)!=NULL  ) {
    inet_ntop(AF_INET,*(ptr_list+i), (ptr_allowed+i)->dotted_ip,INET_ADDRSTRLEN);
    i++;

                                            }

//cosi' abbiamo messo nell'array di struct puntato da ptr_allowed, per ogni struct nel campo ip_dotted uno degli ind ip
//di quel domain name. Ora per ognuno di quegli ip, dobbiamo scorrere tutte le porte iterativamente, e vedere ogni volta
//quali sono le connesse

printf("num ip trovati per quel domain name %d\n",i); //stampiamo prima
int k;

//poi continuiamo
int indice_porta,j;
for(k=0;k<i;k++)
    {//riempiamo la struct dell'ind al quale connetterci con l'ip di quel k ip trovato, ritraducendolo da presentation a network
      inet_pton(AF_INET,(ptr_allowed+k)->dotted_ip,&strind.sin_addr);
      strind.sin_family=AF_INET;
      //ora lanciamo un ciclo for, nel quale ad ogni passo apriamo (e poi richiudiamo alla fine) un nuovo socket, e utilizziamo
      //una diversa porta inserendola nella struct strind, provando a connetterci tramite il socket aperto in quel passo, all'end point
      //il cui ip e porta sono contenuti in strind: ogni volta che una connessione va a buon fine, la porta viene salvata nella lista delle porte
      //corrispondenti a quell'ip
      indice_porta=0; //riazzeriamo l'indice delle porte disponibili per quell'ip
      printf("su <IP>:%s disponibili:\n",(ptr_allowed+k)->dotted_ip);
      for(j=1;j<100000;j++)
            {strind.sin_port=htons(j); //porta = valore del jesimo tentativo
             connsfd=socket(AF_INET,SOCK_STREAM,0); //apriamo il socket
             //e proviamo a connetterlo a quell'ip e quella porta su cui stiamo tentando
             if(connect(connsfd,(struct sockaddr *)&strind,sizeof(strind))>=0) //in questo caso vuol dire che e' riuscito a connettersi
                {
                 (ptr_allowed+k)->nonet_ports[indice_porta]=j; //e salviamo la porta disp relativa a quell'ip all'indice corretto sul campo-array di int
                                                                                        //nb lasciamo la porta non in net order

                 printf("<PORTA>->%d\n",(ptr_allowed+k)->nonet_ports[indice_porta]); //nel caso in cui la porta sia aperta, dopo averla salvata la stampa
                 indice_porta++; //e incrementa l'indice delle porte disponibili salvate per quel relativo ip
                }
            //se invece non riesce, chiude il socket che e' inutilizzabile, e fa un nuovo passo con una nuova porta
             //in ogni caso il socket (riuscito o non riuscito) va chiuso
             close(connsfd);

            }

    }
     //ora stampiamo per ognuno degli ip trovati, le porte ottenute disponibili




                }
