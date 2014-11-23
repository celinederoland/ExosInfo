#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "pile.h"

/**
	 Initialise la pile de points \a pile. elle contient 0 points initialement, mais
	 peut accueillir jusqu'à 100 points sans être agrandid.

	 @param pile un pointeur vers une structure PilePoints.
*/
void PilePoints_init( PilePoints* pile )
{
	pile->taille = 100;
	pile->nb = 0;
	pile->points = (Point*) malloc( pile->taille * sizeof( Point ) );
}

/**
	 Vérifie si une pile est vide

	 @param pile un pointeur vers une structure PilePoints.
*/
int PilePoints_estVide( PilePoints* pile )
{
	return pile->nb == 0;
}

/**
	 Empile un point au sommet

	 @param pile un pointeur vers une structure PilePoints.
	 @param p un point.
*/
void PilePoints_empile( PilePoints* pile, Point p )
{
	if ( pile->nb < pile->taille )
		pile->points[ pile->nb++ ] = p;
	else{
		PilePoints_agrandir(pile);
		pile->points[ pile->nb++ ] = p;
	}
}

/**
	 Dépile le point au sommet

	 @param pile un pointeur vers une structure PilePoints.
*/
void PilePoints_depile( PilePoints* pile )
{
	assert(pile->nb > 0);
	--(pile->nb);
}

/**
	 Retourne le sommet

	 @param pile un pointeur vers une structure PilePoints.
*/
Point PilePoints_sommet( PilePoints* pile )
{
	assert(pile->nb > 0);
	return pile->points[pile->nb-1];
}

/**
	 Retourne point en dessous du sommet

	 @param pile un pointeur vers une structure PilePoints.
*/
Point PilePoints_deuxiemeSommet( PilePoints* pile )
{
	assert(pile->nb > 1);
	return pile->points[pile->nb-2];
}

/**
	 Agrandit la pile de points

	 @param pile un pointeur vers une structure PilePoints.
*/
void PilePoints_agrandir(PilePoints* pile)
{
	int i;
	pile->taille = pile->taille * 2;
	Point* tabInt = (Point*) malloc( pile->taille * sizeof( Point ) );
	for(i = 0; i < pile->nb; i++){
		tabInt[i] = pile->points[i];
	}
	free(pile->points);
	pile->points = tabInt;
}

/**
	 Libère la mémoire occupée par la pile

	 @param pile un pointeur vers une structure PilePoints.
*/
void PilePoints_termine( PilePoints* pile )
{
	if ( pile->points != NULL ) free( pile->points );
	pile->taille = 0;
	pile->nb = 0;
	pile->points = NULL;
}

/**
	 Renvoie le nombre de points dans la pile

	 @param pile un pointeur vers une structure PilePoints.
*/
int PilePoints_nb( PilePoints* pile )
{
	return pile->nb;
}

/**
	 Renvoie un point d'indice donné de la pile

	 @param pile un pointeur vers une structure PilePoints.
	 @param idx indice du point.
*/
Point PilePoints_get( PilePoints* pile, int idx )
{
	assert ( idx < pile->nb );
	return pile->points[idx];
}
