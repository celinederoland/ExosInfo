#ifndef __monTube_exo4__ 
#define __monTube_exo4__

#include <unistd.h>					/* fork, close, pipe, write, read */
#include <sys/wait.h>				/* wait */
#include <sys/mman.h>				/* mmap, munmap, shm_open, shm, unlink */
#include <sys/stat.h>				/* Pour les constantes de mode */
#include <fcntl.h>					 /* Pour les constantes O_ */
#include <semaphore.h>
#include <errno.h>


/*
 * Le type ``monTube_exo4_t`` implémente une version simple de tubes destinés à
 * être partagés entre un processus père et un unique processus fils.
 *
 * Le tube utilisant un segment de mémoire partagée non nommé, il doit être
 * initialisé par le père avant d'effectuer le ``fork()`` donnant naissance
 * au fils avec qui il souhaite partager le tube.
 *
 * On ne peut écrire ou lire qu'une donnée de type ``char`` à la fois dans
 * ces tubes.
 */

/* nombre maximal de caractères pouvant être contenus dans le tube */
#define TAILLE_TUBE 4

typedef struct { 
	sem_t sem; /* Le sémaphore pour que le consommateur attende le producteur */
	sem_t semplace; /* Le sémaphore pour que le producteur attende qu'il y ait de la place pour écrire */
	char c[TAILLE_TUBE]; /* Un tableau de caractères */
	int indecriture; /* Pour savoir à quel indice un producteur doit écrire */
	int indlecture; /* Pour savoir à quel indice un consommateur doit lire */
} monTube_exo4_partagee;

typedef struct { 
	monTube_exo4_partagee* partage; /* La zone de mémoire partagée */
	int df; /* Référence du fichier virtuel pour le partage */
} monTube_exo4_t;



/*
 * Fonction d'initialisation d'un tube. Alloue un segment de mémoire
 * partagée. 
 *
 * Valeur retournée : 0 si succès, -1 si échec.
 */

int monTube_exo4_init(monTube_exo4_t *mt);


/*
 * Copie le caractère ``c`` dans le tube.
 *
 * Si le tube est plein, le processus doit se bloquer jusqu'à ce que de
 * l'espace y soit disponible.
 *
 * Valeur retournée : 1 si succès, une autre valeur si échec.
 */
int monTube_exo4_ecrire(monTube_exo4_t *mt, char c);


/*
 * Lit un caractère dans le tube et l'écrit dans l'espace pointé par ``c``.
 *
 * Si le tube est vide, le processus doit se bloquer jusqu'à ce que des
 * données soient disponibles dans le tube.
 *
 * Valeur retournée : 1 si succès, 0 si le tube a été fermé, -1 en cas
 * d'erreur.
 */
int monTube_exo4_lire(monTube_exo4_t *mt, char *c);


/*
 * Libère toutes les ressources détenues par un tube. On suppose ici
 * qu'aucun processus n'est bloqué sur une opération de lecture ou
 * d'écriture au moment où la destruction est effectuée. On suppose
 * également qu'aucun processus ne tentera de lire ou d'écrire dans un tube
 * qui a été détruit.
 *
 * Valeur retournée : 0 si succès, -1 si échec.
 */
int monTube_exo4_detruire(monTube_exo4_t *mt);

#endif
