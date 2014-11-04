
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


int salva_cronologia(char *dasalvare,char *nickname,char *percorsofinale);
ssize_t full_read(int fd,void *vptr,size_t n);
int chat(struct sockaddr_in *servind, struct sockaddr_in *nostrind,char *nickname,char *percorso);
int tcpipset(struct sockaddr_in *servind, struct sockaddr_in *nostrind,char *nostroip,char *serverip,char *nickname);

int main() {
int ris,chatvalue;
char percorso[FILENAME_MAX];
struct timeval tempo1;
int allset=0; //var logica bool
char nostroip[INET_ADDRSTRLEN]="",serverip[INET_ADDRSTRLEN]="",nickname[15]="";
struct sockaddr_in servind,nostrind;
printf("\t\t\aPROGRAMMA DI CHAT CLIENT-SERVER TCP/IP\n \n\t\t\t\t\t\t\tSito Davide");
do { //menu

                printf("\nCosa desideri fare ?\n");
                printf("[1]Entra nella chat\n[2]Modifica Impostazioni TCP/IP\n");
                printf("[0]Esci dal programma\n");
                scanf("%d",&ris);
                while(getchar()!='\n');

                if(ris!=1 && ris!=2 && ris!=0) {
                            printf("\n\aHai inserito un carattere di risposta non valido!\n");
                            continue;
                }

                if(ris==1){
                        if(!allset){
                                printf("\n\aDurante l'attuale esecuzione, non sono stati ancora definiti i parametri TCP/IP per la connessione (o quelli inseriti non sono validi)\nVerra' lanciata la configurazione...");
                                ris=2;
                                sleep(2);
                        }

                        else {
                                printf("\nTi stai connettendo ad una sessione di chat verso (%s-%d) con nickname %s\nScrivi quit per USCIRE\n",serverip,ntohs(servind.sin_port),nickname);
                                tempo1.tv_sec=2; tempo1.tv_usec=0; printf("\a");

                                select(NULL,NULL,NULL,NULL,&tempo1); //attendiamo qualche secondo

                                getcwd(percorso,sizeof(percorso));

                                chatvalue=chat(&servind,&nostrind,nickname,percorso);

                                if(chatvalue==-1){
                                        perror("\n>>PERROR:");
                                        printf("\nC'e' stato un errore di qualche tipo. Ricontrollare i parametri TCP/IP forniti, e controllare che il SERVER sia in esecuzione\n");
                                }
                                else if(chatvalue==0)
                                        printf("\nIl server ha interrotto la connessione\n");
                                else if(chatvalue==1)
                                        printf("\nAbbiamo interrotto la connessione\n");
                        }

                }

                if(ris==2)
                        allset=tcpipset(&servind,&nostrind,nostroip,serverip,nickname);


}

        while(ris!=0);

printf("\n\aPROGRAMMA TERMINATO\n");
return 0;


}


