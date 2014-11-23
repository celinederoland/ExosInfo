/*
 * TP Tri Rapide info 622. Juliana Leclaire, Céline de Roland (version finale)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include "triRapide.h"

/**
 * Fichier : triRapide.c
 *
 * Implémentation de l'algorithme du tri rapide pour trier un tableau de
 * nombres.
 */


#define NBWORKERS 3
#define LIMITE_INF 100 

/***********************************************************
 * Plan et entêtes de fonctions :                          *
 ***********************************************************/

	/* Exercice 0 */

inline 
void echanger(int * x, int * y);
void partitionner ( tabNombres tn, int * pivot );
void triRapide ( tabNombres tn );


	/* Exercice 1 */

void triRapideMT( tabNombres tn); /* tri rapide multi-threads */
void triRapideMTthread( tabNombres * tn ); /* fonction utilisée par un thread pour commencer son travail */

/* triRapideMT ne fait qu'appeler triRapideMTthread
 * triRapideMTthread partitionne le tableau, puis confie la partie gauche à un nouveau thread et s'occupe de la partie droite 
 * il y a donc un nouveau thread à chaque partitionnement -> le nombre de threads qui travaillent est de l'ordre de la taille du tableau (il y en a donc beaucoup trop dès que le tableau atteind une certaine taille) */


	/* Exercice 2 */

		/* Manipulation des piles : nous avons ajouté un sémaphore pour éviter l'attente active et un mutex pour éviter les conditions de concurrence */
typedef struct /* Structure utilisée pour représenter une pile (respecte le principe FILO) de tableaux de nombres (struct tabNombres). */
{
	tabNombres * tab;
	sem_t nbElem;
	int sommet;
	pthread_mutex_t protecteur;
	pthread_mutex_t protecteurNbTries;
	int tailleTab; /* Taille du tableau utilisé pour stocker les éléments */
	int nbElemTries;
	int nbElemAtrier;
} Pile;
void initPile( Pile * p );
int estVide( Pile * p );
void depile( Pile * p, tabNombres * val);
void agrandirPile( Pile *p);
void empile( Pile * p, tabNombres val);

		/* TriRapideFT */ 
void triRapideFT( tabNombres tn ); /* tri rapide avec un nombre fixé de thread */
void worker(Pile* p); /* fonction utilisée par les thread */

/* triRapideFT empile le tableau initial, appelle les workers puis les attend 
 * chaque worker boucle sur l'action suivante : dépile un tableau, le partitionne et empile les 2 tableaux obtenus s'ils ont plus d'un élément
 * on compte au passage le nombre d'éléments triés (un élément est trié si c'est un pivot ou si il est dans un tableau d'un seul élément qui ne sera pas réempilé) pour savoir quand s'arrêter.
 */

	/* Exercice 3 */

void triInsertion(tabNombres tn);
void shellPhase(tabNombres tn, int pas);
void triShell(tabNombres tn);
void worker_bis(Pile* p);

/* On utilise le même principe que dans l'exercice 2, sauf qu'arrivés à 100 éléments ou moins on arête la récursion pour passer soit au tri par insertion soit au tri de Shell. D'après nos tests, nous avons les ratios suivants (5 tests sur des tableaux de taille 1 000 000) :
* tpsTriInsertion/tpsSort = 0,577
* tpsTriShell/tpsSort = 0,569
* rationTriShellSort/ratioTriInsertionSort = 0,98. */

/***********************************************************
 * Lignes à commenter/décommenter :                        *
 ***********************************************************/

	/* ligne 323 : choix du triFT de l'exercice 2 ou de l'exercice 3 */
	/* ligne 487 et 510 : choix entre triInsertion et triShell */ 

