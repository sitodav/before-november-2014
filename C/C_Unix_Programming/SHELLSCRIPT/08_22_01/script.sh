#!/bin/sh





if [ $# -ne 1 ]
then
	echo "passami file!"
	exit 0
fi

sed '/^D/d' $1 > file1.txt
sed 's/Avv/Ins/' $1 > file2.txt
awk '{printf("%s ",$3)}' $1 
echo "\n"
awk '{printf("%s\n",$3)}' $1 | sort -r #andiamo a capo qua nell'estrazione perche' cosi' funziona il sort in quanto e' fatto su righe diverse



