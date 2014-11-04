#/bin/sh!
if [ $# -ne 1 ]
then
	echo "passami nome file!"
	exit 0
fi

sed '/^I/d' $1 > file2.txt
sed 's/Sign/Ing/g' $1 > file3.txt
awk '{printf("%s\n",$2)}' $1 > temp.txt
cat temp.txt 
sort -r temp.txt
rm temp.txt
