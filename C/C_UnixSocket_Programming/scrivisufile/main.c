#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {
FILE *pfile;
char percorso[FILENAME_MAX],nomefile[10],testo[2000];
int i=0;

getcwd(percorso,sizeof(percorso));
strcat(percorso,"/");
printf("\nIns nome file da creare\n");
fgets(nomefile,10,stdin);
strcat(percorso,nomefile);
percorso[strlen(percorso)-1]='\0';
strcat(percorso,".txt");

printf("\nInserisci il testo da trascrivere sul file\n");
while((testo[i]=getchar())!=EOF)  //in questo modo la sessioen di inserimento terminat quando inseriamo eof, ma questo verra' comunque salvato nel file
    i++;

//l'eof lo sostituiamo con \0, visto che un eof e' un char senza significato per una stringa
//poi quando andremo a trascrivere quella stringa su file, dopo il \0, verra' automaticamente salvato un eof dal kernel
testo[strlen(testo)-1]='\0';

do pfile=fopen(percorso,"a");
    while (pfile==NULL);

i=fprintf(pfile,"TESTO:\n\n%s\n",testo);
printf("\nscritti su file %d char",i);

fclose(pfile);



return 0;}
