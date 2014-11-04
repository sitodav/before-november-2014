#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
int main(int argc,char **argv)
{
char welcome[52]; char *giocatori_online[20];
int resultsearch,j=0;
int g=0,i=0,in=0;
char nickname[12],password[5];
int sockfd,l=0;
int logans=0;

struct sockaddr_in servaddr;
if (argc!=2)
{
fprintf(stderr,"usage:%s<IPaddress>\n",argv[0]);
exit(1);
}

if ((sockfd=socket(AF_INET,SOCK_STREAM,0))<0)
{
fprintf(stderr,"socket error\n"); exit(1);
}

servaddr.sin_family=AF_INET;
servaddr.sin_port=htons(141);
if (inet_pton(AF_INET,argv[1],&servaddr.sin_addr)<=0)
{
fprintf(stderr,"inet_pton error\n"); exit(1);
}

if(connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)
{
fprintf(stderr,"connect error\n"); exit(1);
}
//mi connetto e stampo il messasggio di benvenuto
printf("\n[1]Entra nel gioco\n[2]Esci\n "); scanf("%d",&in);
if (in==2) { close(sockfd); exit(1); }
if (in==1){
write(sockfd,&in,sizeof(int));

if ((read(sockfd,welcome,51))<0)
{
perror("read"); exit(1);
}
printf("\n\n%s",welcome); fflush(stdout);

//scelgo se loggare con un account gia' esistente oppure registrarne uno
while(l==0){
printf("\nScegliere se:\n[1]Registrare un nuovo account\n[2]Loggare con un account gia' esistente\n"); fflush(stdout);
scanf("%d",&logans); l++; if(logans!=1 && logans!=2) {printf("Errore nell'inserimento, digitare 1 o 2\n\n"); fflush(stdout); l=0;} else l=1;
}
l=0;

if((write(sockfd,&logans,sizeof(int)))<0)
{
perror("write"); exit(1);
}
//caso in cui si sceglie di registrarsi :
if(logans==1)
{
while(g==0){

printf("\nScegliere un nickname (max 12 char): "); scanf("%s",nickname); fflush(stdout);
printf("\nScegliere una password (max 5 char): "); scanf("%s",password); fflush(stdout);
g++;
if ((strlen(nickname)>12) || (strlen(password)>6))
{
printf("\nErrore nell'inserimento , il nickname o la password sono troppo lunghi\n\n"); fflush(stdout); g=0;
}
else g=1;
} g=0;

//passiamo le informazioni al server

if ((write(sockfd,nickname,13))<0)
{
perror("write"); exit(1);
}
if ((write(sockfd,password,6))<0)
{
perror("write"); exit(1);
}




giocatori_online[j]=calloc((strlen(nickname)),sizeof(char));
if ((read(sockfd,giocatori_online[j],1000))<0) { perror("read"); exit(1); }
printf("\n\nQuesto e' l'elenco dei giocatori connessi in questo momento :\n"); fflush(stdout);


puts(giocatori_online[j]);
j++;
}




else if(logans==2)
{
while(g==0){
printf("\nInserire il tuo nickname (max 12 char): "); scanf("%s",nickname); fflush(stdout);
printf("\nInserisci la password (max 5 char): "); scanf("%s",password); fflush(stdout);
g++;
if ((strlen(nickname)>12) || (strlen(password)>6))
{
printf("\nErrore nell'inserimento , il nickname o la password sono troppo lunghi\n\n"); fflush(stdout); g=0;
}
else g=1;
} g=0;


//passiamo le informazioni al server

if ((write(sockfd,nickname,13))<0)
{
perror("write"); exit(1);
}
if ((write(sockfd,password,6))<0)
{
perror("write"); exit(1);
}
if (read(sockfd,&resultsearch,sizeof(int))<0) { perror("read"); exit(1); }
if (resultsearch==0) printf("\n\nLogin avvenuto con successo\n\n");
else printf("\nUsername o password errati\n"); fflush(stdout);
fflush(stdout);




giocatori_online[j]=calloc((strlen(nickname)),sizeof(char));
if ((read(sockfd,giocatori_online[j],1000))<0) { perror("read"); exit(1); }
printf("\n\nQuesto e' l'elenco dei giocatori connessi in questo momento :\n"); fflush(stdout);


puts(giocatori_online[j]);
j++;


}

}
}
