#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc,char *argv[])
{ printf("%s",argv[0]);
   FILE *pfile;
    char *percorso;
    percorso=calloc(strlen(argv[0])+1,sizeof(char));
   if((pfile=fopen("/home/molo/progettiC/CLIENTchat/bin/Debug/ciao.txt","a"))==NULL)
    printf("\nImpossibile aprire il file!");

    int i;

i=strlen(argv[0]); //cosi' facendo stiamo esattamente sul terminatore \0 della stringa del percorso argv[0] completo

while(i>=0)
    {
        if(*(argv[0]+i)=='/')
            break;
            i--;

    }
    printf("la prima / si trova in posizione %d",i);
    //in questo modo si piazza sul terminatore stringa dell'indirizzo comprensivo del nome dell'exe che sta in argv[0]
    //e si sposta all'indietro finche' non trova il primo / che e' il carattere DOPO il quale sta il nome dell'exe
    //noi dovremo copiare quindi in percorso, percorso completo (che altro non e' che argv[0]) dal primo char in indice 0 fino al char di indice
    //del valore con cui e' uscito i dal while, questo equivale a fare una strncpy di percorso completo in percorso, dei soli i+1 char
    strncpy(percorso,argv[0],i+1);
    //ora dobbiamo terminare la stringa pero'
    percorso[i+1]='\0';
    printf("\nargv[0]:%s\npercorso:%s",argv[0],percorso);


    char stringa[]="ECCOMI!",stringa2[]="ECCOMI2!";

    fprintf(pfile,"%s\n",stringa);
    fprintf(pfile,"%s\n",stringa2);
}
