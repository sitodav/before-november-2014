#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/select.h>


void fai_loggare(int i);
void fai_creare(int i);
struct record_array{
        int fd;
        int stato;

        char nome[20];
} array_generale[FD_SETSIZE];

int main() {
int i,j,ret,nostraporta,maxfd,nfdready,pid;
char toread[300],towrite[300];
for(i=0;i<FD_SETSIZE;i++) {array_generale[i].fd=-1; array_generale[i].stato=-1;}
fd_set set1; FD_ZERO(&set1);
struct sockaddr_in nostrind;

printf("\nInserire porta su cui ricevere\n");
scanf("%d",&nostraporta);
while(getchar()!='\n');

nostrind.sin_family=AF_INET;
nostrind.sin_port=htons(nostraporta);
nostrind.sin_addr.s_addr=htons(INADDR_ANY);

if((array_generale[0].fd=socket(AF_INET,SOCK_STREAM,0))<0){
        perror("\nErrore creazione socket ascolto:");
        exit(1);
}

if(bind(array_generale[0].fd,(struct sockaddr *)&nostrind,sizeof(nostrind))<0){
        perror("\nErrore bind socket di ascolto\n");
        exit(1);
}

if(listen(array_generale[0].fd,1024)<0){
        perror("\nImpossibile mettere in ascolto il listen socket\n");
        exit(1);
}


maxfd=array_generale[0].fd;
for(;;) {


        for(i=0;i<FD_SETSIZE;i++)
                if(array_generale[i].fd!=-1) FD_SET(array_generale[i].fd,&set1);

        nfdready=select(maxfd+1,&set1,NULL,NULL,NULL); //non puo' andare in timeout


        if(FD_ISSET(array_generale[0].fd,&set1) && pid!=0 ) { //ready in lettura il listening socket
                    //printf("\nUn nuovo client e' pronto a connettersi\n"); fflush(stdout);
                    for(i=1;i<FD_SETSIZE;i++)
                            if(array_generale[i].fd==-1) break;
                    if(i==FD_SETSIZE) {printf("\nTroppi client attualmente connessi\n"); continue;}
                    if((array_generale[i].fd=accept(array_generale[0].fd,NULL,NULL))<0){
                            perror("\nErrore sull'accettazione client\n");
                            continue;
                    }
                   array_generale[i].stato=0;
                    if(array_generale[i].fd>maxfd) maxfd=array_generale[i].fd;
                    printf("\nTentativo di log-in: utente connesso ma non loggato...\n"); fflush(stdout);

        if(nfdready--<=0) continue;
        }

        for(i=1;i<FD_SETSIZE;i++)
                if(array_generale[i].fd!=-1 && FD_ISSET(array_generale[i].fd,&set1) && pid!=0){ //pronto in lettura uno dei client connessi
                            do ret=read(array_generale[i].fd,toread,300);
                                    while(ret<0 && errno==EINTR);
                             if(ret<0) {perror(""); continue;}
                             if(ret==0) {
                                              if(array_generale[i].stato==0) printf("\nSi e' disconnesso un client che non era ancora loggato\n");
                                              else if(array_generale[i].stato==3) printf("\nSi e' disconnesso un client loggato\n %s",array_generale[i].nome);

                                              if(array_generale[i].fd==maxfd){
                                                        maxfd=array_generale[0].fd;
                                                        for(j=1;j<FD_SETSIZE;j++)
                                                                if(array_generale[j].fd>maxfd) maxfd=array_generale[j].fd;
                                              }
                                              close(array_generale[i].fd);
                                              array_generale[i].fd=-1;
                                              array_generale[i].stato=-1;
                                              strcpy(array_generale[i].nome,"\0");



                            }
                            else if(ret>0){
                                            if(array_generale[i].stato==0){ //un client connesso, ma che non aveva verificato account, ci sta inviando la stringa riservata ch dice se vuole creare nuovo account o loggare
                                                    if(strncmp(toread,"new_account",strlen("new_account"))==0) array_generale[i].stato=1;
                                                    else if(strncmp(toread,"log_account",strlen("log_account"))==0) array_generale[i].stato=2;
                                            }
                                            if(array_generale[i].stato==1){ //unu client connesso, che aveva scelto di creare un nuovo account, ci sta inviando nome e pw
                                                               if((pid=fork())==0)
                                                                        {fai_creare(i);
                                                                         exit(0);
                                                                        }
                                            }

                                            if(array_generale[i].stato==2){ //un client connesso, che aveva scelto di loggare con account esistente, ci sta inviando nome e pw
                                                                if((pid=fork())==0)
                                                                        {fai_loggare(i);
                                                                          exit(0);
                                                                        }
                                            }

                                            if(array_generale[i].stato==3){ //un nuovo client loggato, che ha anche scelto nome e pw ci sta inviando una sfida

                                            }

                                            //il 4 non si puo' xke' e' solo in sessione in game
                            }
                if(nfdready--<=0) break;

                }


}


}


