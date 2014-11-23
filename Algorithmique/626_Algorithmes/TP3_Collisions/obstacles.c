#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include "obstacles.h"

void initObstacle( Obstacle* p, ObstacleType type, double x, double y, double r, double att, double cr, double cg, double cb )
{
	p->x[0] = x;
	p->x[1] = y;
	p->r = r;
	p->att = att;
	p -> cr = cr;
	p -> cg = cg;
	p -> cb = cb;
	p -> type = type;
}

void TabObstacles_init( TabObstacles* tab )
{
	tab->taille = 10;
	tab->nb = 0;
	tab->obstacles = (Obstacle*) malloc( tab->taille * sizeof( Obstacle ) );
}

void TabObstacles_ajoute( TabObstacles* tab, Obstacle p )
{
	if ( tab->nb == tab->taille )
		TabObstacles_agrandir( tab );
	tab->obstacles[ tab->nb++ ] = p;
}

void TabObstacles_set( TabObstacles* tab, int i, Obstacle p )
{
	assert ( i < tab->nb );
	tab->obstacles[ i ] = p;
}

Obstacle TabObstacles_get( TabObstacles* tab, int i )
{
	assert ( i < tab->nb );
	return tab->obstacles[ i ];
}

Obstacle* TabObstacles_ref( TabObstacles* tab, int i )
{
	assert ( i < tab->nb );
	return tab->obstacles + i;
}

int TabObstacles_nb( TabObstacles* tab )
{
	return tab->nb;
}

void TabObstacles_termine( TabObstacles* tab )
{
	if ( tab->obstacles != NULL ) free( tab->obstacles );
	tab->taille = 0;
	tab->nb = 0;
	tab->obstacles = NULL;
}

void TabObstacles_agrandir( TabObstacles* tab )
{
	int new_taille = 2*tab->taille;
	Obstacle* new_pts = (Obstacle*) malloc( new_taille * sizeof( Obstacle ) );
	for ( int i = 0; i < tab->nb; ++i )
		new_pts[ i ] = tab->obstacles[ i ];
	free( tab->obstacles );
	tab->obstacles = new_pts;
	tab->taille = new_taille;
}

/**
   Trouve l'indice de la médiane du tableau
*/
int TabObstacles_mediane( Obstacle* tab, int deb, int fin, int a )
{

	if (fin - deb <= 2) { return deb; }
	int i,j,cptInf,cptSup;
	cptInf = 0; cptSup = 0;
	for (i = deb; i < fin; i++)
	{
		for (j = deb; j < fin; j++)
		{
			if (tab[j].x[a] <= tab[i].x[a]) { cptInf++; }
			if (tab[j].x[a] >= tab[i].x[a]) { cptSup++; }
		}
		if (cptInf >= (fin - deb)/2 && cptSup >= (fin - deb)/2) { return i; }
	}
	assert(0);
	return -1;
}

/**
   Echange deux éléments
*/
void TabObstacles_echange( Obstacle* tab, int i, int j )
{
	Obstacle tmp = tab[i];
	tab[i] = tab[j];
	tab[j] = tmp;
}

/**
   Partitionne les éléments du tableau autour de la médiane
*/
void TabObstacles_partition( Obstacle* tab, int deb, int fin, int a )
{
	int med = TabObstacles_mediane( tab, deb, fin, a );
	//assert(med >= deb);
	TabObstacles_echange(tab,med,fin - 1);
	int j = deb;
	for (int i = deb; i < fin - 1; i++)
	{
		if (tab[i].x[a] <= tab[fin - 1].x[a]) 
		{ 
			TabObstacles_echange(tab,i,j);
			j++;
		}
	}
	TabObstacles_echange(tab,j,fin - 1);
}

/*void TabObstacles_supprime_dernier( TabObstacles* tab )
{
	assert( tab->nb > 0 );
	--tab->nb;
}

void TabObstacles_supprime( TabObstacles* tab, int i )
{
	assert ( i >= 0 );
	assert ( i < tab->nb );
	tab->obstacles[ i ] = tab->obstacles[ --tab->nb ];
}*/


