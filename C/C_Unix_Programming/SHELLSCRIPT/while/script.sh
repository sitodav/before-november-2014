#!/bin/sh
n=1; #va inizializzata altrimenti da problemi
while [ "$n" -ne 0 ] 
do
echo "inserisci numero...0 per uscire"
read n
echo "hai inserito" $n
done
exit 0
