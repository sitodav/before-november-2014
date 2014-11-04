#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/select.h>
#include <errno.h>

int sessione_streaming(int connsfd);
int main() {

//stabilisce connessione con password
struct sockaddr_in *servind;
char ipsrv[INET_ADDRSTRLEN];
int portasrv,connsfd,ret,i;
char pw[20],toread[300],tosend[300];



printf("\nInserire porta server\n");
scanf("%d",&portasrv);
while(getchar()!='\n');
printf("\nInserire ip server\n");
fgets(ipsrv,INET_ADDRSTRLEN,stdin);

servind=calloc(1,sizeof(struct sockaddr_in));
servind->sin_port=htons(portasrv);
servind->sin_family=AF_INET;
inet_pton(AF_INET,ipsrv,&(servind->sin_addr));

if((connsfd=socket(AF_INET,SOCK_STREAM,0))<0){
        perror("\nImpossibile creare il socket di connessione:");
        exit(1);
}

do ret=connect(connsfd,(struct sockaddr *)servind,sizeof(*servind));
    while(ret<0 && errno==EINTR);
if(ret<0) {
//gestiamo i vari tipi di errori della connect
    if(errno==ECONNREFUSED) printf("\nL'ip del server e' corretto, ma su quella macchina non c'e' nessun processo in ascolto sulla porta indicata\n"); //cioe' il server ci ha risposto con un RST al SYN che gli abbiamo inviato
    else if(errno==ETIMEDOUT) printf("\nServer non raggiungibile (controllare l'ip inserito)\n"); //sono passati 75 secondi in totale  dall'ultimo syn inviato, ma nessuno (neanche router intermedi) ci hanno risposto
    else if(errno==ENETUNREACH) printf("\nServer non raggiungibile\n"); //abbiamo inviato syn, abbiamo ricevuto un ICMP al liv network da un router intermedio, abbiamo aspettato 75 secondi, ma non e' cambiat onulla
    else perror("\n:");
}

else printf("\nConnessione stabilita-Verifica pw:\n");
fflush(stdout);
 //in realta' la connessione viene effettuata prima del momento dell'inserimento della pw...se sbagliamo
 //il main ritorna, quindi viene chiuso il socket di connessione, (inviando il FIN al server)


    printf("\nIns password: ");
    fgets(pw,20,stdin);
    write(connsfd,pw,strlen(pw)+1);
    read(connsfd,toread,300);
    printf("\nRISPOSTA:%s",toread);
    fflush(stdout);


if(strncmp(toread,"valida",strlen("valida"))==0) {
    ret=sessione_streaming(connsfd); //altrimenti e' uscito perche' abbiamo inseritola pw giusta, quindi lancia la sessione vera e propria del server (gli passiamo solo il conns perche' su quel socket in realta' siamo gia' connessi)
    if(ret=0) //allora la sessione streaming e' terminata perche' il server ha chiuso la connessione
            printf("\nIl server ha chiuso la connessione\n");
    else if(ret==-1) printf("\nC'e' stato un errore nella sessione di connessione\n");
    else printf("\nAbbiamo chiuso la sessione di connessione al server\n");
}
else printf("\nPassword errata\n");
fflush(stdout);


return 0;
}


int sessione_streaming(int connsfd) {
FILE *pfile;
int ret,ret2;
char percorso[FILENAME_MAX],nomefile[21],tosend[1000];


printf("\nPASSWORD VALIDA: SIAMO CONNESSI AL SERVER\n");

printf("\nInserisci il nome del file che si vuol trasferir(senza estensione)\n");
fgets(nomefile,20,stdin);
nomefile[strlen(nomefile)-1]='\0';
getcwd(percorso,sizeof(percorso));
strcat(nomefile,".txt");
strcat(percorso,"/");
strcat(percorso,nomefile);
printf("\nvogliamo aprire il file :\n%s",percorso);
fflush(stdout);

pfile=fopen(percorso,"r");
if(pfile==NULL)
        {perror("\nImpossibile aprire il file:");
         return -1;
}

//per prima cosa dobbiamo inviare 20 char al server col nome del file
printf("\nInvio al server nome file\n");
do ret=write(connsfd,nomefile,21);
    while(ret<0 && errno==EINTR);
if(ret<=0) {perror("\nerrore sulla write:"); return -1;}
printf("\nInviato nome del file al server\n");
printf("\nInizio sessione trasferimento-->:\n");
fflush(stdout);
while(!feof(pfile)){ // ora leggiamo il file apert 100 char alla voltao, e io mandiamo al server attraverso il socket  (file di max 1000 char)
    strcpy(tosend,"\0");
    fgets(tosend,100,pfile); //salviamo in to send dal file da cui stiamo leggendo

    printf("\nprelevato dal disco->%s",tosend);
    do ret=write(connsfd,tosend,strlen(tosend)+1);
        while(ret<0 && errno==EINTR);
    if(ret<=0){
            perror("\n");
            return -1;
    }
    printf("\ninviati %d char:\n",ret);
    int j;
    for(j=0;j<ret;j++) printf("%c",tosend[j]);
}

printf("\nInviato tutto il file\n");
fflush(stdout);

return 1;





}
