#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>


int main() {
int porta,udpsfd,ret;
char tosend[300]="",ipserver[INET_ADDRSTRLEN];
printf("\nIns porta server\n");
scanf("%d",&porta);
while(getchar()!='\n');
printf("\nIns ip server\n");
fgets(ipserver,INET_ADDRSTRLEN,stdin);
ipserver[strlen(ipserver)-1]='\0';

struct sockaddr_in servind;
servind.sin_port=htons(porta);
servind.sin_family=AF_INET;
inet_pton(AF_INET,ipserver,&servind.sin_addr);

struct hostent *ss;
ss=gethostbyaddr(&servind.sin_addr,sizeof(servind),AF_INET); //nb che la gethostbyaddr vuole in input al primo parametro la sin_addr che sta nella sockaddr_in, non solo la sockaddr_in!!!!
printf("\nprova %s",ss->h_name);




if((udpsfd=socket(AF_INET,SOCK_DGRAM,0))<0) {
    perror("\nImpossibile creare il socket udp:");
    exit(1);
}

struct sockaddr_in nostrind;
nostrind.sin_family=AF_INET;
nostrind.sin_port=htons(22); //hardcoded
char nostroip[]="127.0.0.1";
inet_pton(AF_INET,nostroip,&nostrind.sin_addr);
if(bind(udpsfd,(struct sockaddr *)&nostrind,sizeof(nostrind))<0){
perror("\nImpossibile bindare la porta 22:");
exit(1);
}

printf("\nIns parola da inviare\n");
fgets(tosend,300,stdin);
tosend[strlen(tosend)-1]='\0';


//il socket udp non e 'bindato, quindi ogni volta che lo usiam oper inviare qualcosa, potremmo uscire da una diversa porta /interfaccia ip

do ret=sendto(udpsfd,tosend,strlen(tosend)+1,0,(struct sockaddr *)&servind,sizeof(servind)); //gli possiamo passare soltanto il valore, tanto e' solo la recvfrom che modifica il valore della variabile size
    while(ret<0 && errno==EINTR);
if(ret<0){
    perror("\nErrore con la sendto() sul socket udp:");
    exit(1);
}

printf("\nInviata una parola di %d char\n",ret-1);



//struct per il riscrontro del destinatario
struct sockaddr_in mittind;
int lenstructmitt;


  int cont;
  char torec[300]="";
do ret=recvfrom(udpsfd,torec,300,0,(struct sockaddr *)&mittind,&lenstructmitt);
    while(ret<0 && errno==EINTR);
if(ret<0){
    perror("Errore lettura dal socket udp:");
    exit(1);
}


struct hostent *srv,*mittente;
srv=gethostbyaddr(&servind.sin_addr,sizeof(servind),AF_INET);
mittente=gethostbyaddr(&mittind.sin_addr,sizeof(mittind),AF_INET);

printf("\ndomain name srv: %s\n",srv->h_name);
printf("\ndomain name risposta %s\n",mittente->h_name);
printf("\nip 1%s",inet_ntoa(servind.sin_addr));
printf("\nip mitt%s",inet_ntoa(mittind.sin_addr));



if(lenstructmitt==sizeof(servind) && memcmp(&servind,&mittind,sizeof(servind.sin_addr))==0) {
    sscanf(torec,"%d",&cont);
    printf("\nIl server ci ha risposto %d\n",cont);
}
else printf("\nRicevuto messaggio da un altro peer\n");


return 0;
}
