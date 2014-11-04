

if [ $# -ne 1 ]
then
	echo "passami il prezzo"
fi

stipendio=1000

if [ $stipendio -lt $1 ]
then
	echo "Non puoi acquistare"
else
	echo "Puoi acquistare"
	if [ $1 -gt 500 ]
	then
		`echo "COSTOSO" > testo.txt`
	else
		`echo "ECONOMICO" > testo.txt`
	fi
	cat testo.txt
	wc -c testo.txt

fi

