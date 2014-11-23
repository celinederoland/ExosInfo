#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "points.h"
#include <math.h>

void TabPoints_init( TabPoints* tab )
{
	tab->taille = 100;
	tab->nb = 0;
	tab->points = (Point*) malloc( tab->taille * sizeof( Point ) );
}

void TabPoints_agrandir( TabPoints* tab )
{
	int i;
	tab->taille = tab->taille * 2;
	Point* tabInt = (Point*) malloc( tab->taille * sizeof( Point ) );
	for(i = 0; i < tab->nb; i++){
		tabInt[i] = tab->points[i];
	}
	free(tab->points);
	tab->points = tabInt;
}

void TabPoints_ajoute( TabPoints* tab, Point p )
{
	if ( tab->nb < tab->taille )
		tab->points[ tab->nb++ ] = p;
	else{
		TabPoints_agrandir(tab);
		tab->points[ tab->nb++ ] = p;
	}
}

void TabPoints_set( TabPoints* tab, int i, Point p )
{
	assert ( i < tab->nb );
	tab->points[ i ] = p;
}

Point TabPoints_get( TabPoints* tab, int i )
{
	assert ( i < tab->nb );
	return tab->points[ i ];
}

int TabPoints_nb( TabPoints* tab )
{
	return tab->nb;
}

void TabPoints_termine( TabPoints* tab )
{
	if ( tab->points != NULL ) free( tab->points );
	tab->taille = 0;
	tab->nb = 0;
	tab->points = NULL;
}

int TabPoints_indexBasGauche( TabPoints* tab )
{
	assert(tab -> nb > 0);
	int indmin = 0;
	int i;
	for(i = 0; i < tab->nb; i++){
		Point p = tab->points[i];
		if(p.y < tab->points[indmin].y){
			indmin = i;
		}
		else if(p.y == tab->points[indmin].y){
			if(p.x < tab->points[indmin].x){
				indmin = i;
			}
		}
	}
	return indmin;
}

int TabPoints_indexHaut(TabPoints* tab)
{
	int indmax = 0;
	int i;

	for(i = 0; i < tab->nb; i++) 
	{
		Point p = tab->points[i];
		if(p.y > tab->points[indmax].y) 
			indmax = i;
		else if(p.y == tab->points[indmax].y)
		{
			if(p.x > tab->points[indmax].x)
				indmax = i;
		}
	}

	return indmax;
}

int cmpfunc(const void * pPrem, const void * pDeux)
{
	Point p1 = *(Point*)pPrem;
	Point p2	= *(Point*)pDeux;
	double det = - (p1.x*p2.y - p1.y*p2.x);
	if (det > 0) { return 2; }
	else if (det < 0) { return 0; }
	else { return -2; }
}

void affiche(TabPoints* tab)
{
	int i;
	Point P;
	
	for (i = 0; i < TabPoints_nb(tab); i++)
	{
		P = TabPoints_get(tab,i);
		printf("point %i (%f,%f)\n", i, P.x, P.y);
	}
}

void TabPoints_triSelonT0(TabPoints* tab)
{
	int i;
	Point NouvPoint,AncienPoint;
	Point T0 = TabPoints_get(tab,0);
	
	//On translate tous les points du vecteur -T0 pour se ramener à l'origine du repère
	for (i = 1; i < TabPoints_nb(tab); i++)
	{
		AncienPoint = TabPoints_get(tab,i);
		NouvPoint.x = AncienPoint.x - T0.x;
		NouvPoint.y = AncienPoint.y - T0.y;
		TabPoints_set(tab,i,NouvPoint);
	}

	//On effectue le tri
	qsort((tab->points)+1, TabPoints_nb(tab) - 1, sizeof(Point), cmpfunc);

	//On translate tous les points du vecteur T0 pour se ramener à la situation initiale
	for (i = 1; i < TabPoints_nb(tab); i++)
	{
		AncienPoint = TabPoints_get(tab,i);
		NouvPoint.x = AncienPoint.x + T0.x;
		NouvPoint.y = AncienPoint.y + T0.y;
		TabPoints_set(tab,i,NouvPoint);
	}
}

int EstAGauche(Point p1, Point p2, Point p)
{
	double det = (p1.x - p.x)*(p2.y - p.y) - (p1.y - p.y)*(p2.x - p.x);
	return det > 0;
}

int EstADroite(Point p1, Point p2, Point p)
{
	double det = (p1.x - p.x)*(p2.y - p.y) - (p1.y - p.y)*(p2.x - p.x);
	return det < 0;
}

void Echange(int i, int j, TabPoints* tab)
{
	assert(i < tab -> nb);
	assert(j < tab -> nb);
	Point pInt = tab->points[i];
	tab->points[i] = tab->points[j];
	tab->points[j] = pInt;
}

int ChercheAngleFaible(TabPoints* tab, int indPointInitial, int indPointFinal)
{
	Point p1, p2, p;
	int i, indPolaireFaible;
	p = TabPoints_get(tab, indPointInitial);
	// Initialisation de l'angle le plus faible
	p1 = TabPoints_get(tab, indPointFinal);
	indPolaireFaible = indPointFinal;
	for(i = 1; i < TabPoints_nb(tab); i++){
		p2 = TabPoints_get(tab, i);
		if(EstADroite(p1, p2, p)){
			p1 = p2;
			indPolaireFaible = i;
		}
	}
	return indPolaireFaible;
}

int ChercheAngleEleve(TabPoints* tab, int indPointInitial, int indPointFinal)
{
	Point p1, p2, p;
	int i, indPolaireEleve;
	p = TabPoints_get(tab, indPointInitial);
	// Initialisation de l'angle le plus eleve
	p1 = TabPoints_get(tab, indPointFinal);
	indPolaireEleve = indPointFinal;
	for(i = 1; i < TabPoints_nb(tab); i++){
		p2 = TabPoints_get(tab, i);
		if(EstAGauche(p1, p2, p)){
			p1 = p2;
			indPolaireEleve = i;
		}
	}
	return indPolaireEleve;
}
