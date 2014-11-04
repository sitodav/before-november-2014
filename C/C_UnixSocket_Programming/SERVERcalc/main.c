#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>

//il client si connette. Poi quando e' connesso, se vuole eseguire una nuova operazione, manda una stringa con scritto
//new, e questo fa partire la funzione del server che fa le operazioni e rimanda i risultati. I client sono multiplexati con una select, mentre
//quando arriva la stringa new per un client, occorre creare un nuovo processo figlio che faccia le operazioni,in quanto visto che il client manda
//i 2 operandi (identificati dalla stringa op1 e op2) se usassimo solo la select, succederebbe che la concorrenza verrebbe fatta per gestire le connessioni
//dei client, pero' quando parte una funzione per eseguire le operazioni per un client, il processo rimane bloccato nella funzione dell'operazione
//finche' non gli viene mandato il secondo operando. Questo eliminerebbe la concorrenza
//iil risultato viene spedito dal server direttamente nella funzione dell'operazione, dal processo figlio

void computa_and_send(int connsfd);
int main() {
int listensfd,nostraporta,ret,i;
int connsfd;


struct sockaddr_in nostrind;

printf("\nInserire la porta su cui ricevere\n");
scanf("%d",&nostraporta);
while(getchar()!='\n');

nostrind.sin_port=htons(nostraporta);
nostrind.sin_family=AF_INET;
nostrind.sin_addr.s_addr=htons(INADDR_ANY);

if((listensfd=socket(AF_INET,SOCK_STREAM,0))<0){
        perror("\nErrore creazione socket ascolto:");
        exit(1);
}

if(bind(listensfd,(struct sockaddr *)&nostrind,sizeof(nostrind))<0){
        perror("\nErrore bind socket di ascolto\n");
        exit(1);
}

if(listen(listensfd,1024)<0){

    perror("\nImpossibile mettere in ascolto il socket");
    exit(1);
}

int pid;
printf("\nServer in attesa di client");
for(;;){
if((connsfd=accept(listensfd,NULL,NULL))<0){
            perror("\nErrore nel proc figlio sull'accept");
            exit(1);
        }


if((pid=fork())==0){ //siamo sul figlio

        close(listensfd);
        printf("\nPROC FIGLIO: client connesso con successo\n");
        computa_and_send(connsfd);
        close(connsfd);
        exit(0);




}
 close(connsfd);

}

}

void computa_and_send(int connsfd){
float question[3];
int ret;
do ret=read(connsfd,question,sizeof(question));
    while(ret<0 && errno==EINTR);
if(ret<0){
    perror("\nerrore sulla read dal connsfd nel processo figlio\n");
    exit(1);
}
if(ret==0){
        printf("\nIl client ha chiuso la connessione senza inviarci l'operazione da computare\n");
        exit(0);
}

printf("\nOPERAZIONE RICHIESTA DAL CLIENT:\n%f ",question[1]);
if((int)question[0]==1) putchar('+');
else if((int)question[0]==2) putchar('*');
else if((int)question[0]==3) putchar('-');
printf(" %f",question[2]);

float op1=question[1];
float op2=question[2];
int opcode=(int)question[0];
float risult;
if(opcode==1) risult=op1+op2;
else if(opcode==2) risult=op1*op2;
else if(opcode==3) risult=op1-op2;

printf("risultato %f",risult);

do ret=write(connsfd,&risult,sizeof(risult));
    while(ret<0 && errno==EINTR);
if(ret<=0){
        perror("\nerrore sulla write del risultato nel proc figlio\n");
        exit(1);
}

printf("scritti :%d    size %d",ret,sizeof(risult));









}


