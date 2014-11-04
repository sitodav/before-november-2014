#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <string.h>
#include <errno.h>
#include <time.h>



struct fdmultiplexati {
        char *nickclient;
        char   stanza;
        int     fd;
} ;



void stampa_arraygenerale(struct fdmultiplexati multiplexedfd[],int i,int iultimofd);



int clientdisconnesso_salvalog(struct fdmultiplexati multiplexedfd[],int i_uscente);
int clientconnesso_getinfo_salvalog(struct fdmultiplexati multiplexedfd[],int i_nuovoclient);
void toall_mexSRV_nuovoutente(struct fdmultiplexati multiplexedfd[],int i_nuovoclient,int iultimofd);
void toall_mexSRV_uscitautente(struct fdmultiplexati multiplexedfd[],int i_uscente,int iultimofd);
ssize_t full_read(int fd,void *vptr,size_t n);

int main() {
struct fdmultiplexati multiplexedfd[FD_SETSIZE];
int listensfd,connsfd,ret,r,w,porta,nfdready,i,iultimofd;

fd_set set1,rset;
int maxfd,j,k;
struct timeval tempo1;
char toreceive[300]="",tosend[300]="";
struct sockaddr_in nostrind;

printf("\n\t\t\aSERVER APPLICATION PER CHAT\n\t\t\t\t\t Sito Davide\n");
printf("\nSu quale porta vogliamo ricevere ?");
scanf("%d",&porta);
while(getchar()!='\n');

if((listensfd=socket(AF_INET,SOCK_STREAM,0))<0)
    {printf("\nImpossibile creare il socket di ascolto\n");
        exit(1);
    }
nostrind.sin_family=AF_INET;
nostrind.sin_port=htons(porta);
nostrind.sin_addr.s_addr=htons(INADDR_ANY);

if(bind(listensfd,(struct sockaddr *)&nostrind,sizeof(nostrind))<0)
    {printf("\nImpossibile bindare il socket di ascolto\n");
    exit(1);}

if(listen(listensfd,1024)<0)
    {printf("\nImpossibile mettere in ascolto il socket\n");
     exit(1);}



for(i=0;i<FD_SETSIZE;i++) multiplexedfd[i].fd=-1;
maxfd=listensfd;
multiplexedfd[0].fd=listensfd;
FD_ZERO(&set1);
FD_ZERO(&rset);
FD_SET(listensfd,&set1);
iultimofd=0;


for(;;)
{

    if(iultimofd==0)    printf("\n----CHAT APPENA AVVIATA, IN ATTESA DI CLIENT...\n");
    rset=set1;
    nfdready=select(maxfd+1,&rset,NULL,NULL,NULL);

    if(FD_ISSET(listensfd,&rset)) {

                do connsfd=accept(listensfd,NULL,NULL);
                    while(connsfd<0 && errno==EINTR);
                if(connsfd<0) {perror("\nCONNECT():"); continue;}
                printf("\n****UN NUOVO CLIENT SI E' CONNESSO ALLA CHAT !");  fflush(stdout);

                for(i=0;i<FD_SETSIZE;i++)
                        if(multiplexedfd[i].fd<0) {
                            multiplexedfd[i].fd=connsfd;
                            break;
                        }

                if(i==FD_SETSIZE) {printf("\nTROPPI UTENTI CONNESSI\n"); continue;}

                FD_SET(connsfd,&set1);


                if(connsfd>maxfd) maxfd=connsfd;
                if(i>iultimofd) iultimofd=i;

                stampa_arraygenerale(multiplexedfd,i,iultimofd);

                if(--nfdready<=0) continue;
    }



    for(i=1;i<=iultimofd;i++)   {
                if( multiplexedfd[i].fd <0)
                                continue;

                if(FD_ISSET(multiplexedfd[i].fd,&rset)) {
                        if( (r=full_read(multiplexedfd[i].fd,toreceive,300)) ==0) {
                                        //se il client non ha inviato ancora il primo messaggio da quando ha stabilito la connessione, tecnicamente per gli altri client
                                        //non si e' ancora connesso (gli altri non hann ovisualizzato il messaggio del suo ingresso, e sul server non e' ancora stato salvato il suo nick e la sua stanza -in quanto questi arrivano col primo messaggio vero
                                        //In questo caso, se un client quindi si disconnette senza aver inviato almeno il primo messaggio (quindi non abbiamo ne' il suo nick ne' la sua stanza)
                                        //a)NON lanciamo la funzione per avvisare tutti gli altri client della disconnessione di quell'utente (sia perche' non hanno mai neanche visualizzato il messaggio del suo ingresso, sia perche' non sappiamo a che stanza girarlo)
                                        //b)lanciamo lo stesso la funzione che salva il log di connessione (sarebbe inverosimile non registrare una connessione lato server solo perche' manca il nick e stanza)
                                             //ma in questo caso la funzione del log gestisce direttamente la cosa : pone nick e stanze come sconosciute all'interno del log file
                                        if(multiplexedfd[i].nickclient!=NULL)
                                                        toall_mexSRV_uscitautente(multiplexedfd,i,iultimofd);

                                        if(clientdisconnesso_salvalog(multiplexedfd,i)<0) {
                                                        printf("\n***LOG-CORROTTO!\n");
                                                        fflush(stdout);
                                        }

                                        close(multiplexedfd[i].fd);
                                        FD_CLR(multiplexedfd[i].fd,&set1);
                                        multiplexedfd[i].fd=-1;
                                        if(multiplexedfd[i].nickclient!=NULL) { /*potrebbe capitare che il client si connetta, non scriva niente, quindi non gli venga registrato il nick sull'array generale, e poi come primo messaggio invii quit.
                                                                                                        Quindi FIN, quindi va cancellato dall'array, ma il suo nome e' gia deallocato perche' non gli e' stato mai assegnato. Senza il controllo si avrebbe seg fault */
                                                free(multiplexedfd[i].nickclient);
                                                multiplexedfd[i].nickclient=NULL;
                                        }

                                        stampa_arraygenerale(multiplexedfd,i,iultimofd);

                        }

                        else if(r>0){

                                        if(multiplexedfd[i].nickclient==NULL) {
                                                multiplexedfd[i].nickclient=calloc(15,sizeof(char));
                                                strncpy(multiplexedfd[i].nickclient,toreceive,14);
                                                multiplexedfd[i].nickclient[14]='\0';
                                                multiplexedfd[i].stanza=toreceive[14];

                                               toall_mexSRV_nuovoutente(multiplexedfd,i,iultimofd);

                                               if(clientconnesso_getinfo_salvalog(multiplexedfd,i)<0) {
                                                        printf("\n***LOG CORROTTO!");
                                                         fflush(stdout);
                                              }


                                        stampa_arraygenerale(multiplexedfd,i,iultimofd);

                                        }

                                        for(j=1;j<=iultimofd;j++)
                                                if(multiplexedfd[j].fd!=multiplexedfd[i].fd && multiplexedfd[j].fd>=0 && multiplexedfd[j].stanza==multiplexedfd[i].stanza) {

                                                                do w=write(multiplexedfd[j].fd,toreceive,300);
                                                                        while(w<0 && errno==EINTR);

                                                                if(w<=0) {perror("\nIMPOSSIBILE GIRARE UN MESSAGGIO AD UNO DEI CLIENT"); continue;
                                                                }
                                                }

                        }

                        else {perror("\nREAD:"); continue;}


                        if(--nfdready<=0) break;
            }
    } //chiusura for interno

} //chiusura for esterno

} //chiusura main



