#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <netdb.h>
#include <string.h>

ssize_t full_read(int fd,void *vptr,size_t n);
int sessione(char nome[],int udpsocket,struct sockaddr_in *servindudp);

int main() {
char *toread,*towrite,buffer[50],nome[20];
FILE *pfile;
char percorso[FILENAME_MAX];
getcwd(percorso,FILENAME_MAX);
strcat(percorso,"/corrispondenze.txt\0");
int portasrvudp,portasrvtcp; //la nostra tcp e udp non ci interessano
struct sockaddr_in nostrindudp,servindudp,servindtcp;
char nostroip_tcpeudp[INET_ADDRSTRLEN],serverip_tcpeudp[INET_ADDRSTRLEN];
int tcp_connsfd,udp_socketfd;

printf("\nConfigurazione...\n");fflush(stdout);


printf("\nIns porta tcp server per download file corrispondenze\n");
scanf("%d",&portasrvtcp); while(getchar()!='\n'); //porta tcp server
printf("\nIns ip server tcp e udp");
fgets(serverip_tcpeudp,INET_ADDRSTRLEN,stdin);//ip server tcp e udp
serverip_tcpeudp[strlen(serverip_tcpeudp)-1]='\0';
printf("\nIns porta udp server\n");
scanf("%d",&portasrvudp); while(getchar()!='\n'); //porta udp server

printf("\nscaricamento automatico file corrispondenze dal server...\n");fflush(stdout);

if((tcp_connsfd=socket(AF_INET,SOCK_STREAM,0))<0){
        perror("\nerrore creazione tcp_connsfd con socket()"); exit(1);
}

servindtcp.sin_family=AF_INET;
servindtcp.sin_port=htons(portasrvtcp);
if(inet_pton(AF_INET,serverip_tcpeudp,&servindtcp.sin_addr)<=0){
        printf("\nIndirizzo server errato\n");
}

if(connect(tcp_connsfd,(struct sockaddr *)&servindtcp,sizeof(servindtcp))<0){
        perror("\nimpossibile connettersi al server (tcp) per scaricare file di corrispondenze\n");
        exit(1);
}

if((pfile=fopen(percorso,"w"))==NULL){
    perror("\nimpossibile creare il file corrispondenze\n");
    exit(1);
}

int ret;
toread=calloc(300,sizeof(char));
ret=full_read(tcp_connsfd,toread,299);
fprintf(pfile,"%s",toread);
close(tcp_connsfd);
fclose(pfile);

if((pfile=fopen(percorso,"r"))==NULL){
        perror("\nerrore riapertura file\n");
        exit(1);
}

printf("\nlista utenti accessibili:\n");
while(strncmp(fgets(buffer,50,pfile),"end",3)!=0)
            printf("%s",buffer);
fclose(pfile);


printf("\nScegli il nome dell'account tra quelli in lista (accesso libero)\n"); fflush(stdout);
fgets(nome,20,stdin); nome[strlen(nome)-1]='\0';

if((pfile=fopen(percorso,"r"))==NULL){
        perror("\nerrore riapertura file\n");
        exit(1);
}
int h;
while(strncmp(fgets(buffer,50,pfile),"end",3)!=0)
        if(strncmp(buffer,nome,strlen(nome))==0){
                    printf("\nCorrispondenza trovata. Nome valido.");fflush(stdout);
                    for(h=0;h<50;h++)
                        if(buffer[h]==':') break;
                    printf("\nbindiamo come ip di uscita: %s",buffer+h+1); fflush(stdout);
                    strcpy(nostroip_tcpeudp,buffer+h+1);
                    nostroip_tcpeudp[strlen(nostroip_tcpeudp)-1]='\0';
                    break;
        }

if(strncmp(buffer,"end",3)==0)
    {
        printf("\nnome inserito invalido.\n");
        exit(1);
    }


if( (udp_socketfd=socket(AF_INET,SOCK_DGRAM,0))<0 ){
        perror("\nerrore creazione sock udp\n"); exit(1);
}

nostrindudp.sin_family=AF_INET;
//la nostra udp porta di uscita non ci interessa
//come ip prendiamo quello preso dal file corrispondenze
if(inet_pton(AF_INET,nostroip_tcpeudp,&nostrindudp.sin_addr)<=0){
        perror("\nerrore inet_pton per ind nostro udp"); exit(1);
}

if(bind(udp_socketfd,(struct sockaddr *)&nostrindudp,sizeof(nostrindudp))<0){
        perror("\nerrore bind ip di uscita sul socket udp");
        exit(1);
}


servindudp.sin_family=AF_INET;
servindudp.sin_port=htons(portasrvudp);
inet_pton(AF_INET,serverip_tcpeudp,&servindudp.sin_addr);
int res;
res=sessione(nome,udp_socketfd,&servindudp);



}


int sessione(char nome[],int udpsocket,struct sockaddr_in *servindudp){
printf("\nfunzione sessione\n");
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
   else if (nread==0) //per arrivare qui, deve essere la read ad avere restituito 0, se invece lo poniamo noi nread==0 per l'EINTR, non arriva qui perche' c'e' else
                    break;
    nleft-= nread;
    ptr += nread;

}
return (n-nleft);

}