int chat(struct sockaddr_in *servind, struct sockaddr_in *nostrind,char *nickname,char *percorso) {
char cronologiatemp[1200]="\n********INIZIO SESSIONE********\n\0";
FILE *pfile;
int stanza;

char *tosend,*toreceive,*testo;
int connsfd,ret,keep4us=1,keep4srv=1;
int r,w;
char nickmittente[15]="";
fd_set set1;

strcat(percorso,"/");
strcat(percorso,nickname);
strcat(percorso,".txt");

do{printf("\nIn che stanza vuoi entrare(1-9) ?\n");
      scanf("%d",&stanza);
}
    while(stanza<=0 || stanza>=10);

struct timeval tempo2;
if((connsfd=socket(AF_INET,SOCK_STREAM,0))<0){
            printf("\nImpossibile creare il socket per la connessione\n"); return -1;}


if(bind(connsfd,(struct sockaddr *)nostrind,sizeof(*nostrind))<0) {
            printf("\nImpossibile assegnare al socket di connessione, IP e/o porta stabiliti. Ricontrollare le impostazioni!\n");
            if(errno==98) printf("\nOccorre un tempo minimo (TIME_WAIT) affinche' la porta bindata al precedente tentativo, venga davvero liberata e possa essere riutilizzata.\nCambia porta!\n");
            return -1;}

do ret=connect(connsfd,(struct sockaddr *)servind,sizeof(*servind));
            while(ret<0 && errno==EINTR);
if(ret<0) {printf("\nImpossibile portare a termine la connessione (3WH) col SERVER\n");
                 return -1;
}

printf("\nConnessione stabilita\n\n");
fflush(stdout);
printf("\nSEI NELLA STANZA %d\n",stanza);
fflush(stdout);
while(getchar()!='\n');

do{
                printf("\n***[[SCRIVI QUI]]:");
                fflush(stdout);
                FD_ZERO(&set1);
                tempo2.tv_sec=1500;
                FD_SET(fileno(stdin),&set1);
                FD_SET(connsfd,&set1);


                if(select(connsfd+1,&set1,NULL,NULL,&tempo2)==0) {
                                    printf("\n\aSIAMO STATI INATTIVI PER TROPPO TEMPO (1500s). AUTODISCONNESSIONE IN CORSO...\n");
                                    close(connsfd);
                                    keep4us=0;
                }



                if(FD_ISSET(connsfd,&set1)) {
                                    toreceive=calloc(300,sizeof(char));

                                    r=full_read(connsfd,toreceive,300);



                                    if(r<0) {printf("\nImpossibile leggere dal socket\n");
                                                  if(strlen(cronologiatemp)>0) // && if
                                                        if(salva_cronologia(cronologiatemp,nickname,percorso)<0)
                                                                printf("\nImpossibile salvare parte della cronologia");
                                                  free(toreceive);
                                                  return -1;
                                    }

                                    if(r==0) {keep4srv=0;
                                                    free(toreceive);
                                    }

                                    else {

                                                    if(strncmp("**************",toreceive,14)==0) {
                                                                printf("\n---SERVER:%s",toreceive+14);
                                                                fflush(stdout);
                                                    }

                                                    else{
                                                                strncpy(nickmittente,toreceive,14);
                                                                nickmittente[14]='\0';
                                                                printf("\n<<:%s \nscrive:>>%s",nickmittente,toreceive+15);
                                                                fflush(stdout);
                                                    }

                                                    if( (1200-strlen(cronologiatemp))<strlen(toreceive) )
                                                            if(salva_cronologia(cronologiatemp,nickname,percorso)<0)
                                                                    printf("\nImpossibile salvare parte della cronologia");
                                                    strcat(cronologiatemp,"\n");
                                                    strcat(cronologiatemp,toreceive);

                                    }

                                    if(keep4srv)
                                                    free(toreceive);
                }

                int f;
                if(FD_ISSET(fileno(stdin),&set1))  {

                                tosend=calloc(300,sizeof(char));
                                strncpy(tosend,nickname,15);
                                f=0;
                                while(f<15)
                                    {if(tosend[f]=='\n' || tosend[f]=='\0')
                                        tosend[f]=' ';
                                        f++;
                                    }
                                testo=calloc(285,sizeof(char));
                                fgets(testo,285,stdin);
                                tosend[14]=(char)(stanza+48);
                                tosend[15]='\0';
                                strcat(tosend,testo);

                                if(strcmp(tosend+15,"quit\n")==0) {
                                                printf("\nChiusura sessione chat in corso\n");
                                                free(tosend); free(testo); keep4us=0;
                                }

                                else{
                                                do w=write(connsfd,tosend,300);
                                                            while(w<0 && errno==EINTR);

                                                if(w<=0) {
                                                            printf("\nImpossibile scrivere sul socket\n"); perror("\nERRORE:\n");
                                                            if(strlen(cronologiatemp)>0) // && if
                                                                        if(salva_cronologia(cronologiatemp,nickname,percorso)<0)
                                                                                printf("\nImpossibile salvare parte della cronologia");
                                                            free(testo);
                                                            free(tosend);
                                                            return -1;
                                                }


                                                if( (1200-strlen(cronologiatemp))<strlen(tosend) )
                                                            if(salva_cronologia(cronologiatemp,nickname,percorso)<0)
                                                                            printf("\nImpossibile salvare parte della cronologia");

                                                strcat(cronologiatemp,"\n");
                                                strcat(cronologiatemp,tosend);



                                  }

                                if(keep4us)
                                            {free(testo); free(tosend);
                                }

                }


    }
        while(keep4us && keep4srv);



if(strlen(cronologiatemp)>0) // && if
        if(salva_cronologia(cronologiatemp,nickname,percorso)<0)
                    printf("\nImpossibile salvare parte della cronologia");



if(!keep4us) {close(connsfd); return 1;}
if(!keep4srv) {close(connsfd); return 0;}

}


