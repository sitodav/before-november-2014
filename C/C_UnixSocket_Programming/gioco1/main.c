///GESTISCI COSA SUCCEDE QUANDO I MOB VANNO NELLE ALTERAZIONI (BUCHI NEI MURI)--> NON POSSONO ANDARE NEI BUCHI DEI MURI
///GESTISCI LA CREAZIONE DELLE PAROLE, IL FATTO CHE IL PG PUO' PRENDERE SOLO UNA LETTERA
///PRECISA TRA LE TANTE, E IL FATT OCHE QUANDO LE PRENDE SI PROSEGUE CON LA PROSSIMA LETTERA
///E QUELLA PRESA SPARISCE
///fai stampare solo le lettere della parola scelta


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <time.h>
#include <termios.h>
#define NRIGHE 19 //definire qui n tot righe
#define NCOLONNE 60 //n tot colonne
#define MURO 31
#define LIBERO 32
#define CONDIZIONE_ALTERAZIONE ((i==4 || i==5) && (j==0 || j==NCOLONNE-1)) //definire qui la "legge" di alterazione del campo                                                                                                                                                            //far si che i "buchi" nei muri siano simmetrici
#define ALTERAZIONE LIBERO //definire qui l'intero dell'alterazione nel campo
#define SU 'w' //definire i tasti per le direzioni
#define GIU 's'
#define SX 'a'
#define DX 'd'
#define me '0'



struct coordinate_char{
        int ime;
        int jme;
        char char_coda;
} ultime_mie30_posizioni[31]; //riempiamo dall'ultima, alla prima, nell'ultima c'e' sempre la piu' recente, e spostiamo sempre in avanti i char
                                            //in modo tale che stiano sempre nelle ultime posizioni visitate


struct rec_parola{
    char parola[30];
    int lunghezza;
} lista_parole[100];
int ind_prossima_touse=0;
char parola_obiettivo[30],parola_obiettivo_mobpresi[30],parola_rimasta_tospawn[30];
int ind_reverse_mobparola,ind_mob_totake;




char campo[NRIGHE][NCOLONNE];
struct mob_record{
            int i;
            int j;
            int ultima_direzione;
            char carattere;
} mob_spawnati[100];
int ind_ultimo=0;
//le variabili le lasciamo globali cosi' nei cicli che devono essere molto veloci, non bisogna ogni volta passare le variabili

void inizializza_lista(); //questa non e' la funzioen che sceglie la parola, ma crea la lista delle parole
void crea_obiettivo(); //questa e' la funzione che crea laparola prendendola dalla lista
void spawna_mob();
void aggiorna_ultime30posizioni(int ime,int jme);
void perso(struct termios *old,int punteggio);
void inizializza_campo();
void altera_campo();
void stampa_campo();
int controlla_sovrapp_coda(int i,int j);

