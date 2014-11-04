/*modificare l'esercizio 1 in modo che prenda l'input dal file testfile e scriva l'output nel file outputfile */
//uso la sprintf perche' non si puo' dare come parametro da scrivere/leggere ad una write/read un int o un singolo
//char, quindi bisogna scrivere formattandole, le variabili del risultato, su un buff stringa,e mandare quello sul file output

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#define PRIMA 1
#define SECONDA 2


int main(int argc,char *argv[]){
int fdOrigine,fdDestinazione,ret,i,a,b;
char buff[100]="\0",buff2[20]="\0";
int ordineAddendi=PRIMA;

if((fdOrigine=open("testfile.txt",O_RDONLY))<0){
        perror("\n"); exit(1);
}

if((fdDestinazione=open("outputfile.txt",O_WRONLY | O_CREAT))<0){
        perror("\n"); exit(1);

}

/*redirezione da stdin e verso stdout punto 2 esercizio
fdOrigine=dup(STDIN_FILENO);
fdDestinazione=dup(STDOUT_FILENO);*/


while(1){
    ret=read(fdOrigine,buff,100);
    if(errno==EINTR && ret<0) continue;
    if(ret<0) exit(1);
    if(ret==0) break;
    i=0;

    while(buff[i]!='\0' && buff[i]!='-'){
            if((int)'0'<=(int)buff[i] && (int)'9'>=(int)buff[i] ) //allora e' un char digit

                if(ordineAddendi==PRIMA){
                    a=(int)(buff[i]-'0');
                    ordineAddendi=SECONDA;
                }

                else if(ordineAddendi==SECONDA){
                    b=(int)(buff[i]-'0');
                    sprintf(buff2,"\n%d + %d =%d\n",a,b,a+b);
                    ret=write(fdDestinazione,buff2,strlen(buff2));
                    ordineAddendi=PRIMA;
                }

            i++;
    }
}


close(fdOrigine);
close(fdDestinazione);
return 0;
}
