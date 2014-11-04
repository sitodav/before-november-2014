


uno(){
	grep -o "\<più.\>" $1 | wc -l  #tutta la parola piu' non sono l'occorrenza (ecco perche' usiamo \<..\>



}

due(){
	#per salvare il tot di righe come singolo intero (siccome wc -l mi da il problema che mi stampa anche i lnome del file) da passare alla sed -p per fargli stampare ultima riga, faccimo
	tot=`grep -c "^." $1`
	#e poi
	echo "ultima riga:" $tot
	grep -n "^." $1 | sed -n '/^3/p' > file2.txt
	cat file2.txt

}

tre(){
	sed 's/\<più\>/meno/g' $1
}


if [ $# -ne 1 ]
then
	echo "Passami nome file.txt"
	exit 0
fi

while [ 1 ]
do
	echo "1. Nel file passato come parametro, contare il numero di volte che si presenta la parola “più “ e stamparlo a video"
	echo "2. Stampare solo l’ultima riga del testo e porre il tutto in un nuovo file denominato testo2.txt da visualizzare a video "
	echo "3. Sostituire tutte le occorrenze della parola “più “ con la parola “meno” e stampare il risultato a video"
	read ris
	
	if [ "$ris" -eq 1 ]
	then
		uno $1
	
	elif [ "$ris" -eq 2 ]
	then
		due $1

	elif [ "$ris" -eq 3 ]
	then
		tre $1

	else
		echo "Hai inserito risposta sbagliata, riprova!"
		echo ""
	fi
done
exit 0