int main() {
srand((unsigned int)time(NULL));
struct termios old,newt;
tcgetattr( STDIN_FILENO, &old );
newt = old;
newt.c_lflag &= ~( ICANON | ECHO );
tcsetattr( STDIN_FILENO, TCSANOW, &newt );
// ////////// ABBIAMO SETTATO IL BUFFER DELLA TASTIERA COME NON BUFFERIZZANTE, USANDO LA LIBRERIA termios.h

fd_set set1; FD_ZERO(&set1);
struct timeval tempo;
char mov,ultima_direzione=DX,ultima_valida=ultima_direzione;  //il serpente inizia muovendosi a destra
int m,i,j,ime=NRIGHE/2,jme=NCOLONNE/2,ciclo_attuale=0,ciclo_ultimospawn=0,serve_nuovaparola=1; //il pg ha indici a meta' del campo su entrambe le direzioni
int punteggio_bonus=0;

for(i=0;i<=30;i++){
    ultime_mie30_posizioni[i].ime=-1;
    ultime_mie30_posizioni[i].jme=-1;
    ultime_mie30_posizioni[i].char_coda='\0';

}



inizializza_campo();//iniziamolizziamo il campo
campo[ime][jme]=me; //inseriamo il pg
altera_campo();//inseriamo i mutatori secondo la legge definita sopra
inizializza_lista();





while((++ciclo_attuale)<=100000000){

    if(serve_nuovaparola){ //allora hai completato l'ultima parola, quindi bisogna iniziare a spawnare lettere per la nuova
            for(m=0;m<ind_ultimo;m++){
                mob_spawnati[m].i=0;
                mob_spawnati[m].j=0;
                mob_spawnati[m].carattere=' ';
                mob_spawnati[m].ultima_direzione=0;
            }
            ind_ultimo=0;

            for(m=0;m<30;m++){
                    ultime_mie30_posizioni[m].char_coda=' ';
                    ultime_mie30_posizioni[m].ime=-1;
                    ultime_mie30_posizioni[m].jme=-1;

            }

            crea_obiettivo();
            ind_reverse_mobparola=strlen(parola_obiettivo)-1;
            ind_mob_totake=0;

            serve_nuovaparola=0;
    }

    if(ciclo_attuale-ciclo_ultimospawn==10){ //stampa ogni 1000 cicli di while
                    ciclo_ultimospawn=ciclo_attuale;
                    spawna_mob();


    }



    tempo.tv_usec=100000;
    tempo.tv_sec=0.9;
    FD_SET(fileno(stdin),&set1);
    system("clear");

   //printf("\nPAROLA:%s \n",parola_rimasta_tospawn); fflush(stdout);
    stampa_campo();//stampiamo campo
   //printf("\nPREMI <<P>>  PER LA PAUSA"); fflush(stdout);
   int o;
   for(o=0;o<10;o++)
        printf("////i:%d-j:%d-%c",ultime_mie30_posizioni[o].ime,ultime_mie30_posizioni[o].jme,ultime_mie30_posizioni[o].char_coda); fflush(stdout);





    select(fileno(stdin)+1,&set1,NULL,NULL,&tempo);


    if(FD_ISSET(fileno(stdin),&set1))
                    ultima_direzione=getchar();



   if(ultima_direzione==SU && ultima_valida!=GIU) {
                 ime--;
                 campo[ime+1][jme]=LIBERO;

                 if(campo[ime][jme]==MURO){

                    perso(&old,ciclo_attuale+punteggio_bonus);
                }
                 else if(campo[ime][jme]==LIBERO){
                    if(ime==0)
                        ime=NRIGHE-1;
                    campo[ime][jme]=me;
                }

                else if(campo[ime][jme]==parola_obiettivo[ind_mob_totake]){
                        parola_obiettivo_mobpresi[ind_mob_totake]=parola_obiettivo[ind_mob_totake];
                        //
                        ultime_mie30_posizioni[ind_mob_totake+1].char_coda=parola_obiettivo[ind_mob_totake];

                        //
                        ind_mob_totake++;
                        if(strcmp(parola_obiettivo,parola_obiettivo_mobpresi)==0){
                                serve_nuovaparola=1;
                                punteggio_bonus+=500;
                        }
                }
                 else
                         perso(&old,ciclo_attuale+punteggio_bonus);




    }

    else if(ultima_direzione==GIU && ultima_valida!=SU){
                 ime++;
                 campo[ime-1][jme]=LIBERO;

                 if(campo[ime][jme]==MURO){

                    perso(&old,ciclo_attuale+punteggio_bonus);
                }
                 else if(campo[ime][jme]==LIBERO){
                    if(ime==NRIGHE-1)
                        ime=0;
                    campo[ime][jme]=me;
                }

                else if(campo[ime][jme]==parola_obiettivo[ind_mob_totake]){
                        parola_obiettivo_mobpresi[ind_mob_totake]=parola_obiettivo[ind_mob_totake];

                        //
                        ultime_mie30_posizioni[ind_mob_totake+1].char_coda=parola_obiettivo[ind_mob_totake];
                        //
                        ind_mob_totake++;
                        if(strcmp(parola_obiettivo,parola_obiettivo_mobpresi)==0){
                              serve_nuovaparola=1;
                              punteggio_bonus+=500;
                        }
                }

                 else
                         perso(&old,ciclo_attuale+punteggio_bonus);

    }

    else if(ultima_direzione==DX && ultima_valida!=SX) {
                jme++;
                campo[ime][jme-1]=LIBERO;

                if(campo[ime][jme]==MURO){

                    perso(&old,ciclo_attuale+punteggio_bonus);
                }
                else if(campo[ime][jme]==LIBERO){
                    if(jme==NCOLONNE-1)
                        jme=0;
                    campo[ime][jme]=me;
                }

                else if(campo[ime][jme]==parola_obiettivo[ind_mob_totake]){
                        parola_obiettivo_mobpresi[ind_mob_totake]=parola_obiettivo[ind_mob_totake];
                        //
                        ultime_mie30_posizioni[ind_mob_totake+1].char_coda=parola_obiettivo[ind_mob_totake];
                        //
                        ind_mob_totake++;
                        if(strcmp(parola_obiettivo,parola_obiettivo_mobpresi)==0){
                                serve_nuovaparola=1;
                                punteggio_bonus+=500;
                        }
                }

                 else
                         perso(&old,ciclo_attuale+punteggio_bonus);


    }


    else if(ultima_direzione==SX && ultima_valida!=DX){
                 jme--;
                 campo[ime][jme+1]=LIBERO;

                 if(campo[ime][jme]==MURO){

                    perso(&old,ciclo_attuale+punteggio_bonus);
                }
                 else if(campo[ime][jme]==LIBERO){
                    if(jme==0)
                        jme=NCOLONNE-1;
                    campo[ime][jme]=me;
                }

                else if(campo[ime][jme]==parola_obiettivo[ind_mob_totake]){
                        parola_obiettivo_mobpresi[ind_mob_totake]=parola_obiettivo[ind_mob_totake];
                        //
                        ultime_mie30_posizioni[ind_mob_totake+1].char_coda=parola_obiettivo[ind_mob_totake];
                        //
                        ind_mob_totake++;
                        if(strcmp(parola_obiettivo,parola_obiettivo_mobpresi)==0){
                                serve_nuovaparola=1;
                                punteggio_bonus+=500;
                        }
                }

                 else
                         perso(&old,ciclo_attuale+punteggio_bonus);




    }

    else if(ultima_direzione=='p'){
        printf("\npremi una delle 4 direzioni per riprendere\n");
        while(ultima_direzione!=DX && ultima_direzione!=SX && ultima_direzione!=SU && ultima_direzione!=GIU){
            FD_SET(fileno(stdin),&set1);
            select(fileno(stdin)+1,&set1,NULL,NULL,NULL);
            ultima_direzione=getchar();
        }

    }

    else{
        ultima_direzione=ultima_valida; //in questo modo si evita che il programma si blocch inon solo se premuto P ma se premuto qualche altro tasto
        continue;
    }

    aggiorna_ultime30posizioni(ime,jme);
    ultima_valida=ultima_direzione;


}

}


