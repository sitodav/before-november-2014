#include <sys/select.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
int main(int argc, char **argv)
{
char welcome[50]=" ***BENVENUTO A BATTAGLIA NAVALE*** \n\n\n";
FILE *users;
char *lista; char *giocatoreonline;
int nb=0;
char utenti[20]="Utenti.txt";
char nickname[17],password[6];
int listenfd,connfd,logans=0,resultsearch=0,len=0,maxi=0,ready=0,sockfd,in=0;
int maxfd; fd_set set,rset;
int client[FD_SETSIZE],i=0;
struct sockaddr_in servaddr;
if((listenfd=socket(AF_INET,SOCK_STREAM,0))<0)
{
perror("listen"); exit(1);
}

servaddr.sin_family=AF_INET;
servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
servaddr.sin_port=htons(141);
if(bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)
{
perror("bind"); exit(1);
}
if(listen(listenfd,1024)<0)
{
perror("listen"); exit(1);
}

maxfd=listenfd;
maxi=-1;
for(i=0;i<FD_SETSIZE;i++)
client[i]=-1;
FD_ZERO(&set);
FD_SET(listenfd,&set);
for(;;)
{
rset=set;
if((ready=select(maxfd+1,&rset,NULL,NULL,NULL))<0) { perror("select"); exit(1); }
if(FD_ISSET(listenfd,&rset)){
printf("\nUtente connesso\n"); fflush(stdout);

if((connfd=(accept(listenfd,(struct sockaddr*)NULL,NULL)))<0) { perror("accept"); exit(1); }
for(i=0;i<FD_SETSIZE;i++)
if (client[i]<0){
client[i]=connfd;
break;
}
if(i==FD_SETSIZE) { perror("troppiclient"); exit(1); }
FD_SET(connfd,&set);
if(connfd>maxfd) maxfd=connfd;
if(i>maxi) maxi=i;
if(--ready<=0) continue;
}

for(i=0;i<=maxi;i++)
{
if((sockfd=client[i])<0)
continue;
if(FD_ISSET(sockfd,&set))
{


read(sockfd,&in,sizeof(int));
if(in==1){
if(write(sockfd,welcome,51)<0) { perror("write"); exit(1); }
if (read(sockfd,&logans,sizeof(int))<0)
{
perror("read"); exit(1);
}
//ricorda di chiamare la fflush(stdout) dopo ogni printf o stampa in generale
//se il client ha scelto 1 allora implementiamo il caso della registrazione

if((read(sockfd,nickname,13))<0)
{
perror("read"); exit(1);
}
if((read(sockfd,password,6))<0)
{
perror("read"); exit(1);
}
len=strlen(nickname);
strcat(nickname," ");
strcat(nickname,password);
strcat(nickname,"\n");
fflush(stdout);
if (logans==1)
{
users=fopen("Utenti.txt","a+");
if(users == NULL)
{
printf("Can't open %s\n","Utenti.txt");
exit(0);
}
fputs(nickname,users);
fclose(users);
nickname[len]='\0'; //è un metodo per prelevare solo il nickname (dalla concatenazione precedente di nickname e password che serviva per il file) da mettere nella lista di utenti connessi

giocatoreonline=calloc(len,sizeof(char));
strcpy(giocatoreonline,nickname); //stiamo creando la lista sull'array 2D giocatori online.

if ((write(sockfd,giocatoreonline,len))<0) { perror("write\n"); exit(1); }

}

//L'UTENTE DECIDE DI LOGGARSI :
else if (logans==2)
{
users=fopen("Utenti.txt","a+");

if(users == NULL)
{
printf("Can't open %s\n","Utenti.txt");
exit(0);
}

fclose(users);
if((users=fopen("Utenti.txt","r"))<0) { perror("file"); exit(1); }
while(!feof(users))
{
lista=calloc(30,sizeof(char));
fgets(lista,30,users);
resultsearch=strcmp(lista,nickname);
if (resultsearch==0) break;
free(lista);
}
fclose(users);
if ((write(sockfd,&resultsearch,sizeof(int)))<0)
{
perror("write"); exit(1);
}

nickname[len]='\0';
giocatoreonline=calloc(len,sizeof(char));
strcpy(giocatoreonline,nickname); //stiamo creando la lista sull'array 2D giocatori online.

if ((write(sockfd,giocatoreonline,len))<0) { perror("write\n"); exit(1); }

}
close(sockfd);
FD_CLR(sockfd,&set);
client[i]=-1;
if(--ready<=0)
break;

} else if(in==2) { printf("\nUn utente si e' appena disconnesso\n"); fflush(stdout); close(sockfd); FD_CLR(sockfd,&set); client[i]=-1; }

}
}


}
}