ssize_t full_read(int fd,void *vptr,size_t n) {
size_t nleft;
ssize_t nread;
char *ptr;

ptr=vptr;
nleft=n;
while(nleft>0) {
    if( (nread=read(fd,ptr,nleft)) <0) {
        if (errno==EINTR)
            nread=0;
        else
            return (-1);


    }
   else if (nread==0) //per arrivare qui, deve essere la read ad avere restituito 0, se invece lo poniamo noi nread==0 per l'EINTR, non arriva qui perche' c'e' else
                    break;
    nleft-= nread;
    ptr += nread;

}
return (n-nleft);

}


void toall_mexSRV_nuovoutente(struct fdmultiplexati multiplexedfd[],int i_nuovoclient,int iultimofd)    {
int j,w;

char tosend[300]="**************";
strcat(tosend," IL CLIENT ");
strncat(tosend,multiplexedfd[i_nuovoclient].nickclient,strlen(multiplexedfd[i_nuovoclient].nickclient));
strcat(tosend," SI E' CONNESSO ALLA STANZA DI CHAT");


for(j=1;j<=iultimofd;j++) {

            if(j==i_nuovoclient) continue;

            if(multiplexedfd[j].fd>=0 && multiplexedfd[j].stanza==multiplexedfd[i_nuovoclient].stanza){
                       do w=write(multiplexedfd[j].fd,tosend,300);
                                while(w<0 && errno==EINTR);

                        if(w<=0) {perror("\nIMPOSSIBILE GIRARE UN MESSAGGIO SRV AD UN CLIENT");
                                          continue;
                        }
            }

}
}


void toall_mexSRV_uscitautente(struct fdmultiplexati multiplexedfd[],int i_uscente,int iultimofd)  {


int j,w;
char tosend[300]="**************";

strcat(tosend," IL CLIENT ");
strncat(tosend,multiplexedfd[i_uscente].nickclient,strlen(multiplexedfd[i_uscente].nickclient));
strcat(tosend," SI E' DISCONNESSO DALLA SESSIONE DI CHAT!");


for(j=1;j<=iultimofd;j++){

            if(j==i_uscente) continue;
            if(multiplexedfd[j].fd>=0 && multiplexedfd[j].stanza==multiplexedfd[i_uscente].stanza ){
                       do w=write(multiplexedfd[j].fd,tosend,300);
                                while(w<0 && errno==EINTR);
                        if(w<=0) {
                            perror("\nIMPOSSIBILE GIRARE UN MESSAGGIO SRV AD UN CLIENT");
                            continue;
                        }
            }


}
}

