#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>


int lancia_eco(int connsfd);

int main(int argc,char *argv[])
{

    if(argc!=3)
    {
        printf("\n<COMANDO> <IPSERVER> <IPNOSTRO>\n");

    }
    int connsfd,portaserver,nostraporta;

    struct sockaddr_in indserver;
    struct sockaddr_in indnostro;
    printf("\nDA QUALE PORTA VUOI USCIRE ?\n");
    scanf("%d",&nostraporta);
    printf("\nQUAL E' LA PORTA DEL SERVER ?\n");
    scanf("%d",&portaserver);


    indserver.sin_family=AF_INET;
    indserver.sin_port=htons(portaserver);
    inet_pton(AF_INET,argv[1],&indserver.sin_addr);

    indnostro.sin_family=AF_INET;
    indnostro.sin_port=htons(nostraporta);
    inet_pton(AF_INET,argv[2],&indnostro.sin_addr);

    if((connsfd=socket(AF_INET,SOCK_STREAM,0))<0)
    {
        printf("\nImpossibile aprire il socket per la connessione\n");
        exit(1);
    }


    if(bind(connsfd,(struct sockaddr *)&indnostro,sizeof(indnostro))<0)
    {
        printf("\nIMPOSSIBILE ASSEGNARE AL SOCKET LA PORTA DA CUI USCIRE\n");
        exit(1);
    }
    if(connect(connsfd,(struct sockaddr *)&indserver,sizeof(indserver))<0)
    {
        printf("\nimpossibile stabilire la connessione\n");
        exit(0);
    }
    printf("\nConnessione stabilita\n");

//anche il client usa una funzione esterna per mandare il messaggio

    lancia_eco(connsfd);
    close(connsfd);



}

int lancia_eco(int connsfd)
{
    //la funzione prende una stringa, la manda sul socket, e poi rilegge dal socket il messaggio arrivato in risposta
    //se tutto funziona dovra' essere restituita la stessa stringa inviata
    char tosend[100];
    char received[100];

    printf("\nINS PAROLA\n");
    //fgets(tosend,99,stdin);
    fscanf(stdin,"%s",&tosend);
    printf("\nscritti %d",write(connsfd,tosend,10));

    read(connsfd,received,10);
    printf("\nRICEVUTO IN ECHO\n");
    fputs(received,stdout);
    printf("\n");

    return 0;


}