/***********************************************************
 * Exercice 0                                              *
 ***********************************************************/

	/**
	 * Échange les valeurs des entiers pointés par 'x' et 'y'.
	 */
	inline 
	void echanger(int * x, int * y)
	{
		int tmp;
		tmp = *x;
		*x = *y;
		*y = tmp;
	}

	/**
	 * Effectue une partition du talbeau de nombres 'tn' en utilisant la dernière
	 * valeur du tableau comme pivot.
	 * Toutes les valeurs plus petites que la valeur pivot sont placés dans la

	 * moitiée droites et les plus grandes dans la moitié gauche.
	 *
	 * Inscrit la position finale de la valeur pivot à l'endroit pointé par le
	 * paramètre 'pivot'.
	 */
	void partitionner ( tabNombres tn, int * pivot )
	{
		int i, j, valPivot;
		j = tn.debut;
		valPivot = tn.val [ tn.fin - 1 ];
		for ( i=tn.debut; i < tn.fin-1; ++i )
		  {
		    if ( tn.val[i] < valPivot ) 
		{
			echanger ( &tn.val[i] , &tn.val[j] );
			++j;
		}
		  }
		echanger( &tn.val[j], &tn.val[ tn.fin - 1] );
		*pivot = j;
	}

	/**
	 * Tri le tableau 'tn' à l'aide de l'algorithme du tri rapide.
	 */
	void triRapide ( tabNombres tn )
	{
		tabNombres gauche, droite;
		int pivot;
		partitionner ( tn, &pivot );
		/* On calcule la taille du sous tableau gauche, si >1 alors récursion */
		if ( pivot - tn.debut > 1 )
		  {
		    gauche.debut = tn.debut;
		    gauche.fin = pivot;
		    gauche.val = tn.val;
		    triRapide( gauche );
		  }
		/* On calcule la taille du sous tableau droit, si >1 alors récursion */
		if ( tn.fin - (pivot + 1) > 1 )
		  {
		    droite.debut = pivot + 1;
		    droite.fin = tn.fin;
		    droite.val = tn.val;
		    triRapide( droite );
		  }
	}

/***********************************************************
 * Exercice 1                                              *
 ***********************************************************/

	void triRapideMT( tabNombres tn)
	{
		/* On appelle une autre fonction qui prend en argument un pointeur de façon à pouvoir utiliser les thread */
		triRapideMTthread(&tn);
	}

	void triRapideMTthread( tabNombres * tn )
	{
		tabNombres gauche, droite;
		int pivot; 
		pthread_t TG;
		int result; /* résultat renvoyé par pthread_create (vaudra 0 uniquement si on a utilisé un thread) */
		result = 1;
		partitionner ( *tn, &pivot );

		/* On calcule la taille du sous tableau gauche, si >1 alors récursion */
		if ( pivot - tn -> debut > 1 )
		{
			gauche.debut = tn -> debut;
			gauche.fin = pivot;
			gauche.val = tn -> val;

			/* on demande à un thread de s'occuper de la partie gauche */ 
			result = pthread_create(&TG, NULL, (void *(*)(void *)) triRapideMTthread, (void *) &gauche);

			/* par conséquent, le nombre de threads double à chaque étape : on arrive rapidement à un nombre de threads très important. En réalité, à la fin de l'exécution le nombre de thread est du même ordre de grandeur que le nombre d'éléments du tableau ... */
		}

		/* On calcule la taille du sous tableau droit, si >1 alors récursion */
		if ( tn -> fin - (pivot + 1) > 1 )
		{
			droite.debut = pivot + 1;
			droite.fin = tn -> fin;
			droite.val = tn -> val;

			/* le thread courant s'occupe de la partie droite */
			triRapideMT(droite);
		}

		/* on attend le thread qui s'occupe de la partie gauche (s'il existe) */
		if (result == 0) { pthread_join(TG,NULL); }
	}