void fai_loggare(int i) {
printf("\nUn client ci sta inviando i dati per il login\n");
fflush(stdout);
char nome[20],pw[20],*toread,towrite[300],percorso[FILENAME_MAX],buffer[50],key[50];
FILE *pfile;
int ret,query;

toread=calloc(300,sizeof(char));
do ret=read(array_generale[i].fd,toread,300);
    while(ret<0 && errno==EINTR);
if(ret<0){
        perror("\nImpossibile leggere nick e pw del log in dal client,operazione annullata\n");
        return;
}

if(ret==0){
        printf("operazione creazione account annullata (FIN dal client)\n");
        return;

}
printf("\nRicevuti nick e pw (in tot %d char) dal client\n",ret); fflush(stdout);

int j;
for(j=0;j<strlen(toread);j++)
    if(toread[j]==':') break;

strcpy(pw,toread+j+1);
strncpy(nome,toread,j);
nome[strlen(nome)]='\0';


printf("\nun client ha chiesto di loggaret\nnome:%s --- pw:%s\n",nome,pw);
getcwd(percorso,FILENAME_MAX);
strcat(percorso,"/account.txt\0");
if((pfile=fopen(percorso,"r"))==NULL)
        {printf("\nImpossibile aprire il file con gli account\nOperazione annullata\n");
         return;
        }

query=0;
strcpy(key,toread);
printf("\n...CONTROLLO NOME/PW UTENTE:\n");
while(!feof(pfile)){
        fgets(buffer,50,pfile);
        printf("/////controllo:--> %s ?? %s",key,buffer); fflush(stdout);
        if(strncmp(key,buffer,strlen(buffer)-1)==0) {printf(" YES!\n "); query=1; break;}
        else printf(" NO!\n");
}



fclose(pfile);
if(query==1) //esiste corrispondenza
    strcpy(towrite,"ok\0");
else
    strcpy(towrite,"invalido\0");

do ret=write(array_generale[i].fd,towrite,strlen(towrite)+1); //inviata conferma
        while(ret<0 && errno==EINTR);
    if(ret<=0) {
        perror("problema nell'invio conferma al client, annulliamo l'operazione\n");
        return;
    }
    //ora salviamo nell'array globale e aggiorniamo lo stato
    if(query==1){
            array_generale[i].stato=3;
            strcpy(array_generale[i].nome,nome);
            printf("\nutente: %s ha loggato con successo\n",array_generale[i].nome);
    }
    else printf("\noperazione di log annullata: nome/pw errati");
    fflush(stdout);
    return;






}


void fai_creare(int i){
char nome[20],pw[20],toread[300],towrite[300],percorso[FILENAME_MAX],key[50];
FILE *pfile;
int ret;

printf("\nUn client sta creando un nuovo account(attendiamo che ci invii nick e pw)\n"); fflush(stdout);


do ret=read(array_generale[i].fd,toread,300);
    while(ret<0 && errno==EINTR);
if(ret<0){
        perror("Impossibile leggere nick e pw del nuovo account, da un client, operazione annullata\n");
        fflush(stdout);
        return;
}

if(ret==0){
        printf("operazione creazione account annullata (FIN dal client)\n");
        return;

}
printf("\nRicevuti nick e pw (in tot %d char) dal client\n",ret); fflush(stdout);

int j;
for(j=0;j<strlen(toread);j++)
    if(toread[j]==':') break;

strcpy(pw,toread+j+1);
strncpy(nome,toread,j);
nome[strlen(nome)]='\0';

printf("\nun client ha chiesto di creare account nome:%s --- pw:%s\n",nome,pw);
fflush(stdout);
getcwd(percorso,FILENAME_MAX);
strcat(percorso,"/account.txt\0");
if((pfile=fopen(percorso,"a"))==NULL)
        {printf("\nImpossibile aprire il file con gli account\nOperazione annullata\n");
         return;
        }
strcpy(key,toread);
fprintf(pfile,"%s\n",key); //nome:pw sul testo
fclose(pfile);
//diamo conferma al client
strcpy(towrite,"ok\0");
do ret=write(array_generale[i].fd,towrite,strlen(towrite)+1);
    while(ret<0 && errno==EINTR);
if(ret<0) {printf("\nproblema nell'inviare conferma\n"); return;}
//ora salviamo stato e nick dell'utente
array_generale[i].stato=3;
strcpy(array_generale[i].nome,nome);
printf("\nutente: %s ha creato account  e risulta loggato con successo\n",array_generale[i].nome);
fflush(stdout);
return;

}
