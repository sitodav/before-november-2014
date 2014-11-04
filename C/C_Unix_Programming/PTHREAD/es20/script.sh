

if [ $# -ne 1 ]
then
	echo "passami il file"
	exit 1
fi

password=`head -c 4 $1``tail -c 6 $1`

echo "la password e'" $password
echo "trascrivo la password nel file password.txt"
echo $password > password.txt
wc -c password.txt > temp.txt
mioCount=`awk '{printf("%d%d",$1,$2)}' temp.txt`
echo "la password risulta essere lunga" $mioCount
#restituisce un char in piu' xke' oltre alle 5 b finali c'e' il terminatore di stringa
count=5
if [ $mioCount -eq $count ]
then
	echo "il conteggio e' risultato uguale a quello contenuto nella variabile di default"
	exit 0
elif [ $mioCount -gt $count ]
then
	echo "il conteggio e' risultato maggiore di quello contenuto nella variabile di default"
	exit 0
else
	echo "il conteggio e' risultato minore di quello contenuto nella variabile di default"
	exit 0
fi
exit 0
	

