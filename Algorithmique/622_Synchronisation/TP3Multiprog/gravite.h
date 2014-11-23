#ifndef __GRAVITE__H__
#define __GRAVITE__H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>

/* Exercice n°1 */
/*
Notre stratégie pour gérer les fusions entre deux corps est d'utiliser un mutex qui protège l'appel de fusionnerCorps.
Pour la divison par zéro on se contente de vérifier que la masse n'est pas nulle.
*/
/* Exercice n°2 */
/*

*/
/* Exercice n°3 */
/*

*/


/* Constante gravitationnelle exprimée en :	m^3 / ( kg * s^2 ) */
#define G 6.67384e-11

#define NBTHREADS 2


typedef long double Flottant;
#define ZERO 0.0L


typedef struct 
{
	Flottant x;
	Flottant y;
} Point;

typedef struct 
{
	Flottant x;
	Flottant y;
} Vecteur;

typedef struct 
{
	int nb_corps;
	int nb_collisions;
	Flottant * lesMasses;
	Point * posActuelles;
	Vecteur * vitesses;
	Point * nouvellesPos;
	Flottant delta_t;

	Flottant dist_min;

	Flottant max_x;
	Flottant max_y;

	pthread_mutex_t mutexFusion;

	pthread_t* tabThreads;
	sem_t arreteSimu;
	sem_t corpsTraite;
	int refTube[2];
} Systeme;
 


/**
 * Initialise un système gravitationnel à partir d'un fichier.
 *
 * Paramètres :
 *	- S - Le système à initialiser
 *	- fichier - nom du fichier source
 */
void initSysteme(	Systeme * S, const char * fichier );

/**
 * Effectue une étape de la simulation du système ``S``.
 *
 * Paramètres :
 *	- S - Le système gravitationnel
 */
void simule_un_pas ( Systeme * S, Flottant delta_t );

void threadAction( void* d);

/**
 * Détruit un système gravitationnel en libérant les ressources
 */
void detruireSysteme( Systeme * S );

/**
 * Affiche le système dans la console, au même format que le fichier source.
 */
void afficherSysteme( Systeme * S);

/**
 * Indique la distance minimale entre deux corps, en dessous de cette distance
 * on considère qu'il y a collision.
 */
void setDistMin ( Systeme * S, Flottant dist_min );

#endif

