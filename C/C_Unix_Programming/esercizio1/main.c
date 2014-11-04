#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc,char *argv[]){
char buff[10]="\0";
int a,b;
while(1){
    printf("\nInserisci primo numero (1 sola cifra //-1 per uscire)\n");

    read(STDIN_FILENO,buff,1);

    if(buff[0]=='-') return 0;

    while(getchar()!='\n');

    a=(buff[0]-(int)'0');

    printf("\nInserisci il secondo numero\n");

    read(STDIN_FILENO,buff,1);

    if(buff[0]=='-') return 0;

    while(getchar()!='\n');

    b=(buff[0]-(int)'0');

    sprintf(buff,"%d",a+b);

    write(STDOUT_FILENO,buff,strlen(buff));

}



}