void inizializza_campo(){
int i,j;
for(i=0;i<NRIGHE;i++)
    for(j=0;j<NCOLONNE;j++)
        if(i==0 || i==NRIGHE-1 || j==0 || j==NCOLONNE-1) campo[i][j]=MURO;
        else campo[i][j]=LIBERO;

}


void altera_campo(){
int i,j;
for(i=0;i<NRIGHE;i++)
    for(j=0;j<NCOLONNE;j++)
        if(CONDIZIONE_ALTERAZIONE)
                campo[i][j]=ALTERAZIONE;



}

void stampa_campo(){

int i,j;
char ret;

for(i=0;i<NRIGHE;i++){
    for(j=0;j<NCOLONNE;j++)
        if((ret=controlla_sovrapp_coda(i,j))==0) //quella e' una casalle di campo senza un char di coda sopra
            printf("%c",campo[i][j]);
        else printf("%c",ret); //altrimenti stampiamo il valore ritornato dalla controlla_sovrapp_coda che e' la lettera presente a quelle coord
    printf("\n");

}

}


void perso(struct termios *old,int punteggio) {
printf("\nHAI PERSO!\n");
printf("\nHAI TOTALIZZATO %d",punteggio);
exit(0);


}


void spawna_mob(){
if(ind_reverse_mobparola<0) return;
do{
    mob_spawnati[ind_ultimo].i=5+rand()%(NRIGHE-10); //generiamo randomicamente le coordinate del mob
    mob_spawnati[ind_ultimo].j=5+rand()%(NCOLONNE-10);
}
    while(campo[mob_spawnati[ind_ultimo].i][mob_spawnati[ind_ultimo].j]==me);
//le direzioni dei mob, sono riferite in 1-2-3-4 per farli spostare randomicamnete
mob_spawnati[ind_ultimo].ultima_direzione=1+rand()%4;
mob_spawnati[ind_ultimo].carattere=parola_obiettivo[ind_reverse_mobparola];
parola_rimasta_tospawn[ind_reverse_mobparola]='*';
//lo piazziamo, disegnandolo nell'array in base al tipo
campo[ mob_spawnati[ind_ultimo].i ] [ mob_spawnati[ind_ultimo].j ]=mob_spawnati[ind_ultimo].carattere;

ind_reverse_mobparola--;
ind_ultimo++;

}


void inizializza_lista(){

strcpy(lista_parole[0].parola,"cane");
lista_parole[0].lunghezza=4;

strcpy(lista_parole[1].parola,"scatola");
lista_parole[1].lunghezza=7;

strcpy(lista_parole[2].parola,"sedia");
lista_parole[2].lunghezza=5;

strcpy(lista_parole[3].parola,"soldi");
lista_parole[3].lunghezza=5;

///continua la lista aumentando la lunghezza all'aumentare dell'indice dell'array


}


void crea_obiettivo(){

    strcpy(parola_obiettivo,lista_parole[ind_prossima_touse].parola);
    strcpy(parola_rimasta_tospawn,parola_obiettivo);
    parola_obiettivo[strlen(parola_obiettivo)]='\0';
    ind_prossima_touse++;

}

void aggiorna_ultime30posizioni(int ime,int jme){ //trasla tutto in avanti e inserisce al primo posto l'ultima coppia di coordinate
int i,j;


for(i=0;i<=30;i++)
    if(ultime_mie30_posizioni[i].ime==-1)
        break;
if(i==31)
    j=29;
else j=i-1;
while(j>=0){
    ultime_mie30_posizioni[j+1].ime=ultime_mie30_posizioni[j].ime;
    ultime_mie30_posizioni[j+1].jme=ultime_mie30_posizioni[j].jme;
    j--;
}
ultime_mie30_posizioni[0].ime=ime;
ultime_mie30_posizioni[0].jme=jme;


}


int controlla_sovrapp_coda(int i,int j){
int k;
for(k=1;k<=30;k++)
    if(ultime_mie30_posizioni[k].ime==i && ultime_mie30_posizioni[k].jme==j)
            return ultime_mie30_posizioni[k].char_coda;

return 0;



}
