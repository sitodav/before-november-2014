#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <string.h>
#include <errno.h>
#include <time.h>
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


int fase=DISCENDENTE,x_pal=(NCOLONNE-1)/2,y_pal=1,x_diviso_y_pal=0,x1_pl=(NCOLONNE-3)/2;
char campo[NRIGHE][NCOLONNE];



void inizializza_campo_player();
void disegna_campo();
void inizializza_campo();
void stampa_campo();
void movimento_pallina();
void spostamento_player(int mov);
void rimbalzo_su_player(int mov,int player_on_move,int prec_player_on_move,int precprec_player_on_move);
void rimbalzo_su_muro_sup();
void rimbalzo_su_muro_lat();

int main()
{
fd_set set1; FD_ZERO(&set1);
char mov;
struct timeval tempo;
int precprec_player_on_move,prec_player_on_move,player_on_move=0;
int movepalla_logic=1,zona_pericolosa=0;

inizializza_campo();
disegna_campo();

//
struct termios oldt,newt;

  tcgetattr( STDIN_FILENO, &oldt );
  newt = oldt;
  newt.c_lflag &= ~( ICANON | ECHO );
  tcsetattr( STDIN_FILENO, TCSANOW, &newt );
//



while(1){
    movepalla_logic++;
    //
    precprec_player_on_move=prec_player_on_move;
    prec_player_on_move=player_on_move; //questo per sapere se anche alla precedente select il client si stava muovendo (per evitare che non si riesca a premere in tempo per dare il movimento alla pallina)
    //
    FD_SET(fileno(stdin),&set1);
    tempo.tv_usec=10000;
    tempo.tv_sec=0.9; /// 0.9
    stampa_campo();
    player_on_move=select(fileno(stdin)+1,&set1,NULL,NULL,&tempo); //se la select ritorna per il timeout il player nell'ultimo istante non si stava muovendo, altrimenti se ritorna 1, allora e' perche' lo stdin e' pronto xke' il player ha premuto un tasto direzionale
    system("clear");


    if(FD_ISSET(fileno(stdin),&set1)){
        mov=getchar();
        spostamento_player(mov);


    }

    if(y_pal==NRIGHE-1 && (x_pal>=x1_pl && x_pal<=x1_pl+LUNGHEZZAPLAYER)){ //il player ha intercettato la pallina

                 rimbalzo_su_player(mov,player_on_move,prec_player_on_move,precprec_player_on_move);

                 continue;

    }



    if(y_pal==0) //rimbalzo su muro superiore
        rimbalzo_su_muro_sup();


    if(x_pal==0 || x_pal==NCOLONNE-1) //rimbalzo su muri laterali
        rimbalzo_su_muro_lat();

    if(  x_pal+(1*x_diviso_y_pal)<0 || x_pal+(1*x_diviso_y_pal)>NCOLONNE-1  ) //zona pericolosa di bug sui muri laterali
        //siccome ci troviamo nella zona "pericolosa" del muro laterale, al prossimo giro la pallina, muovendosi a questa "velocita' angolare"
        //uscira' oltre il muro laterale, quindi dobbiamo forzare che al prossimo giro di select, venga eseguita la funzione di rimbalzo sul muro laterale
        //anche se non ci sta ancora sulla casella del muro laterale
            rimbalzo_su_muro_lat();


    if(y_pal==NRIGHE){ //la pallina cade
        system("clear");
        printf("\nHAI PERSO\n");
        exit(0);


    }

      if(movepalla_logic%LIVELLO==0) // ogni LIVELLO giri di select si muove la pallina
            movimento_pallina();
}


}

void inizializza_campo(){
int i,j;
for(i=0;i<NRIGHE;i++)
    for(j=0;j<NCOLONNE;j++)
        campo[i][j]=LIBERO;
}

void disegna_campo(){
int i,j;
 campo[y_pal][x_pal]=PALLINA;

for(i=0;i<NRIGHE-1;i++){
    campo[i][0]=MURO_LAT;
    campo[i][NCOLONNE-1]=MURO_LAT;
}

for(j=0;j<NCOLONNE;j++)
    campo[0][j]=MURO_SUP;

for(j=0;j<=LUNGHEZZAPLAYER;j++)
    campo[NRIGHE-1][x1_pl+j]=PLAYER;



}

void stampa_campo(){
int i,j;

for(i=0;i<NRIGHE;i++){
        for(j=0;j<NCOLONNE;j++)
        printf("%c",campo[i][j]);
        printf("\n");
}

}


void movimento_pallina(){

if(x_pal==0 || x_pal==NCOLONNE-1)
    campo[y_pal][x_pal]=MURO_LAT;
else if(y_pal==0)
    campo[y_pal][x_pal]=MURO_SUP;
else
    campo[y_pal][x_pal]=LIBERO;

if(fase==DISCENDENTE)
        y_pal++;

else
        y_pal--;

x_pal+=1*x_diviso_y_pal;
campo[y_pal][x_pal]=PALLINA;



}


void spostamento_player(int mov){
if(mov==SX && x1_pl!=1){
    campo[NRIGHE-1][x1_pl+LUNGHEZZAPLAYER]=LIBERO;
    x1_pl--;
    campo[NRIGHE-1][x1_pl]=PLAYER;
}

else if(mov==DX && x1_pl!=NCOLONNE-5){
    campo[NRIGHE-1][x1_pl]=LIBERO;
    x1_pl++;
    campo[NRIGHE-1][x1_pl+LUNGHEZZAPLAYER]=PLAYER;

}


}

void rimbalzo_su_player(int mov,int player_on_move,int prec_player_on_move,int precprec_player_on_move){
fase=ASCENDENTE;

if(player_on_move || prec_player_on_move || precprec_player_on_move){ //il player si e' mosso almeno 1 volta negli ultimi 3 giri di select
        if(mov==DX) x_diviso_y_pal+=1;
        else if(mov==SX) x_diviso_y_pal-=1;

}

movimento_pallina();
campo[NRIGHE-1][x_pal]=PLAYER;



}


void rimbalzo_su_muro_sup(){
fase=DISCENDENTE;
campo[0][x_pal]=MURO_SUP;


}


void rimbalzo_su_muro_lat(){
//la fase non cambia
//ma cambia il segno di x_diviso_y
x_diviso_y_pal*=-1;



}
