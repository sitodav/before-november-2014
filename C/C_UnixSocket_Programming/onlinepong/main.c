#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <netdb.h>
#include <termios.h>

#define NRIGHE 20
#define NCOLONNE 30
#define LIBERO 32
#define MURO_LAT 120
#define MURO_SUP 120
#define ASCENDENTE 1
#define DISCENDENTE 0
#define PALLINA '*'
#define PLAYER '-'
#define SX 'a'
#define DX 'd'
#define LUNGHEZZAPLAYER 8
#define LIVELLO 6 //piu' aumenta il livello, piu' e' facile
#define P1 1
#define P2 2

struct sockaddr_in nostrind,servind;
int xnoi,xpal,ypal,x_diviso_y,fase_to,xavversario,udpsocket,player_id; //fase_to e' VERSO quale id player sta andando la pallina
char campo[NRIGHE][NCOLONNE];                //globali

int sincronizzazione_udp(char nostroip[],char ip_avversario[],int portanostra,int portaavversario);
void gioco();
void inizializza_campo();
void setta_campo();
void setta_parametri_globali();
void aggiorna_pacchetto_parametri(char tosend[]);
void aggiorna_parametri_da_pacchetto(char toreceive[]);
void setta_avversario_nelcampo();


int main(){
char ip_avversario[INET_ADDRSTRLEN],nostroip[INET_ADDRSTRLEN];
int portanostra,portaavversario;
printf("\nInserire la nostra porta\n");
scanf("%d",&portanostra);
while(getchar()!='\n');
printf("\nInserire porta avversario\n");
scanf("%d",&portaavversario);
while(getchar()!='\n');

printf("\nInserire nostro ip\n");
fgets(nostroip,INET_ADDRSTRLEN,stdin);
printf("\nInserire ip avversario\n");
fgets(ip_avversario,INET_ADDRSTRLEN,stdin);
nostroip[strlen(nostroip)-1]='\0';
ip_avversario[strlen(ip_avversario)-1]='\0';



if(sincronizzazione_udp(nostroip,ip_avversario,portanostra,portaavversario)<=0){
        printf("\nImpossibile sincronizzarsi con l'avversario\n");
        exit(0);
}

else gioco();

}



int sincronizzazione_udp(char nostroip[],char ip_avversario[],int portanostra,int portaavversario){
//a seconda di chi la fa per primo, e chi per secondo, vengono assegnati gli identificativi P1 e P2, quindi ch ibatte etc
int ret;
fd_set set1; FD_ZERO(&set1);
struct timeval tempo; tempo.tv_sec=1;
char *tosend,*toreceive,*trash;

player_id=P2; //entrambi sono p2, ma piu' avanti viene cambiato a p1 quello che ha fatto la sincronizzazione per primo
if((udpsocket=socket(AF_INET,SOCK_DGRAM,0))<0){
        perror("");
        return -1;
}

nostrind.sin_family=AF_INET;
nostrind.sin_port=htons(portanostra);
inet_pton(AF_INET,nostroip,&nostrind.sin_addr);

if(bind(udpsocket,(struct sockaddr *)&nostrind,sizeof(nostrind))<0){
        perror("");
        return -1;
}

servind.sin_family=AF_INET;
servind.sin_port=htons(portaavversario);
inet_pton(AF_INET,ip_avversario,&servind.sin_addr);

tosend=calloc(10,sizeof(char));
toreceive=calloc(10,sizeof(char));

strcpy(tosend,"ready");
tosend[strlen(tosend)]='\0';

do ret=sendto(udpsocket,tosend,10,0,(struct sockaddr *)&servind,sizeof(servind));
    while(ret<0 && errno==EINTR);
if(ret<0){
    perror("");
    return -1;
}


toreceive=calloc(10,sizeof(char));
do ret=recvfrom(udpsocket,toreceive,10,0,NULL,NULL);
    while(ret<0 && errno==EINTR);
if(ret<0){
    perror("");
    return -1;
}

do ret=sendto(udpsocket,tosend,10,0,(struct sockaddr *)&servind,sizeof(servind));
    while(ret<0 && errno==EINTR);
if(ret<0){
    perror("");
    return -1;
}





FD_SET(udpsocket,&set1);
select(udpsocket+1,&set1,NULL,NULL,&tempo);
    if(FD_ISSET(udpsocket,&set1)){


            trash=calloc(10,sizeof(char));
            recvfrom(udpsocket,trash,10,0,NULL,NULL);
            if(strcmp(trash,"ready")==0) //allora abbiamo ricevuto un udp in piu', in quanto da protocollo siamo noi che abbiamo lanciato per primi la sincronizzazione
                    player_id=P1;
            //altrimenti in caso contrario, rimaniamo p2, poiche' il buffer risulta ready poiche' l'altro (quello che ha iniziato per primo la sincronizzazione, gia' ci ha inviato dati di gioco, ecco xke' risulta ready il socket in lettura)
            free(trash);
            sleep(2);
    }

if(strcmp(toreceive,tosend)==0)
    return 1;
else return 0;

}

