#!/bin/sh

f(){

if [ -e "$1" ]
then
	echo "il file  $1 esiste"
else
	echo "il file $1 non esiste"
	exit 0
fi

if [ -w "$1" ]
then
	echo "abbiamo diritti di scrittura"
else
	echo "non ci sono diritti di scrittura"
	exit 0
fi

if [ -r "$1" ]
then
	echo "abbiamo diritti anche in lettura"
else
	echo "...ma non abbiamo diritti anche in lettura"
	exit 0
fi

echo "inizio operazioni di modifica"
echo "nel file le righe che iniziano con south sono"
sed -n /"^south"/p $1 #-n nel sed fa si che vengan omostrate SOLO quelle e non anche le altre
echo "sostituisco SOUTH con south dalla riga 1 alla 7"
sed -i 's/south/SOUTH/' $1 #i rende definite le modifiche salvandole nel file target di sed
echo "cancello le righe che contengono Sharon o sharon"
sed -i /"Sharon\|sharon"/d $1 #siccome le espr regolari nel sed non sono estese come quelle di egrep, serve l'escape \ affinche' il metacarattere | non perda il suo valore, e funzioni da OR
echo "modifiche effettuate e salvate! Mostro il file"
cat $1

}


if [ $# -ne 1 ]
then
	echo "uso: nomefile.txt"
	exit 0
fi
echo "lancio funzione"
f $1
exit 0
