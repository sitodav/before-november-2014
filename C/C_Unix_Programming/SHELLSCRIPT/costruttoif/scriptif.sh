#!/bin/sh
echo "inserisci città"
read citta
if [ "$citta"="napoli" ]
then 
	echo "sei napoletano"
elif [ "$citta" = "bari" ]
	then
		echo "sei barese"
else
	echo "di dove cazzo sei?"
fi
