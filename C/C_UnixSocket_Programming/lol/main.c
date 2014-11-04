#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <time.h>

int main() {

int x,f;
srand((unsigned int)time(NULL));
struct timeval tempo;

while(1) {
tempo.tv_sec=1;
tempo.tv_usec=1000;
select(NULL,NULL,NULL,NULL,&tempo);
x=rand()%10;

for(f=0;f<x;f++)
    printf(" ");
printf("\aILARIA |_|");

printf("\n");

             }

                }