/***********************************************************
 * Exercice 2                                              *
 ***********************************************************/


	/***********************************************************
	 * Les fonctions qui suivent servent à la manipulation des *
	 * piles de 'tabNombres'.                                  *
	 ***********************************************************/

		/**
		 * Initilise 'p' en une pile vide.
		 */
		void initPile( Pile * p )
		{
			p->nbElemTries = 0;
			p->tailleTab = 1;
			p->sommet = -1;
			p->tab = malloc( p->tailleTab * sizeof (tabNombres) );
			sem_init( &(p -> nbElem), 0, 0 ); 
			pthread_mutex_init( &(p -> protecteur), NULL );
			pthread_mutex_init( &(p -> protecteurNbTries), NULL );
		}


		/**
		 * Retourne 0 si 'p' contient au moins un élément, une autre valeur sinon.
		 */
		int estVide( Pile * p )
		{
			return (p->sommet == -1);
		}


		/**
		 * Retire le premier élément de la Pile 'p' et l'incrit dans 'tn'.
		 *
		 * Note : si la pile 'p' est vide, l'appel est bloquant jusqu'à ce qu'un élément
		 * y soit inséré.
		 */
		void depile( Pile * p, tabNombres * val)
		{
			/* Attention bien faire un sem_wait avant un mutex pour laisser les autres proc travailler et eviter les interblocages */
			sem_wait(&(p -> nbElem));
			pthread_mutex_lock(&(p -> protecteur));
			*val = p->tab [ p->sommet ];
			--(p->sommet);
			pthread_mutex_unlock(&(p -> protecteur));

		}

		/**
		 * Double la taille du tableau de la pile.
		 *
		 * Alloue un nouveau tableau de deux fois la taille du premier, y copie son
		 * contenu et libère la mémoire occupée par le premier.
		 *
		 * ATTENTION : 'agrandirPile' ne devrait être appelée que par la fonction
		 *             'empile'.
		 */
		void agrandirPile( Pile *p)
		{
			tabNombres * tmp = malloc ( 2*p->tailleTab *sizeof(tabNombres) );
			memcpy( tmp, p->tab, p->tailleTab * sizeof( tabNombres ) );
			free( p->tab );
			p->tab = tmp;
			p->tailleTab *= 2;
		}


		/**
		 * Insère le tableau 'tn' au sommet de la pile 'p'.
		 *
		 * Note : au besoin, la pile 'p' est agrandie dynamiquement. Elle n'a pas de
		 * taille maximale.
		 */
		void empile( Pile * p, tabNombres val)
		{
			pthread_mutex_lock(&(p -> protecteur));
			if ( p->sommet + 1 == p->tailleTab )
			{
				agrandirPile( p );
			}
			++(p->sommet);
			p->tab[p->sommet] = val;
			pthread_mutex_unlock(&(p -> protecteur));
			sem_post(&(p -> nbElem));
		}

		/**
		 * Libère les ressources utilisées par la file 'p'.
		 */
		void detruirePile( Pile * p )
		{
			free( p->tab );
		}

	/***********************************************************
	 * Tri rapide FT et workers                                *
	 ***********************************************************/

		void triRapideFT( tabNombres tn )
		{
			Pile p;
			pthread_t T[NBWORKERS];
			int i;
			initPile(&p);
			p.nbElemAtrier = tn.fin;
			empile(&p,tn);
			for (i = 0; i < NBWORKERS; i++)
			{
				/* choisir une des deux lignes ci dessous pour le triFT normal ou amélioré */
				/* pthread_create(&T[i],NULL,(void *(*)(void *)) worker, (void *) &p); */
				pthread_create(&T[i],NULL,(void *(*)(void *)) worker_bis, (void *) &p);
			}
			for (i = 0; i < NBWORKERS; i++)
			{
				pthread_join ( T[i], NULL );
			}
			detruirePile(&p);
		}

		void worker(Pile* p)
		{

			tabNombres tn;
			tabNombres gauche, droite;
			int pivot;
			int i;

			while (1)
			{
				/* on vérifie que le tri n'est pas fini. Si c'est le cas on empile des tableaux bidons*/
				pthread_mutex_lock(&(p -> protecteurNbTries));
					if(p->nbElemTries == p->nbElemAtrier)
					{
						/* Empiler des tableaux bidons */
						for(i = 0; i < NBWORKERS; i++){
							tn.val = NULL;
							empile(p,tn);
						}
					}
				pthread_mutex_unlock(&(p -> protecteurNbTries));

				/* on dépile un tableau */
				depile(p,&tn);

				/* si on a dépilé un tableau bidon c'est qu'il faut s'arrêter */
				if(tn.val == NULL){
					pthread_exit(NULL);
				}

				/* partitionnement : après ça le pivot est à sa place définitive */
				partitionner ( tn, &pivot );
				pthread_mutex_lock(&(p -> protecteurNbTries));
					++(p->nbElemTries);
				pthread_mutex_unlock(&(p -> protecteurNbTries));

				/* On calcule la taille du sous tableau gauche, si >1 alors récursion */
				if ( pivot - tn.debut > 1 )
				{
					gauche.debut = tn.debut;
					gauche.fin = pivot;
					gauche.val = tn.val;
					empile(p,gauche);
				}
				/* Sinon, si la taille du tableau est exactement 1 son élément est à sa place définitive */
				else if (pivot - tn.debut == 1)
				{
					pthread_mutex_lock(&(p -> protecteurNbTries));
						++(p->nbElemTries);
					pthread_mutex_unlock(&(p -> protecteurNbTries));
				}

				/* On fait pareil pour le sous tableau droit */
				if ( tn.fin - (pivot + 1) > 1 )
				{
					droite.debut = pivot + 1;
					droite.fin = tn.fin;
					droite.val = tn.val;
					empile(p,droite);
				}
				else if (tn.fin - (pivot + 1) == 1)
				{
					pthread_mutex_lock(&(p -> protecteurNbTries));
						++(p->nbElemTries);
					pthread_mutex_unlock(&(p -> protecteurNbTries));
				}
			}
		}

