#!/bin/sh
funz (){

if [ $# -gt 2 ]
then
	echo "sono stati passati troppi parametri alla funzione"
	return 0;
else
	echo "mi sono stati passati " $# " parametri"
	case $# in
		1) echo "I parametro: " $1;;
		2) echo "I parametro: " $1 " II parametro: " $2;;	
		*) echo "0 parametri";;
	esac
	return 1
fi

}

echo "provo con 0 parametri"
funz 
echo "provo con 1 parametro"
funz ciao
echo "provo con 2 parametri"
funz ciao come 
echo "provo con 3 parametri"
funz ciao come stai
echo "provo a richiamare la funzione e salvo il valore ritornato"
ret=$`funz ciao`
echo "la funzione ha ritornato: " $ret

exit 0
