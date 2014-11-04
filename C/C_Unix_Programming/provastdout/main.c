#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(){

char towrite[40];
sprintf(towrite,"il mio stdout ha fd valore: %d\0",fileno(stdout));
write(1,towrite,strlen(towrite));


return 0;
}
