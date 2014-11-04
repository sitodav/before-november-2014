#!/bin/sh
ELIMINA(){
 	sed '/^I/d' $1 > file1.txt
	cat $1

}

SOSTITUISCI(){

	sed 's/Sign./Ins./g' $1 > file2.txt 

}

RIFORMATTA(){
#Stampare a video soltanto la riga dei nom i e, success ivamente, ord inarl i in senso alfabetico decrescente
echo "lista dei nomi"
awk '{printf("%s  ",$2)}' $1
echo "\n"
echo "lista nomi ordinata in senso decrescente"
sort -r file2.txt #il sort lo facciamo sul file2.txt e non sull'output dell'awk, in quanto i lsort lavora tra righe diverse, mentre con awk abbiamo messo tutto sulla stessa riga, quindi non funzionerebbe il sort -r inverso

}







if [ $# -ne 1 ]
then
	echo "uso: nomefile.txt"
	exit 1
fi

while [ 1 ]
do
	echo "1)Elimina righe che iniziano per I e metti tutto in file1.txt"
	echo "2)Sostituire Sign con Ins e metti tutto in file2.txt"
	echo "3)Stampa solo riga dei nomi e ordina a video in senso alfabetico decrescente"
	read ris
	
	if [ "$ris" -eq 1 ]
	then
		ELIMINA $1
	
	elif [ "$ris" -eq 2 ]
	then
		SOSTITUISCI $1
	elif [ "$ris" -eq 3 ]
	then
		RIFORMATTA $1
	else
		echo "risposta errata"
	fi


done
exit 0