/***********************************************************
 *                   Exercice 3                            *
 ***********************************************************/

	void triInsertion(tabNombres tn)
	{
		int i,j,elem;
		for (i = tn.debut; i < tn.fin; i++){
			elem = tn.val[i];
			for(j = i; j > 0 && tn.val[j-1] > elem; j--){
				tn.val[j] = tn.val[j-1];
			}
			tn.val[j] = elem;
		}
	}

	void shellPhase(tabNombres tn, int pas) {
		  int i,j,value;
	 
		  for (i = tn.debut + pas; i < tn.fin; ++i) {
		      value = tn.val[i];
		      for (j = i - pas; j >= 0 && tn.val[j] > value; j -= pas) {
		          tn.val[j + pas] = tn.val[j];
		      }
		      tn.val[j + pas] = value;
		  }
	}
	 
	void triShell(tabNombres tn) {
		  static const int pas[] = {
		      1, 4, 10, 23, 57
		  };
		  int taille;
	 
		  for (taille = sizeof(pas)/sizeof(pas[0]) - 1;
		             taille >= 0;
		             --taille)
		      shellPhase(tn, pas[taille]);
	}

	void worker_bis(Pile* p)
	{

		tabNombres tn;
		tabNombres gauche, droite;
		int pivot;
		int i;

		while (1)
		{

			pthread_mutex_lock(&(p -> protecteurNbTries));
				if(p->nbElemTries == p->nbElemAtrier){
					for(i = 0; i < NBWORKERS; i++){
						tn.val = NULL;
						empile(p,tn);
					}
				}
			pthread_mutex_unlock(&(p -> protecteurNbTries));
		
			depile(p,&tn);

			if(tn.val == NULL){
				pthread_exit(NULL);
			}

			partitionner ( tn, &pivot );
		
			pthread_mutex_lock(&(p -> protecteurNbTries));
				++(p->nbElemTries);
			pthread_mutex_unlock(&(p -> protecteurNbTries));

			/* Application du tri par insertion lorsque la portion gauche du tableau a traiter est inferieure a une certaine valeur */
			if( pivot - tn.debut < LIMITE_INF )
			{
				gauche.debut = tn.debut;
				gauche.fin = pivot;
				gauche.val = tn.val;

				/* nbElemTries sera egal a la taille du tableau qu'on passe en parametre a triInsertion */
				pthread_mutex_lock(&(p -> protecteurNbTries));
					p->nbElemTries += pivot - tn.debut;
				pthread_mutex_unlock(&(p -> protecteurNbTries));

				/* choisir une des lignes ci-dessous pour le tri par insertion ou le triShell */
				/* triInsertion(gauche); */
				triShell(gauche);
			} 
			else 
			{
				 gauche.debut = tn.debut;
				 gauche.fin = pivot;
				 gauche.val = tn.val;
				 empile(p,gauche);
			}

			/* On fait pareil sur la partie droite */
			if( tn.fin - (pivot + 1) < LIMITE_INF )
			{
				droite.debut = pivot + 1;
				droite.fin = tn.fin;
				droite.val = tn.val;

				pthread_mutex_lock(&(p -> protecteurNbTries));
					p->nbElemTries += tn.fin - (pivot + 1);
				pthread_mutex_unlock(&(p -> protecteurNbTries));

				/* choisir une des lignes ci-dessous pour le tri par insertion ou le triShell */
				/* triInsertion(droite); */
				triShell(droite);
			} 
			else 
			{
				 droite.debut = pivot + 1;
				 droite.fin = tn.fin;
				 droite.val = tn.val;
				 empile(p,droite);
			}
		}
	}
