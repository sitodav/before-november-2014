#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <time.h>

int main() {
fd_set set1;

struct timeval tempo;
tempo.tv_sec=5;
tempo.tv_usec=0;
FD_ZERO(&set1);
FD_SET(fileno(stdin),&set1); //impostiamo su on il file descriptor che corrisponde al puntatore stdin, nel set1
select(fileno(stdin)+1,NULL,&set1,NULL,&tempo); //il processo si blocca QUI e prosegue appena succede qualcosa su uno qualunque dei fp che abbiamo "acceso" nel set che siano inferiori di maxfd+1
if(FD_ISSET(fileno(stdin),&set1))
                    printf("\nstdin pronto\n");

                }