int tcpipset(struct sockaddr_in *servind, struct sockaddr_in *nostrind,char *nostroip,char *serverip,char *nickname) {
int ris,nostraporta=-1,serverporta=-1;
do{ //menu
                printf("\nPremi:\n[1]Per definire le porte TCP\n[2]Per definire gli IP\n[3]Per il nickname\n[0]Per salvare le modifiche e uscire\n");
                scanf("%d",&ris);
                while(getchar()!='\n');

                if(ris==1) {
                        printf("\nInserisci la porta dalla quale \"uscire\": ");
                        scanf("%d",&nostraporta);
                        while(getchar()!='\n');
                        printf("\nInserisci la porta del server: ");
                        scanf("%d",&serverporta);
                        while(getchar()!='\n');
                }

                if(ris==2) {
                        printf("\nInserisci ip che ci identifica: ");
                        fgets(nostroip,INET_ADDRSTRLEN,stdin);
                        nostroip[strlen(nostroip)-1]='\0';
                        printf("\nInserisci ip del server: ");
                        fgets(serverip,INET_ADDRSTRLEN,stdin);
                        serverip[strlen(serverip)-1]='\0';
                }

                if(ris==3){
                        printf("\nChe nickname vuoi usare?: (max 13 char) ");
                        fgets(nickname,15,stdin);
                }

                if(ris==0){

                        nostrind->sin_family=AF_INET;
                        nostrind->sin_port=htons(nostraporta);
                        if(inet_pton(AF_INET,nostroip,&(nostrind->sin_addr))<1) {
                                    printf("\n\aSINTASSI NOSTRO IP NON VALIDA, MODIFICHE NON SALVATE\n");
                                    return 0;
                        }

                        servind->sin_family=AF_INET;
                        servind->sin_port=htons(serverporta);
                        if(inet_pton(AF_INET,serverip,&(servind->sin_addr))<1) {
                                    printf("\naSINTASSI SERVER IP NON VALIDA, MODIFICHE NON SALVATE\n");
                                    return 0;
                        }

                        if(strcmp(nickname,"")==0) {
                                    printf("\nNon e' stato deciso un nickname. Modifiche non salvate. Riprova\n");
                                    return 0;
                        }
                }

}
        while(ris!=0);

printf("\n\aMODIFICHE SALVATE CORRETTAMENTE!\n");
return 1;
}


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
   else if (nread==0)
                       break;
    nleft-= nread;
    ptr += nread;

}
return (n-nleft);

}


int salva_cronologia(char *dasalvare,char *nickname,char *percorsofinale) {


time_t datacodificata;
struct tm *data;
time(&datacodificata);
data=localtime(&datacodificata);

//printf("%s",nickname);
FILE *pfile;
if((pfile=fopen(percorsofinale,"a"))==NULL)
       {
          return -1;
       }

else{
        if(fprintf(pfile,"\n\n%d:%d:%d   %d/%d/%d\n",data->tm_sec,data->tm_min,data->tm_hour,data->tm_year,data->tm_mon,data->tm_mday)<0) return -1; //ogni volta che scarichiamo il buff temp della cronologia sul file, scriviamo anche prima la data
        if(fprintf(pfile,"%s",dasalvare)<0) return -1;
        fflush(pfile); // svuotamento del pnt a file prima di chiuderlo
        fclose(pfile);
        return 0;
}


}
