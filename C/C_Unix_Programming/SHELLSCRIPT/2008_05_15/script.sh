#!/bin/sh
HELP(){
	echo "Passa da riga di comando un prezzo. Premi 2....."
}

MAGGIORA(){
	
	attuale=$(($1+$1/5))
	echo "prezzo maggiorato:  $attuale"

}

SOMMA(){
	echo "inserisci primo numero"
	read n1
	echo "inserisci secondo numero"
	read n2
	echo "la somma e': " $(($n2+$n1))

}






while [ 1 ]
do
	if [ $# -ne 1 ]
	then
		echo "uso: prezzo"
		exit 1
	fi

	echo "1)help"
	echo "2)maggiorazione prezzo"
	echo "3)somma 2 numeri da tastiera"
	read ris
	if [ "$ris" -eq 1 ]
	then
		HELP
	
	elif [ "$ris" -eq 2 ]
	then
		MAGGIORA $1

	elif [ "$ris" -eq 3 ]
	then
		SOMMA
		
	
	else
		echo "Risposta Errata"
	fi
done
exit 0
