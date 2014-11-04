

touch DIZIONARIO
i=0
while [ $i -lt 4 ]
do
	echo "Inserisci nome"
	read nome
	echo $nome >> DIZIONARIO
	i=$(($i+1))
done
sort DIZIONARIO -r 
echo "Chi vuoi cercare ?"
read key
found=`grep -o -c "$key" DIZIONARIO`
if [ $found -eq 1 ]
then
	echo "TROVATO" $key "CON SUCCESSO"
else
	echo "NON TROVATO"
fi

echo "DIZIONARIO OCCUPA N BYTES: " `wc -c DIZIONARIO`
