#include "mieiheader.h"

int main(int argc,char *argv[]){

//installiamo un handler con la signal
signal(SIGINT,handlerSigInt);

while(1);




}
