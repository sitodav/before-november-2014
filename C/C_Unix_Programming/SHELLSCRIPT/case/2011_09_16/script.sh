#!/bin/sh
uno(){
i=0
while [ $i -lt 4 ]
do
	echo "inserire nome di persona.."
	read nome
	echo $nome >> dizionario.txt
	i=$(($i+1))
done

}

due(){
	sort -r dizionario.txt > temp.txt
	cat temp.txt > dizionario.txt
	rm temp.txt
	echo "invertito!"
	cat dizionario.txt
}

tre(){
	echo "inserisci nome da ricercare"
	read key
	occ=`grep -o -c "$key" dizionario.txt`
	if [ $occ -ne 0 ]
	then
		echo "Il nome inserito e' stato trovato !"
	fi

}

quattro(){
	echo "il tot di byte occupati dal file dizionario.txt e'..."
	wc -c dizionario.txt
	echo "il totale di char e'..."
	grep -o -c "." dizionario.txt

}

uno
due
tre
quattro
