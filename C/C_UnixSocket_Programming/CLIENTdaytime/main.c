#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc,char *argv[]) {
int sockfd,n;
char buffer[1025];


if(argc!=2) {printf("\nInserisci <IP>"); exit(1);}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0)
        {
            fprintf(stderr,"\nErr creazione socket\n");  //creato
            exit(1);
        }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


struct sockaddr_in strutt_ind;
if(inet_pton(AF_INET,argv[1],&(strutt_ind.sin_addr))<=0)
    {
        printf("\nIP error"); exit(1);
    }
strutt_ind.sin_family=AF_INET;
strutt_ind.sin_port=htons(13);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    if(connect(sockfd,(struct sockaddr *)&strutt_ind,sizeof(strutt_ind))<0)
    {printf("\nCONNECTION PROBLEM");
     exit(1);
    }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

while((n=read(sockfd,buffer,1024))>0)
    {buffer[n]='\0'; //mettiamo il terminatore stringa sul successivo dell'ultima lettera scritta sul buffer dalla read
      //e stampiamo sullo stream stdout facendo attenzione che la fputs non restituisca EOF( che e' il modo in cui la fputs segnala errore)
     if(fputs(buffer,stdout)==EOF) {printf("\nfputs error"); exit(1);}


    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
