#!/bin/bash

##
# 
# Fichier : testTri.sh
# Auteur : Xavier Provençal
# 
# Teste si un programme de tri donne le même résultat que la commande 'sort'.
##

exec="tri"

usage() {
	echo "Usage : testTri n"
	echo "  - 'n'    - Nombre d'éléments à générer."
	#echo "Usage : testTri exec n N"
	#echo "  - 'exec' - Nom de l'exécutable à tester."
	#echo "  - 'n'    - Nombre d'éléments à générer (n=20 par défaut)."
	#echo "  - 'N'    - Valeur max des éléments générés (N=100000000 par défaut)."
	echo ""
}

if [ $# == 0 ]  ; then
	usage
	exit 1
fi

#exec=$1
#shift
if [ $# -ge 1 ] ; then
	n=$1
	shift
else
	n=20
fi
if [ $# -ge 1 ] ; then
	N=$1
	shift
else
	N=100000000
fi


if [ ! -x ./genAleat ] ; then
	if [ -r ./genAleat.c ] ; then 
		gcc -Wall -pedantic genAleat.c -o genAleat
		if [ ! -x ./genAleat ] ; then
			echo "Erreur lors de compilation de genAleat.c"
			exit 1
		fi
	else
		echo "Fichier manquand : genAleat.c"
		exit 1
	fi
fi

echo ""
./genAleat $n $N | tee nbAleat | sort -g > nbTriees
cat nbAleat | ./$exec > aTester
diff -q nbTriees aTester > /dev/null
if [ $? -eq 0 ] ; then
	echo "Trié avec succès"
else 
	echo "Echec du tri"
	echo "  Tri effectuée par '${exec}' : fichier 'aTester' "
	echo "  Tri de la commande 'sort' : fichier 'nbTriees'"
	echo "  Voici les premières valeurs obtenues par '${exec}' :"
	head aTester
	exit 1
fi
exit 0



