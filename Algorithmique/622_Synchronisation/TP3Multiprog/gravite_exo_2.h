#ifndef __GRAVITE__H__
#define __GRAVITE__H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>

/**
 * INFO 622 TP3
 * De Roland Céline et Leclaire Juliana
 * L3-STIC-INFO-3-2
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

	pthread_mutex_t mutexFusion; /* Pour gérer les fusions des corps */

	sem_t travailleSimu; /* Indique aux threads de travailler */
	sem_t arreteSimu; /* Indique que tous les threads ont fini */

	pthread_t* tabThreads; /* Les threads qui vont travailler */

	int arrete;
} Systeme;

typedef struct 
{
	int finIntervalle;
	int debIntervalle;
	Systeme* systeme;
} Data;
 


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

