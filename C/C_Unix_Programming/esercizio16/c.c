#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>


void siginthandler(int sign);
int main(int argc,char *argv[]){
//questo programma, termina solo quando il fratello secondo figlio gli manda SIGINT, quindi
//dovremmo disattivare tutti i segnali che non sono un SIGINT in modo tale che solo il SIGINT del fratello ci puo' far terminare..ma per semplicita'...
if(signal(SIGINT,siginthandler)==SIG_ERR){
	perror("\n");
	exit(1);

}

printf("\nSono il programma %s , e aspetto SIGINT da mio fratello per terminare!\n",argv[0]);
pause();

return 0; //superfluo
}


void siginthandler(int sign){
printf("\nHo ricevuto sigint! termino\n");
fflush(stdout);
exit(0);


}
