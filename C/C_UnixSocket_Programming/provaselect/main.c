#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/select.h>
#include <unistd.h>

int mygetch();
int main()
{
    char a;
    fd_set set1;
    FD_ZERO(&set1);
    FD_SET(fileno(stdin),&set1);

     struct termios oldt,
                 newt;
  int            ch;
  tcgetattr( STDIN_FILENO, &oldt );
  newt = oldt;
  newt.c_lflag &= ~( ICANON | ECHO );
  tcsetattr( STDIN_FILENO, TCSANOW, &newt );

    select(fileno(stdin)+1,&set1,NULL,NULL,NULL);
    if(FD_ISSET(fileno(stdin),&set1)){
       // a=getchar();

        printf("\nSI\n");
        sleep(2);
    }

}

int mygetch() {
  struct termios oldt,
                 newt;
  int            ch;
  tcgetattr( STDIN_FILENO, &oldt );
  newt = oldt;
  newt.c_lflag &= ~( ICANON | ECHO );
  tcsetattr( STDIN_FILENO, TCSANOW, &newt );
  ch = getchar();
  tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
  return ch;
}
