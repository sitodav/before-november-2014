#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>

int main(int argc,char *argv[]) {
struct sockaddr_in ind1;

printf("\n%s",argv[1]);
fflush(stdout);
ind1.sin_port=htons(22);
ind1.sin_family=AF_INET;
inet_pton(AF_INET,argv[1],&ind1.sin_addr);

   char ipquery2[INET_ADDRSTRLEN];
struct hostent *queryres,*query2;


if((query2=gethostbyname(argv[1]))==NULL)
    printf("\n%s",hstrerror(h_errno));

else {

    inet_ntop(AF_INET,*(query2->h_addr_list),ipquery2,INET_ADDRSTRLEN);
    printf("\n ip %s",ipquery2);


}












}
