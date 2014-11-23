#ifndef _OBSTACLES_H_
#define _OBSTACLES_H_

#include "points.h"

/// Représente un obstacle.
typedef enum { DISQUE } ObstacleType;
typedef struct SObstacle {
	ObstacleType type; //< Le type de l'obstacle. Ici on aura juste des DISQUEs.
	double x[ DIM ]; //< Les coordonnées du centre de l'obstacle.
	double r; //< Le rayon de l'obstacle
	double att; //< le facteur d'atténuation de l'obstacle (0.0 amortisseur parfait, 1.0 rebondisseur parfait, 3.0 "bumper" comme dans un flipper.)
	double cr, cg, cb; //< couleurs rgb de l'obstacle.
} Obstacle;

/**
   Initialise l'obstacle \a p 
*/
void initObstacle( Obstacle* p, ObstacleType type, double x, double y, double r, double att, double cr, double cg, double cb );

/// Représente un tableau dynamique d'obstacles.
typedef struct STabObstacles {
  int taille;
  int nb;
  Obstacle* obstacles;
} TabObstacles;

/**
   Initialise le tableau d'obstacles \a tab. Il contient 0 obstacles initialement, mais
   peut accueillir jusqu'à 10 obstacles sans être agrandi.

   @param tab un pointeur vers une structure TabObstacle.
*/
void TabObstacles_init( TabObstacles* tab );

/**
   Ajoute si possible l'obstacle \a p à la fin du tableau de obstacles \a tab.
   
   @param tab  un pointeur vers une structure TabObstacle valide.
   @param p une obstacle.
*/
void TabObstacles_ajoute( TabObstacles* tab, Obstacle p );

/**
   Modifie le \a i-ème obstacle du tableau d'obstacle \a tab. Il devient
   l'obstacle \a p.
   
   @param tab  un pointeur vers une structure TabObstacle valide.
   @param i un index valide (entre 0 et TabObstacles_nb( tab ) )
   @param p un obstacle.
*/
void TabObstacles_set( TabObstacles* tab, int i, Obstacle p );

/**
   @param tab un pointeur vers une structure TabObstacle valide.
   @param i un index valide (entre 0 et TabObstacles_nb( tab ) )
   @return le \a i-ème obstacle du tableau de points \a tab.   
*/
Obstacle TabObstacles_get( TabObstacles* tab, int i );

/**
   @param tab  un pointeur vers une structure TabObstacle valide.
   @param i un index valide (entre 0 et TabObstacles_nb( tab ) )
   @return un pointeur vers le \a i-ème obstacle du tableau de points \a tab.   
*/
Obstacle* TabObstacles_ref( TabObstacles* tab, int i );

/**
   @param tab  un pointeur vers une structure TabObstacle valide.
   @return le nombre de points utiles stockés dans le tableau de points \a tab.   
*/
int TabObstacles_nb( TabObstacles* tab );

/**
   Indique que le tableau de points \a tab n'est plus utilisé et
   libère la mémoire associée. Il passe à une taille 0.
  
   @param tab  un pointeur vers une structure TabObstacle valide.
 */
void TabObstacles_termine( TabObstacles* tab );

/**
   Utilisé en interne. Agrandit automatiquement le tableau si nécessaire.
*/
void TabObstacles_agrandir( TabObstacles* tab );

/**
   Trouve l'indice de la médiane du tableau
*/
int TabObstacles_mediane( Obstacle* tab, int deb, int fin, int a );

/**
   Echange deux éléments
*/
void TabObstacles_echange( Obstacle* tab, int i, int j );
/**
   Partitionne les éléments du tableau autour de la médiane
*/
void TabObstacles_partition( Obstacle* tab, int deb, int fin, int a );

#endif
