#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/select.h>


int log_in(int connsfd);
int crea_account(int connsfd);

int main() {
int i,j,ret,connsfd,portasrv,risp;
struct sockaddr_in servind;
char serverip[INET_ADDRSTRLEN];

printf("\nInserire porta del server\n");
scanf("%d",&portasrv);
while(getchar()!='\n');
printf("\nInserire ip del server\n");
fgets(serverip,INET_ADDRSTRLEN,stdin);
serverip[strlen(serverip)-1]='\0';

servind.sin_family=AF_INET;
servind.sin_port=htons(portasrv);
inet_pton(AF_INET,serverip,&servind.sin_addr);

if((connsfd=socket(AF_INET,SOCK_STREAM,0))<0){
                        perror("\nImpossibile creare socket di connessione:");
                        exit(1);
                }

//printf("vogliamo connetterci a %s",inet_ntoa(servind.sin_addr));


//siamo connessi al server, dobbiamo verificare nome e pw oppure creare un nuovo account
for(;;){

                do ret=connect(connsfd,(struct sockaddr *)&servind,sizeof(servind)); //qui scatta la select lato server, e gli viene impostato stato 0
                        while(ret<0 && errno==EINTR);
                if(ret<0){

                    if(errno==ECONNREFUSED) printf("\nIl server non ha un'applicazione in ascolto per il gioco\n");
                    else if(errno==ETIMEDOUT || errno==ENETUNREACH) printf("\nIl server non risulta raggiungibile ");
                    perror("");
                    exit(1);
                }



                for(;;){
                        printf("\nSEI CONNESSO AL SERVER (MA NON SEI LOGGATO)\n[1]Log-in con account gia' esistente\n[2]Crea nuovo account\n[0]Exit\n");
                        fflush(stdout);
                        scanf("%d",&risp);
                        if(risp==1 || risp==2 || risp==0) break;
                        else printf("\ncar non valido\nriprova\n");
                }

                if(risp==1)
                            ret=log_in(connsfd);
                else if(risp==2)
                            ret=crea_account(connsfd);
                else {
                            close(connsfd);
                            break;
                }

                if(ret==1) {//qui lanciamo la sessione per giocare xke' tutto e' andato bene
                                printf("gioca()");
                                close(connsfd);
                                break;

                }


                else if(ret<0) {
                    printf("\nErrore durante le comunicazioni per nick/pw oppure stringa riservata non riconosciuta - interruzione programma");
                    fflush(stdout);
                    close(connsfd);
                    break;

                }

                else if(ret==0) { printf("\nRiprova\n");
                                            fflush(stdout);
                                             if((connsfd=socket(AF_INET,SOCK_STREAM,0))<0){
                                                                perror("\nImpossibile creare socket di connessione:");
                                                                exit(1);
                                            }
                                            continue;

                }

}
printf("\nProgramma terminato\n");
fflush(stdout);

}



int  crea_account(int connsfd) {
//invia per prima cosa una stringa riservata al server per dirgli che si sta creando un nuovo account che deve essere salvato sul file del server
char *nome,*pw,*toread,*towrite;
int ret;
towrite=calloc(300,sizeof(char));
strcpy(towrite,"new_account\0"); //scatta la select sul server: gli imposta stato 1

do ret=write(connsfd,towrite,strlen(towrite)+1);
    while(ret<0 && errno==EINTR);
if(ret<=0){
        perror("\nErrore sulla write\n");
        return -1;
}
while(getchar()!='\n');

free(towrite);
towrite=calloc(300,sizeof(char));
printf("\nInserire nome utente (nuovo): "); fflush(stdout);
nome=calloc(20,sizeof(char));
fgets(nome,20,stdin);
nome[strlen(nome)-1]=':';

printf("\nInserire password(nuova): "); fflush(stdout);
pw=calloc(20,sizeof(char));
fgets(pw,20,stdin);
pw[strlen(pw)-1]='\0';


strncpy(towrite,nome,strlen(nome));
towrite[19]='\0';
strcat(towrite,pw);
towrite[strlen(towrite)]='\0';

printf("\nInviamo dati...(attendi)\n");
printf("%s",towrite);
fflush(stdout);


do ret=write(connsfd,towrite,strlen(towrite)+1);
        while(ret<0 && errno==EINTR);
if(ret<=0){
        perror("");
        close(connsfd);
        return -1;
}

printf("\Dati inviati (%d char). In attesa di conferma da parte del server per la creazione account-> %s\n",ret,towrite);
fflush(stdout);

toread=calloc(300,sizeof(char));
//abbiamo inviato nick e pw, ora aspettiamo la conferma
do ret=read(connsfd,toread,300);
    while(ret<0 && errno==EINTR);
if(ret<=0){
        perror("Errore nella lettura della conferma\n");
        return -1;
}

if(strncmp(toread,"ok",strlen("ok"))==0){
    printf("\nConferma ricevuta: account creato\n");
    return 1;
}

else if(strncmp(toread,"invalido",strlen("invalido"))==0){
    printf("\nAccount gia' esistente\n");
    return 0;

}

else return -1;




}



int log_in(int connsfd){
char *nome,*pw,*toread,*towrite;
int ret;
towrite=calloc(300,sizeof(char));
strcpy(towrite,"log_account\0");

do ret=write(connsfd,towrite,strlen(towrite)+1); //scatta la select sul server: gli imposta stato 2
    while(ret<0 && errno==EINTR);
if(ret<=0){
        perror("\nErrore sulla write\n");
        return -1;
}
while(getchar()!='\n');
free(towrite);
towrite=calloc(300,sizeof(char));
printf("\nInserire nome utente (esistente): "); fflush(stdout);
nome=calloc(20,sizeof(char));
fgets(nome,20,stdin);
nome[strlen(nome)-1]=':';
printf("\nInserire password(esistente): "); fflush(stdout);
pw=calloc(20,sizeof(char));
fgets(pw,20,stdin);

pw[strlen(pw)-1]='\0';


strncpy(towrite,nome,strlen(nome));
towrite[19]='\0';
strcat(towrite,pw);
towrite[strlen(towrite)]='\0';

printf("\nInviamo dati...(attendi)\n");
printf("%s",towrite);
fflush(stdout);



do ret=write(connsfd,towrite,strlen(towrite));
        while(ret<0 && errno==EINTR);
if(ret<=0){
        perror("");
        close(connsfd);
        return -1;
}

//abbiamo inviato nick e pw, ora aspettiamo la conferma del server che controllando sul file, ci dica che esiste quel nick/pw
toread=calloc(300,sizeof(char));
do ret=read(connsfd,toread,300);
    while(ret<0 && errno==EINTR);
if(ret<=0){
        perror("Errore nella lettura della conferma\n");
        return -1;
}

if(strncmp(toread,"ok",strlen("ok"))==0){
    printf("\nConferma ricevuta: loggato con successo\n");
    return 1;
}

else if(strncmp(toread,"invalido",strlen("invalido"))==0){
    printf("\nAccount/pw errati\n");
    return 0;
}

else return -1;









}

