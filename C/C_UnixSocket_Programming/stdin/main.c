#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>



int main()
{
    int connsfd;
    connsfd=socket(AF_INET,SOCK_STREAM,0);
    printf("%d %d",connsfd,stdin);
}
