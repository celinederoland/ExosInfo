#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include "triRapide.h"

/**
 * Fichier : triRapide.c
 *
 * Implémentation de l'algorithme du tri rapide pour trier un tableau de
 * nombres.
 */


#define NBWORKERS 1
#define LIMITE_INF 100 


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

void debugNombres(tabNombres tn)
{
  int i;
  for (i = tn.debut; i < tn.fin; ++i)
    {
      fprintf(stderr,"%d - ", tn.val[i] );
    }
  fprintf(stderr,"\n");
}

void triRapideMTH( tabNombres * tn )
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
	 result = pthread_create(&TG, NULL, (void *(*)(void *)) triRapideMTH, (void *) &gauche);
      /* par conséquent, le nombre de threads double à chaque étape : on arrive rapidement à un nombre de threads très important */
	 /* en réalité, à la fin de l'exécution le nombre de thread est du même ordre de grandeur que le nombre d'éléments du tableau */
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

  /* on attend le thread qui s'occupe de la partie gauche */
  if (result == 0) { pthread_join(TG,NULL); }
}

void triRapideMT( tabNombres tn)
{
	/* On appelle une autre fonction qui prend en argument un pointeur de façon à pouvoir utiliser les thread */
	triRapideMTH(&tn);
}

/***********************************************************
 * Les fonctions qui suivent servent à la manipulation des *
 * piles de 'tabNombres'.                                  *
 ***********************************************************/

/** 
 * Structure utilisée pour représenter une pile (respecte le principe FILO) de
 * tableaux de nombres (struct tabNombres).
 */
typedef struct 
{
  tabNombres * tab;
  sem_t sommet; /* int sommet; /* Indice du plus haut élément de la pile, -1 si vide */ /* Pour nous, c'est 0 si vide, ie sommet représente la taille occupée */
  pthread_mutex_t protecteur;
  int tailleTab; /* Taille du tableau utilisé pour stocker les éléments */
} Pile;


/**
 * Initilise 'p' en une pile vide.
 */
void initPile( Pile * p )
{
  p->tailleTab = 1;
  p->tab = malloc( p->tailleTab * sizeof (tabNombres) );
  sem_init( &(p -> sommet), 0, 0 ); 
  pthread_mutex_init( &(p -> protecteur), NULL );
}

/**
 * Retire le premier élément de la Pile 'p' et l'incrit dans 'tn'.
 *
 * Note : si la pile 'p' est vide, l'appel est bloquant jusqu'à ce qu'un élément
 * y soit inséré.
 */
void depile( Pile * p, tabNombres * val)
{
  int taillePile;
  sem_wait(&(p -> sommet));
  pthread_mutex_lock(&(p -> protecteur));
  sem_getvalue(&(p -> sommet),&taillePile);
	fprintf(stderr,"taillePile : %d\n",taillePile);
	debugNombres(p -> tab [ taillePile - 1 ]);
  *val = p -> tab [ taillePile - 1 ];
	debugNombres(*val);
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
void empile( Pile * p, tabNombres * val)
{
  int taillePile;
  /*if ( p -> sommet + 1 == p -> tailleTab )
    {
      agrandirPile( p );
    }*/
  pthread_mutex_lock(&(p -> protecteur));
  sem_getvalue(&(p -> sommet),&taillePile); /* après ça, d'autres workers peuvent empiler .... et donc peut être qu'il n'y aura plus de place sur la pile quand l'ordonnanceur nous redonnera la main */
  if ( taillePile == p -> tailleTab )
  {
    agrandirPile( p );
  }
  p->tab[ taillePile ] = *val;
  pthread_mutex_unlock(&(p -> protecteur));

  sem_post(&(p -> sommet));
}

/**
 * Libère les ressources utilisées par la file 'p'.
 */
void detruirePile( Pile * p )
{
  free( p->tab );
}

/***********************************************************
 * Exercice 2                                              *
 ***********************************************************/
void worker(Pile* p)
{

  tabNombres tn;
  tabNombres gauche, droite;
  int pivot;

  while (1)
  {
		fprintf(stderr,"tri rapide, je vais dépiler \n");
  	depile(p,&tn);
		fprintf(stderr,"tri rapide, j'ai dépilé \n");
	  /****************
	   *    Debug     *
	   ****************/
	  fprintf(stderr,"tri rapide de : ");
	  debugNombres(tn);
	  /****************/
	  partitionner ( tn, &pivot );
	  /* On calcule la taille du sous tableau gauche, si >1 alors récursion */
	  if ( pivot - tn.debut > 1 )
	    {
		 gauche.debut = tn.debut;
		 gauche.fin = pivot;
		 gauche.val = tn.val;
		 /****************
		  *    Debug     *
		  ****************/
	  	 fprintf(stderr,"gauche : ");
		 debugNombres(gauche);
		 /****************/
		 empile(p,&gauche);
	    }
	  /* On calcule la taille du sous tableau droit, si >1 alors récursion */
	  if ( tn.fin - (pivot + 1) > 1 )
	    {
		 droite.debut = pivot + 1;
		 droite.fin = tn.fin;
		 droite.val = tn.val;
		 /****************
		  *    Debug     *
		  ****************/
	   fprintf(stderr,"droite : ");
		 debugNombres(droite);
		 /****************/
		 empile(p,&droite);
	    }
  }
}

void triRapideFT( tabNombres tn )
{
	Pile p;
	pthread_t T[NBWORKERS];
	int i;
	initPile(&p);
	empile(&p,&tn);
  fprintf(stderr,"tri rapide FT, j'ai empilé \n");
  debugNombres(tn);
	for (i = 0; i < NBWORKERS; i++)
	{
		pthread_create(&T[i],NULL,(void *(*)(void *)) worker, (void *) &p);
	}
	sleep(2);
	for (i = 0; i < NBWORKERS; i++)
	{
		pthread_cancel(T[i]);
	}
}


