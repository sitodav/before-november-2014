#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


WINDOW *nuova_window(int nrighe,int ncolonne,int posy,int posx);
void cancella_window(WINDOW *pw);
int main() {

initscr();
noecho();
cbreak();
keypad(stdscr,TRUE);
//
int nrighe,ncolonne,startx=COLS/2,starty=LINES/2;
int mov,pid;
printw("\ntot righe: --> ");
scanw("%d",&nrighe);
printw("\ntot colonne: --> ");
scanw("%d",&ncolonne);
wrefresh(stdscr);
WINDOW *pw2=nuova_window(nrighe,ncolonne,starty-nrighe,startx);
wborder(pw2,' ',' ',' ',' ',' ',' ',' ',' '); wrefresh(pw2);
if((pid=fork())==0) {
while(1){
    wprintw(pw2,"\n|");
    wrefresh(pw2);
}
}

WINDOW *pw=nuova_window(nrighe,ncolonne,starty,startx);

//noecho();
while((mov=getch())!='q'){

    switch(mov){
            case KEY_UP:
                cancella_window(pw);
                pw=nuova_window(nrighe,ncolonne,--starty,startx);
                break;

            case KEY_DOWN:
                cancella_window(pw);
                pw=nuova_window(nrighe,ncolonne,++starty,startx);
                break;

            case KEY_LEFT:
                cancella_window(pw);
                pw=nuova_window(nrighe,ncolonne,starty,--startx);
                break;

            case KEY_RIGHT:
                cancella_window(pw);
                pw=nuova_window(nrighe,ncolonne,starty,++startx);
                break;

        }




}



//
getch();
endwin();
return 0;
}

WINDOW *nuova_window(int nrighe,int ncolonne,int posy,int posx){
WINDOW *pw=newwin(nrighe,ncolonne,posy,posx);
wborder(pw,'|','|','-','-','*','*','*','*');
wrefresh(pw);
return pw;

}

void cancella_window(WINDOW *pw){
wborder(pw,' ',' ',' ',' ',' ',' ',' ',' ');
wrefresh(pw);
delwin(pw);

}