int clientdisconnesso_salvalog(struct fdmultiplexati multiplexedfd[],int i_uscente) {


FILE *pfile;
char percorso[FILENAME_MAX],nickname[15],stanza_lasciata;
time_t datacodif;
struct tm *dataleggibile;

time(&datacodif);
dataleggibile=localtime(&datacodif);
getcwd(percorso,sizeof(percorso));

strcat(percorso,"/");
strcat(percorso,"logfile");
strcat(percorso,".txt");

//nel caso specifico in cui un client si disconnette senza aver mandato neanche un messaggio il log file salva i suoi dati senza la stanza e il nick (che non avendo ricevuto
//almeno un messaggio da quel client, non conosciamo .
if(multiplexedfd[i_uscente].nickclient!=NULL) {
                strncpy(nickname,multiplexedfd[i_uscente].nickclient,15);
                stanza_lasciata=multiplexedfd[i_uscente].stanza;
}

else {
                strcpy(nickname,"-unknown-");
                stanza_lasciata='?';
}
//ora qualunque sia il caso, in stanza_lasciata e nickname ci sono valori stampabili

if((pfile=fopen(percorso,"a"))==NULL)
    return -1; //non riesce ad aprire il file
else{
        if(stanza_lasciata=='?') { //stampa un messaggio sul log che spieghi perche' appare un client con nome unknown che si e' disconnesso, ma non appare il momento in cui si collega
                    if(fprintf(pfile,"\n\n//%d:%d:%d %d-%d-%d// nick:%s (stanza %c) \n >>>Il client ha effettuato connessione al server, ma ha scelto di disconnettersi prima di inviare almeno 1 messaggio\n >>>Nick e stanza scelta dal client non pervenuti!",dataleggibile->tm_sec,dataleggibile->tm_min,dataleggibile->tm_hour,
                    dataleggibile->tm_year,dataleggibile->tm_mon,dataleggibile->tm_mday,nickname,stanza_lasciata)<0)
                        return -1;
        }



        else if(fprintf(pfile,"\n\n//%d:%d:%d %d-%d-%d// l'utente %s  si e' disconnesso (da stanza %c) ",dataleggibile->tm_sec,dataleggibile->tm_min,dataleggibile->tm_hour,
                    dataleggibile->tm_year,dataleggibile->tm_mon,dataleggibile->tm_mday,nickname,stanza_lasciata)<0)
                    return -1; //non riesce a salvare il log
        }
        fflush(pfile);
        fclose(pfile);
        printf("\n***LOG DISCONNESSIONE UTENTE (A SEGUITO DI FIN) SALVATO\n");
        fflush(stdout);
        return 0;
}




int clientconnesso_getinfo_salvalog(struct fdmultiplexati multiplexedfd[],int i_nuovoclient) {
time_t datacodif;
struct tm *dataleggibile;
FILE *pfile;
char percorso[FILENAME_MAX],ipclient[INET_ADDRSTRLEN];
int portaclient;
struct sockaddr_in infoclient;
socklen_t sizestruct=sizeof(infoclient);

time(&datacodif);
dataleggibile=localtime(&datacodif);

getpeername(multiplexedfd[i_nuovoclient].fd,(struct sockaddr *)&infoclient,&sizestruct);
inet_ntop(AF_INET,&infoclient.sin_addr,ipclient,sizeof(ipclient));
portaclient=ntohs(infoclient.sin_port);

getcwd(percorso,sizeof(percorso));
strcat(percorso,"/");
strcat(percorso,"logfile");
strcat(percorso,".txt");
if((pfile=fopen(percorso,"a"))==NULL)
    return -1;



else{

        if(fprintf(pfile,"\n//%d:%d:%d %d-%d-%d// l'utente %s (fd:%d  ip:%s  porta:%d) si e' connesso (stanza %c)",dataleggibile->tm_sec,dataleggibile->tm_min,dataleggibile->tm_hour,
                   dataleggibile->tm_year,dataleggibile->tm_mon,dataleggibile->tm_mday,multiplexedfd[i_nuovoclient].nickclient,multiplexedfd[i_nuovoclient].fd,ipclient,portaclient,multiplexedfd[i_nuovoclient].stanza)<0)

        fflush(pfile);
        fclose(pfile);
        printf("\n***LOG CONNESSIONE UTENTE (A SEGUITO DI PRIMO MESSAGGIO) SALVATO");
        fflush(stdout);
        return 0;

}

}




void stampa_arraygenerale(struct fdmultiplexati multiplexedfd[],int i,int iultimofd) {
int j;
printf("\nARRAY CONTENENTE LE CORRISPONDENZE TRA FDSOCKET, NICK CLIENT E STANZE (la freccia indica l'ultima entry):\n");
for(j=0;j<=iultimofd;j++)
    {if(j==0) printf("listening socket: ");
      if(j==i) printf("----> ");
      printf("fd:%d   nick %s       stanza:%c\n",multiplexedfd[j].fd,multiplexedfd[j].nickclient,multiplexedfd[j].stanza);
      fflush(stdout);

    }






}

