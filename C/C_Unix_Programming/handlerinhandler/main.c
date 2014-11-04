#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>




void handler(int sign);
void prova();

int main(int argc,char *argv[]){
signal(SIGINT,handler);
while(1);


return 0;
}


void handler(int sign){
printf("\nSono handler\n");
while(1);


}

void prova(){
printf("\nSono prova\n");
while(1);


}
