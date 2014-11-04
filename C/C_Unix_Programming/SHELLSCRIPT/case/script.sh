#!/bin/sh
echo "Come ti chiami ?"
read nome
case "$nome" in
A*) echo "Il tuo nome inizia per A";;
B*) echo "Il tuo nome inizia per B";;
*) echo "Il tuo nome" $nome "non e' riconosciuto";;
esac
exit 0