void inizializza_campo(){
int i,j;
for(i=0;i<NRIGHE;i++)
    for(j=0;j<NCOLONNE;j++)
        campo[i][j]=LIBERO;

}

void setta_campo(){
int i;
for(i=0;i<NRIGHE;i++){
        campo[i][0]=MURO_LAT;
        campo[i][NCOLONNE-1]=MURO_LAT;
}
campo[ypal][xpal]=PALLINA;

}

void setta_parametri_globali(){
int j;
xnoi=(NCOLONNE-1-LUNGHEZZAPLAYER)/2;
x_diviso_y=0;
fase_to=P2; //vuol dire che batte sempre P1, quindi la pallina va verso P2
xpal=(NCOLONNE-1)/2;
if(player_id==P1) //siamo il player 1 quindi la pallina sta nella nostra meta' campo
        ypal=NRIGHE-2;


else
       ypal=1;

for(j=0i;j<LUNGHEZZAPLAYER;j++)
    campo[NRIGHE-1][xnoi+j]=PLAYER;

}

void stampa_campo(){
int i,j;
for(i=0;i<NRIGHE;i++){
    for(j=0;j<NCOLONNE;j++){
            printf("%c",campo[i][j]);
            fflush(stdout);
    }
    printf("\n");
}




}

void aggiorna_pacchetto_parametri(char tosend[]){
tosend[0]=ypal;
tosend[1]=xpal;
tosend[2]=fase_to;
tosend[3]=x_diviso_y;
tosend[4]=xnoi;

}

void aggiorna_parametri_da_pacchetto(char toreceive[]){

if(player_id==P2){
    ypal=NRIGHE-1-toreceive[0];
    xpal=NCOLONNE-1-toreceive[1];
}
fase_to=toreceive[2];
x_diviso_y=-1*toreceive[3];
xavversario=NCOLONNE-1-toreceive[4]-LUNGHEZZAPLAYER;

}

void setta_avversario_nelcampo(){


void manifesta_parametri(){
int j;
for(j=0;j<LUNGHEZZAPLAYER;j++)
    campo[0][xavversario+j]=PLAYER;





}

void gioco(){ //p1 incrementa ad ogni giro la posizione della pallina, mentre p2 si limita a prenderne le coordinate attraverso il socket
fd_set set1; FD_ZERO(&set1);
struct timeval tempo;
char *tosend,*toreceive;
int ret,logic_mov_pallina=0;
//
struct termios oldt,newt;

  tcgetattr( STDIN_FILENO, &oldt );
  newt = oldt;
  newt.c_lflag &= ~( ICANON | ECHO );
  tcsetattr( STDIN_FILENO, TCSANOW, &newt );
//

inizializza_campo();
setta_parametri_globali();
setta_campo();
stampa_campo();
tosend=calloc(20,sizeof(char));
toreceive=calloc(20,sizeof(char));


while(1){
logic_mov_pallina++;

FD_SET(fileno(stdin),&set1);
tempo.tv_usec=10000;
tempo.tv_sec=0.9;

aggiorna_pacchetto_parametri(tosend);
///

do ret=sendto(udpsocket,tosend,10,0,(struct sockaddr *)&servind,sizeof(servind));
    while(ret<0 && errno==EINTR);
if(ret<0){
    perror("\n");
    exit(1);
}


do ret=recvfrom(udpsocket,toreceive,10,0,NULL,NULL);
    while(ret<0 && errno==EINTR);
if(ret<0){
    perror("\n");
    exit(1);
}

aggiorna_parametri_da_pacchetto(toreceive);

manifesta_parametri();
stampa_campo();

//printf("\n>%d %d %d %d %d id_%d",ypal,xpal,fase_to,x_diviso_y,xnoi,player_id);
select(fileno(stdin)+1,&set1,NULL,NULL,&tempo);
system("clear");

///



 if(logic_mov_pallina%LIVELLO==0) // ogni LIVELLO giri di select si muove la pallina
            movimento_pallina();


}




}
