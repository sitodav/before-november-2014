#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main(int argc,char *argv[]) {
int i;
struct hostent *hind;
char str[INET_ADDRSTRLEN]; //INET_ADDRSTRLEN e' una costante definita per contenere un IP di tipo presentation (cioe'la stringatipo 128.167.0.1)
char **ptrstr;
printf("\nStai eseguendo una query nslookup per i seguenti hostname:\n");
for(i=1;i<argc;i++)
    printf("%s\n",argv[i]);
////
for(i=1;i<argc;i++) {
    if((hind=gethostbyname(argv[i]))==NULL)
        printf("\nERRORE NELLA RISOLUZIONE DEL DOMAIN NAME %s\n%s\n",argv[i],hstrerror(h_errno));
    else if(hind->h_addrtype==AF_INET)  {
                printf("\nNOME UFFICIALE:%s\n",hind->h_name);
                ptrstr=hind->h_aliases;
                while(*ptrstr!=NULL)
                    {printf("\nALIAS:%s\n",*ptrstr);
                     ptrstr++;
                    }
                ptrstr=hind->h_addr_list;
                while(*ptrstr!=NULL)
                    {  inet_ntop(AF_INET,*ptrstr,str,sizeof(str));
                        printf("IP:%s\n",str);
                        ptrstr++;
                    }


                                                            }





}



                                               }
