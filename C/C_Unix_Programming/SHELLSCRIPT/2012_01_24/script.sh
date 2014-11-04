#!/bin/sh
a(){

sed -i '/Ricercatore/s/^/Dott./' $1
sed -i '/Associato\|Ordinario/s/^/Prof./' $1 #cioe' fai substitute delle righe dove c'e' o Associato o Ordinario, mettendo al posto della stringa vuota a inizio linea (^) quindi all'inizio, la parola Prof.

}

b(){

echo "Inserisci nome docente di cui voi sapere la stanza"
read nome

grep "$nome" $1 >"temp.txt"  #fai grep della sola riga con quel nome
stanza=`sed 's/[[:alpha:]]//g' "temp.txt"` #..e su quella sola riga, elimina tutti i char alfabetici e lascia i numeri, salva in variabile stanza la stringa risultante come output
echo "la stanza di "$nome" e' "$stanza

}

c() {

#COME SOSTITUIRE SOLO LA PRIMA OCCORRENZA DELLA STRINGA VUOTA, CON PETROSINO, ELIMINANDO SENZA METTERCI NIENTE AL LORO POSTO, TUTTE LE SUCCESSIVE ?
#cosi:

sed -i '1,/^$/s/^$/Prof.Alfredo Petrosino Associato 429/' $1  
#cioe: le righe sulle quali fare substitute devono essere innanzitutto quelle dalla prima fino prima riga vuota, e solo su queste al posto della riga vuota metti la stringa con i dati di petrosino. Questo equivale a dire di farlo soltanto
#per la prima riga vuota perche' si va dalla riga 1 alla PRIMA riga che soddisfa l'espr regolare ^$, che e' proprio la prima vuota

#e ora tutte le altre rimaste vuote le eliminiamo semplicemente
sed -i '/^$/d' $1

}



d() {

sed -i 's/[[:digit:]]/x/g' $1 #sostituisci tutte(g) le occorrenze di numeri ,di qualsiasi riga(niente prima di s), con x

}


if [ $# -ne 1 ] 
then
	echo "uso: file.txt"	
	exit 0
fi

while [ 1 ]
do
	echo "a)funzione...."
	echo "b)numero di stanza..."
	echo "c)cancella linee vuote e mettici petrosino"
	echo "d)xxx al posto dei numeri di stanza"
	read ris
	if [ "$ris" = "a" ]
	then
		a $1
	elif [ "$ris" = "b" ]
	then
		b $1
	elif [ "$ris" = "c" ]
	then
		c $1
	elif [ "$ris" = "d" ]
	then
		d $1
	else
		echo "Hai inserito un tasto non riconosciuto!"
	fi
done
exit 0
