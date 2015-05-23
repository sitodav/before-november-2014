#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>


void mio_handler(int sign);
int main() {
struct sigaction nuovo,vecchio;
int pid;
nuovo.sa_flags=0;
nuovo.sa_handler=mio_handler;
sigemptyset(&nuovo.sa_mask);

if(sigaction(SIGCHLD,&nuovo,&vecchio)<0){
    perror("\n");
    exit(1);
}

else printf("\ncreata una nuova gestione per il SIGCHLD signal: mio handler\n"); fflush(stdout);

if((pid=fork())==0){
        printf("\nprocesso figlio:chiusura figlio in corso\n");
        exit(0);

}
else printf("\nprocesso padre:creato figlio 1\n");
sleep(5); //qui viene intercettato il SIGCHLD della chiusura de lfiglio, usando il nostro handler...
// poi torniamo alle vecchie impostazioni

if(sigaction(SIGCHLD,&vecchio,NULL)<0){
    perror("\nseconda sigacton");
    exit(1);
}

else printf("\nrecuperate vecchie impostazioni per la gestione del SIGCHLD signal (:ignora signal)\n"); fflush(stdout);
//abbiamo rimesso le vecchie impostazioni per la gestione del segnale SIGCHLD
if((pid=fork())==0){
        printf("\nfiglio 2:chiusura in corso\n");fflush(stdout);
        exit(0);

}
printf("\ncreato figlio 2\n");
sleep(5);

//qui non dovrebbe apparire nient altro nella stampa, in quanto il SIGCHLD viene gestito come nel vecchio settaggio, ovvero di default,
//quindi ignorato

}

void mio_handler(int sign){
printf("\nMIO HANDLER PER SIGCHLD: FIGLIO CHIUSO\n");
}
