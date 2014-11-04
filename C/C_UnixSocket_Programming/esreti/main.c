#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc,char *argv[]) {

struct sockaddr_in addr_struct;

int socketfd;
int porta;
char buffer[1025]=" ";
//controllo che sia passato l'ip da command line al lancio del programma da riga di comando (linux: ./nomeprogramma input  )
if(argc!=2)
    {fprintf(stderr,"\nerrore: <command> <IP>\n"); exit(1);
    }
//apertura socket sul file descriptor, e controllo che l'apertura sia avvenuta correttamente
if((socketfd=socket(AF_INET,SOCK_STREAM,0))<0)
    {fprintf(stderr,"\errore: per qualche motiv o non e' stato possibile aprire il socket");
     exit(1);
        }
//ora riempiamo la struttura degli indirizzi che passeremo poi alla funzione che usa il socket per connettersi
//htons(int...) prende in input un int, e restituisce quell'int convertito in network order (cioe' big endian)
//invece la inet_pton(famiglia indr,&stringadaConv,&buffsuCuiSalvare) trasforma una stringa passata, in un indirizzo della famiglia passata, salvandolo nel buffer scelto
//NB: che noi come buff in cui salvare scegliamo il campo sin_addr della struct degli indirizzi..ma questa e' a sua volta una struct, ecco perche' ci vuole un indirizzo anche se la destinazione
//e' una variabile (in questo caso struct) in cui scrivere un intero (se fosse stata una variabile normale avremmo passato il campo, invece essendo una struct dobbiamo passargli
//l'indirizzo dell'intera struct che rappresenta il campo della struct indirizzi)

//qui inseriamo da tastiera la porta, e la convertiamo con htons
printf("\nInserisci <PORTA>\n");
scanf("%d",&porta);
addr_struct.sin_port=htons(porta);


 //NB: la htons va sempre a buon fine. Invece la inet_pton non sempre riesce a tradurre una stringa in un indirizzo network order di una data famiglia
//quindi andrebbe sempre controllato il risultato che restituisce
addr_struct.sin_family=AF_INET; //assegnamo la famiglia di protocollo alla struct

//ora memorizziamo l'ip del server nella struttura degl indirizzi, convertendolo  da stringa
//passata come parametro su command -line
if(inet_pton(AF_INET,argv[1],&(addr_struct.sin_addr))<0)
 {fprintf(stderr,"\nImpossibile convertire l'ip fornito in network address");
   exit(1);

 }

//ora la struttura indirizzi e' pronta
//quindi una struttura indirizzi per un socket di un client necessita' di : famiglia,porta e indirizzo (in network order)

//ora connettiamo il socket che abbiamo creato (lato client) all'indirizzo e alla porta definiti dalla struttura indirizzi addr_struct di tipo struct sockaddr_in
//ed effettuaimo come sempre il controllo che vada a buon finela chiamata della funzione connect() che come le altre, ritorna un valore <0 se c'e' stato un errore

//la funzioen connect vuole connect(fileDESCRP&,struttINDIR,LunghezzaSTruttInd)
if (connect(socketfd, (struct sockaddr *) &addr_struct, sizeof(addr_struct)) < 0) {
fprintf(stderr,"connect error\n");
exit(1);
}

//NB ricorda sempre che la connect vuole in input solo struct di tipo sockaddr poiche' e' vecchia e all'epoca non esistevano i prototipi con void come argomenti
//pero'si puo' passare ugualmente qualunque tipo di struct, pero' bisogna castarla in sockaddr


                  }
