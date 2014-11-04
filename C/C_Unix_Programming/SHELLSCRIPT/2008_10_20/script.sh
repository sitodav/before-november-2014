#fare nella shell alias ESEGUIRE=./script.sh  in modo tale che scrivendo la parola ESEGUIRE parta lo script
#!/bin/sh

HELP(){
 echo "istruzioni a video di help..."

}

FORMULA(){
	echo "inserisci numero"
	read X
	echo "il valore restituito dalla formula e'" $((($X+10)*2))

}

while [ 1 ]
do
	echo "1)help"
	echo "2)formula"
	read ris
	if [ $ris -eq 1 ]
	then
		HELP
	
	elif [ $ris -eq 2 ]
	then
		FORMULA
	
	else
		echo "Risposta errata"
	fi
done
exit 0
