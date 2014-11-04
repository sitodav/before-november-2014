#Si componga uno script che realizzi le seguenti operazioni :
#1. Conti il numero di righe del file out.txt e stampi a video il risultato
#2. Stampi, relativamente al file out.txt, soltanto il campo Nome e la Città, tralasciando i #rimanenti
#3. Sostituisca le linee contenenti la parola “Commerciante” con la parola “Nulla”
#Si chiede all’utente di attuare il passaggio di parametri per il file su cui operare e di #inviare un messaggio
#a video se dovesse risultare mancante. Il contenuto del file out.txt è il seguente:





if [ $# -ne 1 ]
then
	echo "occorre passare in cmd line nome file.txt\n"
	exit 0
fi
echo "il numero di righe e'..."
wc -l $1
echo ""
awk '{printf("%s %s\n",$1,$4)}' $1
sed -i 's/Commerciante/Nulla/g' $1
