#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <errno.h>


int main() {
char tosend[300]="",toread[300]="",word[300],ipsrv[INET_ADDRSTRLEN]="";
int ret,connsfd_tcp;
struct sockaddr_in servind;
int porta;

printf("\ninserire la porta a cui connettersi\n");
scanf("%d",&porta);
while(getchar()!='\n');

printf("\nins ip server\n");
fgets(ipsrv,INET_ADDRSTRLEN,stdin);
ipsrv[strlen(ipsrv)-1]='\0';


servind.sin_port=htons(porta);
servind.sin_family=AF_INET;
inet_pton(AF_INET,ipsrv,&servind.sin_addr);

if((connsfd_tcp=socket(AF_INET,SOCK_STREAM,0))<0){
            printf("\nImpossibile aprire il socket\n");
            exit(1);

}

//iniziamo 3 way handshake
if(connect(connsfd_tcp,(struct sockaddr *)&servind,sizeof(servind))<0) {
            printf("\nImpossibile ottenere il syn-hack dal server\n");
            exit(1);
}
printf("\nConnessione stabilita (ricevuto syn-hack dal server, 3 way handshake lato client compltato\n");

printf("\nInserire la parola che vuoi che il server conteggi\n");
fgets(word,300,stdin);
word[strlen(word)-1]='\0';

do ret=write(connsfd_tcp,word,300);
     while(ret<0 && errno==EINTR);
if(ret<=0){
printf("\nImpossibile scrivere sul socket");
perror("");
exit(1);
}

printf("\nParola inviata al server\n");
do ret=read(connsfd_tcp,toread,300);
    while(ret<0 && errno==EINTR);
if(ret<0) {printf("\nErrore nella lettura dal socket\n"); exit(1);}
if(ret==0) {printf("\nRicevuto FIN dal server\n"); exit(1); } //in realta' questo contorllo e' valido solo per la full_read etc etc

int risultato;
sscanf(toread,"%d",&risultato);
printf("\nIl server ci dice che la parola inserita da tastiera era lunga:%d char\n",risultato);








}
