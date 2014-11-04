#!/bin/sh
NUMOCC(){
	numocc=`grep -c "Linux" $1`
	echo $numocc
	

}

NRIGHE(){
#per stampare solo i numeri di riga sulle quali c'e' l'occorrenza
out=`grep -n "Linux" $1 | sed 's/[^0-9]//g'` #per prima cosa usiamo grep facendogli numerare (-n) le righe con l'occorrenze, e poi questo output dato da grep lo mandiamo con una pipeline in input a sed, facendogli sostituire(s///) TUTTI (g) i caratteri non numerici ([^0-9]) con char vuoti

echo "I numeri di riga sulle quali ci sono le occorrenze sono \n $out"

}








if [ $# -ne 1 ]
then
	echo "uso: passami nomefile.txt"
	exit 1
fi

while [ 1 ]
do
	echo "1)calcola num occorrenze parola LINUX nel testo"
	echo "2)n riga occorrenza LINUX"
	echo "3)se numocc>2 stampa solo prime 2 righe del testo"
	read ris
	
	if [ "$ris" -eq 1 ]
	then
		valore=`NUMOCC $1`
		echo "valore: $valore" 

	elif [ "$ris" -eq 2 ]
	then	
		NRIGHE $1
	elif [ "$ris" -eq 3 ]
	then
		if [ "$valore" -gt 2 ]
		then
			echo "Il numero di occorrenze e' >2, quindi stampo solo le prime 2 righe del testo"
			sed -n '1,2p' $1 #metto -n altrimenti mi stamperebbe anche tutte le altre
		
		else
			echo "Il numero di occorrenze e' <=2, quindi stampo tutto il testo"
			cat $1				
		fi
	else
		echo "risposta errata"

	fi
done
exit 0
