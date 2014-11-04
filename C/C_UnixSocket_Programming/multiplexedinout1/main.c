#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <time.h>

int main() {

struct timeval tempo;
tempo.tv_sec=0;
tempo.tv_usec=0;

fd_set set_fdescrittori;
FD_ZERO(&set_fdescrittori);
FD_SET(0,&set_fdescrittori);
do {select(1,&set_fdescrittori,NULL,NULL,&tempo);}
while(FD_ISSET(0,&set_fdescrittori)==0);
}


