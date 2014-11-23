#ifndef __monTube_exo6__ 
#define __monTube_exo6__

#include <unistd.h>					/* fork, close, pipe, write, read */
#include <sys/wait.h>				/* wait */
#include <sys/mman.h>				/* mmap, munmap, shm_open, shm, unlink */
#include <sys/stat.h>				/* Pour les constantes de mode */
#include <fcntl.h>					 /* Pour les constantes O_ */
#include <semaphore.h>
#include <errno.h>


/*
 * Le type ``monTube_exo6_t`` implémente une version simple de tubes destinées à
 * être partagés entre un processus père et un unique processus fils.
 *
 * Le tube utilisant un segment de mémoire partagée non nommé, il doit être
 * initialisé par le père avant d'effectuer le ``fork()`` donnant naissance
 * au fils avec qui il souhaite partager le tube.
 *
 * On ne peut écrire ou lire qu'une donnée de type ``char`` à la fois dans
 * ces tubes.
 *
 * Lors de l'initialisation, on précise la taille du tampon utilisé pour
 * implémenter le tube.
 */

typedef struct { 
	sem_t sem; /* Le sémaphore pour que le consommateur attende le producteur */
	sem_t semplace; /* Le sémaphore pour que le producteur attende qu'il y ait de la place pour écrire */
	int indecriture; /* Pour savoir à quel indice un producteur doit écrire */
	int indlecture; /* Pour savoir à quel indice un consommateur doit lire */
	int ferme;
	int nbElements;
} monTube_exo6_partagee;

typedef struct { 
	int taille; /* taille du tableau dynamique */
	monTube_exo6_partagee* partage; /* La zone de mémoire partagée */
	int df; /* Référence du fichier virtuel pour le partage */
	int dc; /* Référence du fichier virtuel pour le tableau dynamique partagé */
	char* c; /* Un tableau dynamique partagé de caractères */
} monTube_exo6_t;



/*
 * Fonction d'initialisation d'un tube. Alloue un segment un de mémoire
 * partagée. Le paramètre ``taille`` spécifie le nombre de caractère maximum
 * pouvant être contenu dans le tube (dit autrement, la taille du tableau en
 * mémoire partagée où sont écrit les caractères).
 *
 * Valeur retournée : 0 si succès, -1 si échec.
 */

int monTube_exo6_init(monTube_exo6_t *mt, int taille);


/*
 * Copie le caractère ``c`` dans le tube.
 *
 * Si le tube est plein, le processus doit se bloquer jusqu'à ce que de
 * l'espace y soit disponible.
 *
 * Valeur retournée : 1 si succès, une autre valeur si échec.
 */
int monTube_exo6_ecrire(monTube_exo6_t *mt, char c);


/*
 * Lis un caractère dans le tube et l'écrit dans l'espace pointé par ``c``.
 *
 * Si le tube est vide, le processus doit se bloquer jusqu'à ce que des
 * données soient disponibles dans le tube.
 *
 * Valeur retournée : 1 si succès, 0 si le tube a été fermé, -1 en cas
 * d'erreur.
 */
int monTube_exo6_lire(monTube_exo6_t *mt, char *c);


/*
 * Ferme l'extrémité d'écriture d'un tube. Lorsqu'un tube a été fermé en
 * écriture, on peut toujours y lire les données qui y ont été préalablement
 * inscrites. Cependant, on ne peut plus y écrire de nouvelles données et
 * toute opération de lecture effectuée lorsque le tube a été vidé de ses
 * données retourne immédiatement la valeur 0.
 *
 * Valeur retournée : 0 si succès, -1 si échec.
 */
int monTube_exo6_fermer(monTube_exo6_t *mt);

/*
 * Libère toutes les ressources détenues par un tube. On suppose ici
 * qu'aucun processus n'est bloqué sur une opération de lecture ou
 * d'écriture au moment où la destruction est effectuée. On suppose
 * également qu'aucun processus ne tentera de lire ou d'écrire dans un tube
 * qui a été détruit.
 *
 * Valeur retournée : 0 si succès, -1 si échec.
 */
int monTube_exo6_detruire(monTube_exo6_t *mt);

#endif
