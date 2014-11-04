#!/bin/sh

#fibonacci di un numero passato

calcola(){
	
	ordine=2
	ordineMax=$1
	pre2=0
	pre1=1
	while [ $ordine -le $ordineMax ]
	do
		echo "pre1" $pre1
		echo "pre2" $pre2
		lastn=$(($pre1+$pre2))
		echo "last:" $lastn
		pre2=$pre1
		
		pre1=$lastn
		
		ordine=$(($ordine+1))
	
	done
	echo "fibonacci di ordine" $1 "e'" $lastn
}
	



if [ $# -ne 1 ]
then
	echo "passami il numero di cui calcolare fibonacci"
	exit 0
fi

echo "calcolo fibonacci di ordine" $1
calcola $1
