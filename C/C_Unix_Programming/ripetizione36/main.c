#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>


int main(int argc,char *argv[]){


sigset_t set1;
sigemptyset(&set1);
sigaddset(&set1,SIGINT);
sigprocmask(SIG_SETMASK,&set1,NULL);
pause();







}
