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


int sessione_calc(int opcode,char servip[],int portasrv); //1 addizione, 2 molt, 3 sottra
int main(){
int ret,risp,portasrv;
char servip[INET_ADDRSTRLEN];
printf("\nInserire la porta del server\n");
scanf("%d",&portasrv);
while(getchar()!='\n');
printf("\ninserire ip server\n");
fgets(servip,INET_ADDRSTRLEN,stdin);
servip[strlen(servip)-1]='\0';


while(1){
    printf("\nCosa vuoi fare ?\n");
    printf("\n[0]ESCI\n[1]Addizione\n[2]Moltiplicazione\n[3]Sottrazione\n");
    scanf("%d",&risp);
    while(getchar()!='\n');
    if(risp==1 || risp==2 || risp==3) {
                ret=sessione_calc(risp,servip,portasrv);
                if(ret==0) printf("\nIl server ci ha disconnessi prematuramente\n");
                else if(ret<0) printf("\nAvvenuto un errore nel programma di calcolo\n");
                else printf("\nSessione di calcolo terminata correttamente\n");
    }
    if(risp==0) break;
    else continue;

}

printf("\nProgramma di calcol terminato\n");


}


int sessione_calc(int opcode,char servip[],int portasrv) { //1 addizione, 2 molt, 3 sottra
//la funzioen non ritorna il risultato, ma ritorna un numero che indica se c'e' stato un errore, o una disconnessione prematura da parte del server, o se tutto e' andato a buon fine
//il risultato invece e' stampato all'interno della stessa funzioen quando lo riceve dal server
float question[3]; //mandiamo questo array: il primo elemento e' il codice dell'operando, poi viene op1 in dice 1, e in indice 2 c'e' il secondo op
int connsfd,ret;
float op1,op2;
struct sockaddr_in servind;
//creiamo e riempiamo la struttura con i dati del server, creiamo i lnostro socket e lo connesttiamo al server

servind.sin_port=htons(portasrv);
servind.sin_family=AF_INET;
inet_pton(AF_INET,servip,&(servind.sin_addr));

if((connsfd=socket(AF_INET,SOCK_STREAM,0))<0){
    printf("\nProblema nella creazione del conns");
    return -1;
}

//connettiamo il client al server
if(connect(connsfd,(struct sockaddr *)&servind,sizeof(servind))<0){
        printf("\nImpossibile portare a termine la connessione col server\n");
        perror("");
        return -1;
}

printf("\nSiamo connessi al server\n");
float risultato;
question[0]=(float)opcode;
printf("\nInserire operando 1\n");
scanf("%f",&op1);
while(getchar()!='\n');
printf("\nInserire operando 2\n");
scanf("%f",&op2);
printf("\nSTIAMO CHIEDENDO AL SERVER DI ESEGUIRE LA SEGUENTE OPERAZIONE\N");
printf("\n%f ",op1);
if(opcode==1) putchar('+');
else if(opcode==2) putchar('*');
else putchar('-');
printf(" %f\n",op2);
question[1]=op1;
question[2]=op2;

//ora inviamo al server
do ret=write(connsfd,question,sizeof(question));
    while(ret<0 && errno==EINTR);
if(ret<=0) {perror("problema nella write del quesito\n"); return -1;}

do ret=read(connsfd,&risultato,sizeof(risultato));
    while(ret<0 && errno==EINTR);
if(ret<0){
    perror("\nErrore nella ricezione del risultato");
    close(connsfd);
    return -1;
}

if(ret==0){
    printf("\nMentre stavamo aspettando il risultato dal server, questo ha chiuso la connessione inaspettatamente\n");
    close(connsfd);
    return 0;
}

printf("\nIl server ci dice che il risultato e' %f",risultato);
return 1;


}
