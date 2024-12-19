#!/bin/sh

# Fonction pour dessiner un sapin
dessiner_sapin() {
	hauteur=$1
	# Dessiner les branches du sapin
	i=0
	while [ $i -lt $hauteur ]; do
		# Calculer le nombre d'espaces et d'étoiles pour chaque ligne
		espaces=""
		j=0
		while [ $j -lt $(($hauteur - $i - 1)) ]; do
			espaces="$espaces "
			j=$(($j + 1))
		done
		etoiles=""
		k=0
		while [ $k -lt $((2 * i + 1)) ]; do
			etoiles="$etoiles*"
			k=$(($k + 1))
		done
		# Afficher les espaces suivis des étoiles
		echo "$espaces$etoiles"
		i=$(($i + 1))
	done

	# Dessiner le tronc du sapin
	tronc=""
	l=0
	while [ $l -lt $(($hauteur - 1)) ]; do
		tronc="$tronc "
		l=$(($l + 1))
	done
	tronc="$tronc|"
	j=0
	while [ $j -lt 2 ]; do
		echo "$tronc"
		j=$(($j + 1))
	done
}

# Vérification de l'argument
if [ -z "$1" ]; then
	hauteur_sapin=5  # Hauteur par défaut si aucun argument n'est passé
else
	hauteur_sapin=$1  # Hauteur spécifiée par l'utilisateur
fi

# Appeler la fonction pour dessiner un sapin de Noël
dessiner_sapin $hauteur_sapin
