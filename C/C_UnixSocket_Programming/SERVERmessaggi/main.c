#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <netdb.h>
#include <string.h>

struct array_generale{
        int fd;

} multiplexedfd[FD_SETSIZE];

struct corrispondenze{
        char nome[20];
        char ip[INET_ADDRSTRLEN];
} corrispondenze_inmemoria[300];

int main() {
int pid;
char *towrite,*toread,buffer[100];
int connsfd;
char percorso[FILENAME_MAX];
FILE *pfile,*pfile2;
fd_set set1;
int nostraportatcp,nostraportaudp,ret;
struct sockaddr_in tcp_nostrind,udp_nostrind;
getcwd(percorso,FILENAME_MAX);
strcat(percorso,"/corrispondenze.txt\0");
int i,j,k;
for(i=0;i<FD_SETSIZE;i++)
    multiplexedfd[i].fd=-1;

printf("\nInserire nostra porta tcp\n");
scanf("%d",&nostraportatcp);
printf("\nInserire nostra porta udp\n");
scanf("%d",&nostraportaudp);
while(getchar()!='\n');

if((multiplexedfd[1].fd=socket(AF_INET,SOCK_STREAM,0))<0){
        perror("imp creare listening socket per tcp");
        exit(1);
}

tcp_nostrind.sin_family=AF_INET;
tcp_nostrind.sin_port=htons(nostraportatcp);
tcp_nostrind.sin_addr.s_addr=htons(INADDR_ANY);

if( bind(multiplexedfd[1].fd,(struct sockaddr *)&tcp_nostrind,sizeof(tcp_nostrind))<0 ||  listen(multiplexedfd[1].fd,1024)<0 ){
        perror(""); exit(1);
}

udp_nostrind.sin_family=AF_INET;
udp_nostrind.sin_port=htons(nostraportaudp);
udp_nostrind.sin_addr.s_addr=htons(INADDR_ANY);

if((multiplexedfd[0].fd=socket(AF_INET,SOCK_DGRAM,0))<0 || bind(multiplexedfd[0].fd,(struct sockaddr *)&udp_nostrind,sizeof(udp_nostrind))<0){
        perror(""); exit(1);
}

if((pfile2=fopen(percorso,"r"))==NULL){
        perror("errore su pfile2\n"); exit(1);

}
//carichiamo in memoria il file delle corrispondenze nell'array di struct
k=0;
while(strncmp(fgets(buffer,100,pfile2),"end",3)!=0){
        for(i=0;i<100;i++)
                if(buffer[i]==':') break;
        strncpy(corrispondenze_inmemoria[k].nome,buffer,i);
        corrispondenze_inmemoria[k].nome[i]='\0';

        for(j=i+1;j<100;j++)
                if(buffer[j]==':') break;

        strncpy(corrispondenze_inmemoria[k].ip,buffer+i+1,j-i-1);
        corrispondenze_inmemoria[k].ip[j-i]='\0';


                k++;
}



int maxfd,nfdready;
if(multiplexedfd[0].fd>multiplexedfd[1].fd)
        maxfd=multiplexedfd[0].fd;
else maxfd=multiplexedfd[1].fd;
for(;;) {
    for(i=0;i<FD_SETSIZE;i++)
        if(multiplexedfd[i].fd!=-1) FD_SET(multiplexedfd[i].fd,&set1);
    nfdready=select(maxfd+1,&set1,NULL,NULL,NULL);

    if(FD_ISSET(multiplexedfd[0].fd,&set1)){
                    ///udp ready
                    if(nfdready--<=0) continue;
    }

    if(FD_ISSET(multiplexedfd[1].fd,&set1)){
            connsfd=accept(multiplexedfd[1].fd,NULL,NULL);
            if((pid=fork())==0){
                     printf("\nAvvio processo figlio per trasferimento file corrispondenze\n"); fflush(stdout);
                     close(multiplexedfd[1].fd);
                     if((pfile=fopen(percorso,"r"))==NULL) {
                                printf("\nImpossibile aprire il file corrispondenze\n");
                                exit(1);
                    }
                    towrite=calloc(300,sizeof(char));

                    do {fgets(towrite,40,pfile);
                            write(connsfd,towrite,strlen(towrite));
                    }
                        while(strncmp(towrite,"end",3)!=0);



                    printf("\nfiglio: trasferimento completato\n"); fflush(stdout);
                    fclose(pfile);
                    close(connsfd);
                    exit(0);
            }
            else close(connsfd);
            if(nfdready--<=0) continue;
    }


}




}
