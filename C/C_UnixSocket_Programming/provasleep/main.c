#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>

int main()
{  printf("\nASPETTIAMO 10 SECONDI!\n");
    struct timeval tempo;
    tempo.tv_sec=10;
    select(NULL,NULL,NULL,NULL,&tempo);

    printf("Hello world!\n");
    return 0;
}
