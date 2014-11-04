#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/select.h>
#include <errno.h>


int sessione_server(int nostraporta);
int main() {


int nostraporta,ret;

printf("\nInserire porta su cui ricevere\n");
scanf("%d",&nostraporta);
while(getchar()!='\n');
printf("\nserver is starting...\n");
ret=sessione_server(nostraporta);
if(ret==1) printf("\nAbbiamo chiuso la sessione del server\n");
else if(ret==-1) printf("\nC'e' stato un errore: chiusa sessione server\n");
//non capita che il server vada giu' perche' un client chida la sua connessione, perche' noi forniamo il servizio a piu' client (multiplexati ognuno sul suo connsfd
                                                                                                                                 //con in piu' un unico listensfd)
return 0;
}

int sessione_server(int nostraporta){
FILE *pfile;
char nomefile[21],percorso[FILENAME_MAX];
int pid;

fd_set set1;
FD_ZERO(&set1);
struct timeval tempo1;
char tosend[300],toread[1000];
int multiplexedfd[FD_SETSIZE],ret,i,maxfd,j;
for(i=1;i<FD_SETSIZE;i++)
            multiplexedfd[i]=-1;
struct sockaddr_in nostrind;

nostrind.sin_port=htons(nostraporta);
nostrind.sin_family=AF_INET;
nostrind.sin_addr.s_addr=htons(INADDR_ANY);

if((multiplexedfd[0]=socket(AF_INET,SOCK_STREAM,0))<0) { //in indice 0 sull'array degli fd multiplexati, c'e' il listening socket
            printf("\nImpossibile aprire il socket di ascolto\n");
            return -1;
}

//bindiamo e mettiamo in ascolto il socket di connessione
if(bind(multiplexedfd[0],(struct sockaddr *)&nostrind,sizeof(nostrind))<0){
            printf("\nImpossibile bindare il socket sulla comb porta/ip scelti\n");
            return -1;
}

if(listen(multiplexedfd[0],1024)<0){
            printf("\nImpossibile mettere in ascolto il socket\n");
            return -1;
}



//non multiplexiamo pure la tastiera lato srv, visto che non ci nteressa
int nfdready;
maxfd=multiplexedfd[0]; //il max e' inizializzzato sul valore dell'unico fd presente nell'array, quello del listening socket

for(;;){
        printf("\nIn attesa di connessioni client/trasferimenti file\n");
        tempo1.tv_sec=5;
        for(i=0;i<FD_SETSIZE;i++)
                if(multiplexedfd[i]!=-1)
                        FD_SET(multiplexedfd[i],&set1);
        nfdready=select(maxfd+1,&set1,NULL,NULL,&tempo1);
        if(nfdready==0) continue;

        if(FD_ISSET(multiplexedfd[0],&set1)){ //il listn sock risulta ready=cioe' sulla coda dei client che hanno completato il 3wh (cioe' per i quali abbiamo ricevuto l'ultimo ack) ci sta almen oun client
                printf("\nUn client chiede di connettersi\n");
                fflush(stdout);
                //lo connettiamo,riceviamo la password, ma se e' errata lo disconnettiamo
                for(i=1;i<FD_SETSIZE;i++)
                    if(multiplexedfd[i]==-1) break;
                if(i==FD_SETSIZE){
                        printf("\nTroppi client connessi\n");
                        continue;
                }

                do multiplexedfd[i]=accept(multiplexedfd[0],NULL,NULL);
                    while(multiplexedfd[i]<0 && errno==EINTR);
                if(multiplexedfd[i]<0){
                        printf("\nImpossibile girare la connessione del client (3wh completato) sul connsfd relativo\n");
                        continue;
                }

                if(multiplexedfd[i]>maxfd) maxfd=multiplexedfd[i];
                printf("\nClient connesso: verifica password in corso\n");
                fflush(stdout);
                do ret=read(multiplexedfd[i],toread,300);
                    while(ret<0 && errno==EINTR);
                if(ret==0 || strncmp(toread,"rivaldo",strlen("rivaldo"))!=0) { //quidi quando un client si e' collegato, leggiamo subito dal socket. Se non leggiamo niente (ABBIAMO RICEVUTO FIN) oppure se quello che leggiamo non corrisponde alla password (rivaldo) allora lo disconnettiamo
                        printf("\nIl client si e' disconnesso prima di inserire la password/oppure la password non era corretta\n");
                        //quindi lo cancelliamo dall'array etc, prma gli inviamo pero' un messaggio che indica che la pw non e' valida, perche' il client cmq stara' in attesa della risposta, e se chiudiamo di botto da problemi
                        strcpy(tosend,"invalida");
                        do ret=write(multiplexedfd[i],tosend,300);
                                while(ret<0 && errno==EINTR);



                        close(multiplexedfd[i]);

                        if(maxfd==multiplexedfd[i]){
                                maxfd=multiplexedfd[0];
                                for(j=1;j<FD_SETSIZE;j++)
                                        if(multiplexedfd[j]>maxfd)
                                                maxfd=multiplexedfd[j];
                        }
                        multiplexedfd[i]=-1;

                }

                else printf("\nPassword corretta: Manteniamo la connessione del client\n");
                strcpy(tosend,"valida");
                do ret=write(multiplexedfd[i],tosend,300);
                    while(ret<0 && errno==EINTR);

                if(nfdready--<=0) continue;

        }


        for(i=1;i<FD_SETSIZE;i++)
                if(multiplexedfd[i]!=-1 && FD_ISSET(multiplexedfd[i],&set1)) {
                        printf("\nil socket %esimo e' pronto in lettura (dati sul socket)\n",i);
                        fflush(stdout);
                        //allora in tal caso e' pronto in lettura il socket di connessione di un client connessosi in precedenza
                        //quindi leggiamo
                        do ret=read(multiplexedfd[i],toread,21);
                            while(ret<0 && errno==EINTR);
                        if(ret<0) {
                                perror("\n");
                                continue;
                        }

                        else if(ret==0) { //abbiamo ricevuto FIN quindi dobbiamo cancellare quel client dalle nostre strutture dati xke' si e' disconnesso
                        printf("\nUn client si e' disconnesso\n"); fflush(stdout);
                        close(multiplexedfd[i]);

                        if(maxfd==multiplexedfd[i]){
                                maxfd=multiplexedfd[0];
                                for(j=1;j<FD_SETSIZE;j++)
                                        if(multiplexedfd[j]>maxfd)
                                                maxfd=multiplexedfd[j];
                        }
                        multiplexedfd[i]=-1;

                        }

                        else{
                        //quando il client ci invia qualcosa, vuol dire che ci sta trasferendo il file. I primi 20 char del testo che ci manda sono
                        //riservati al nome del file che ci sta mandando
                        //in questo caso, per evitare che il server debba ricevere un file alla volta da un solo client, non possiamo usare
                        //neanche la select, xke' comunque terrebbe occupato il processo per tutto il tempo in cui il socket risulta ready in lettura
                        //ovvero tutto il tempo della ricezione del file. Quindi dobbiamo usare per forza una fork
                        if((pid=fork())==0){ //allora siamo nel processo figlio, e chiudiamo tutto quello che e' superfluo (per prestazioni etc...)
                                                            //in realta', siccome stiamo duplicando un processo che ha comunque un array pieno di  fd, alcuni dei quali di conn aperte
                                                            //dovremmo chiudere tutti i socket dell'array, tranne quello dell'iesimo socket (iesimo client) che ci sta inviando qualcosa
                                        printf("\nprocesso figlion\n");
                                        fflush(stdout);

                                                //tiriamo fuori i primi 20 char ricevuti, che sono il nome del file che ci sta venendo inviato
                                                strncpy(nomefile,toread,20);
                                                //il client ci manda il file gia' con l'estensione aggiunta
                                                printf("\nIl client ci sta inviando il file di nome %s\n",nomefile);
                                                fflush(stdout);
                                                //creiamo il percorso nel quale salvarlo
                                                getcwd(percorso,sizeof(percorso));
                                                strcat(percorso,"/");
                                                strcat(percorso,nomefile);
                                                printf("\nSalviamo il file in:\n%s",percorso);
                                                fflush(stdout);

                                                pfile=fopen(percorso,"a");
                                                if(pfile==NULL) perror("\n");

                                                do ret=read(multiplexedfd[i],toread,1000);
                                                    while(ret<0 && errno==EINTR);
                                                if(ret==0) {
                                                        printf("\nIl client ha chiuso mentre durante la sessione di trasferimento\n");
                                                        /// come faccio ad eliminare quel client adesso dall'aray multiplexato nel processo padre, visto che si e' disconnesso adesso ?
                                                }
                                                if(ret<0) {perror("\n"); exit(1);}
                                                //trascriviamo sul file il contenuto del buffer di ricezione
                                                printf("\nbuffer ricezione:%s\n",toread);
                                                ret=fprintf(pfile,"%s",toread);
                                                printf("\nLa sessione ha ricevuto in tutto un file di %d caratter\ni",ret);
                                                fclose(pfile);
                                                exit(0);

                                        }

                        //nel padre comunque non dobbiamo chiudere neinte, perche' il processo figlio dura soltanto finche' va il trasferimento, poi si chiude,
                        //ma la connessione col client deve continuare (in realta' nel nostro esercizio il client manda solo 1 file alla volta, non c'e' un menu' per mandare piu' volte
                                                                        //quindi mandato un file, muore il figlio, quindi diminuisce di 1 il contatore del fd della connessione, e potremmo chiudere anche nel padre
                                                                        //in modo tale che la connessione vada via per quel client che ormai ha fatto tutto quello che poteva fare)
                        //NB: nel processo padre, il socket va chiuso, altrimenti nel proc padre, durante il trasferimento del file nel proc figlio, il sock comunque risultera' sempre ready
                        //e fara' partire n-processi figli sempre per lo stesso client e parti diverse dello stesso file
                        //in questo modo pero', una volta terminato il processo figlio, il socket verra' chiuso definitivamente e il client verra' disconnesso
                        //questo funziona in questo caso in cui un client su una connessione manda 1 file. Se avessimo voluto fare che una volta connesso, il client
                        //sulla stessa connect puo' mandare piu' di un file, avremmo magari dovuto creare un socket di connessione temporaneo solo lat ofiglio, kin modo tale che
                        // modo tale da spedire su questo il file, cosicche' lato processo padre, il socket con cui era stata creata la connessione, non risulti sempre ready

                        else //proc padre
                            close(multiplexedfd[i]); //quindi, nel processo padre, non verra' piu' fatta scattare la select per il socket che nel frattempo sta trasferendo file al figlio
                            //dovremmo poi gestire il modo in cui va tolto dall'array etc

                        }
                if(nfdready--<=0) break;

                }




}


}
