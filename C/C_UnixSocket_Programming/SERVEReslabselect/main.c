#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <string.h>
#include <errno.h>


void sessione_srv(int porta);
int main()
{
    int porta;
    printf("\nSu quale porta vogliamo ricevere ?\n");
    scanf("%d",&porta);
    while(getchar()!='\n');
    printf("\nAvvio del server...");
    sleep(2);
    sessione_srv(porta);
    return 0;
}

void sessione_srv(int porta) {
char *toread,*tosend;
int listensfd,i,nsocketready,r,w;
struct sockaddr_in nostrind;

if((listensfd=socket(AF_INET,SOCK_STREAM,0))<0) {
            printf("\nImpossibile aprire il socket di ascolto\n");
            exit(1);
}

nostrind.sin_port=htons(porta);
nostrind.sin_family=AF_INET;
nostrind.sin_addr.s_addr=htons(INADDR_ANY);

if(bind(listensfd,(struct sockaddr *)&nostrind,sizeof(nostrind))<0) {
        perror("\nImpossibile bindare il socket di ascolto:");
        exit(1);
}

if(listen(listensfd,1024)<0) {
        perror("Impossibile mettere in listening il socket:");
        exit(1);
}


int array_fdmultiplexed[FD_SETSIZE],maxfd=listensfd;
array_fdmultiplexed[0]=listensfd;
for(i=1;i<FD_SETSIZE;i++) {
            array_fdmultiplexed[i]=-1;
}

fd_set set1;


for(;;) {
    FD_ZERO(&set1);
    for(i=0;i<FD_SETSIZE;i++) {
            if(array_fdmultiplexed[i]!=-1)
                    FD_SET(array_fdmultiplexed[i],&set1);
    }

    nsocketready=select(maxfd+1,&set1,NULL,NULL,NULL);

    if(FD_ISSET(array_fdmultiplexed[0],&set1)){
            printf("\nUN NUOVO CLIENT E' PRONTO A COLLEGARSI\n");
            for(i=1;i<FD_SETSIZE;i++)
                    if(array_fdmultiplexed[i]==-1) break;
            if(i==FD_SETSIZE) printf("\nTroppi Client connessi, richiesta non processata!\n");
            else {
                array_fdmultiplexed[i]=accept(listensfd,NULL,NULL);
                if(array_fdmultiplexed[i]>maxfd) maxfd=array_fdmultiplexed[i];
                printf("\nCONNESSIONE COL CLIENT ACCETTATA\n");

            }
            if(nsocketready--<=0) continue;
    }

    for(i=1;i<FD_SETSIZE;i++)
        if(array_fdmultiplexed[i]!=-1 && FD_ISSET(array_fdmultiplexed[i],&set1)) {
                toread=calloc(300,sizeof(char));
                do r=read(array_fdmultiplexed[i],toread,300);
                    while(r<0 && errno==EINTR);
                if(r<0){ //non riusciam oa leggere correttamente
                    perror("Errore sulla read dal socket:");
                    free(toread);
                    exit(1);
                }
                if(r==0) { //leggiamo 0 = FIN ...//in realta' questo controllo e' valido solo per la full_read etc etc...
                    printf("\nRicevuto FIN dal client\n");

                    if(array_fdmultiplexed[i]==maxfd){
                    maxfd=array_fdmultiplexed[0];
                    int j;
                    for(j=1;j<FD_SETSIZE;j++)
                            if(array_fdmultiplexed[j]>maxfd) maxfd=array_fdmultiplexed[j];
                    }
                    array_fdmultiplexed[i]=-1;
                    printf("\nIl client si e' disconnesso\n");
                    free(toread);
                }
                else{ //leggiamo almeno 1 char
                int nchar=0,k=0;
                while(toread[k]!='\0'){
                            nchar++;
                            k++;
                }
                free(toread);
                tosend=calloc(300,sizeof(char));
                sprintf(tosend,"%d",nchar);
                do w=write(array_fdmultiplexed[i],tosend,300);
                    while(w<0 && errno==EINTR);
                if(w<=0){
                    perror("errore sulla write sul socket:");
                    free(tosend),
                    exit(1);
                }
                printf("\nla parola ricevuta dal client era lunga %d char\nRisposta inviata al client\n",nchar);
                free(tosend);




                }

          if(nsocketready--<=0) break;
        }

}

}
