#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "triRapide.h"

/**
 * Fichier : tri.c
 *
 * Lis des nombres sur l'entrée standard, les trie à l'aide du tri rapide
 * et affiche le résultat sur la sortie standard.
 */

/* Gestion des entrées/sorties */
void lireNombres( tabNombres * ptrTn );
void afficheNombres ( tabNombres tn );

/* Fonctions utilitaires */
void debut_block ( const char * nom, struct timeval * temps );
void fin_block ( const char * nom, struct timeval * temps );



int main( )
{
  struct timeval temps;
  tabNombres tn;
  int err = 0;

  debut_block( "Lecture", &temps);
  lireNombres( &tn );
  fin_block( "Lecture", &temps);

  debut_block( "Tri", &temps);
  /* triRapide( tn ); ++err; */
  /* triRapideMT( tn ); ++err; */
  triRapideFT( tn ); ++err; 
  if ( err > 1 ) 
    {
      fprintf( stderr, "Vous triez la liste deux fois, "
              "vous ne testez pas réellement votre tri !\n" );
      exit( EXIT_FAILURE );
    }
  fin_block( "Tri", &temps);

  afficheNombres ( tn );
  free ( tn.val );

  return 0;
}


/**
 * Lit une liste de nombre sur l'entrée standard et l'inscrit dans le tableau de
 * nombre pointé par 'tn'.
 */
void lireNombres( tabNombres * ptr_tn )
{
  int *tmp, n;
  ptr_tn->debut = 0;
  ptr_tn->fin = 10;
  ptr_tn->val = malloc ( ptr_tn->fin *sizeof(int) );
  n=0;
  while ( scanf( "%d" , & ptr_tn->val[n] ) != EOF )
    {
      ++n;
      if ( n == ptr_tn->fin ) 
	{
          /* Le tableau est plein, on le remplace par un plus grand */
	  tmp = malloc ( 2*n * sizeof(int) );
	  memcpy ( tmp, ptr_tn->val, n*sizeof(int) );
	  free ( ptr_tn->val );
	  ptr_tn->val = tmp;
	  ptr_tn->fin *= 2;
	}
    }
  ptr_tn->fin = n;
}


/**
 * Affiche les nombres du tableau 'tn' dans la sortie standrad.
 */
void afficheNombres ( tabNombres tn )
{
  int i;
  for (i = tn.debut; i < tn.fin; ++i)
    {
      printf("%d\n", tn.val[i] );
    }
}


/**
 * Affiche le début d'un bloc de calcul dans le canal d'erreur et lance un
 * chronomètre.
 */
void debut_block ( const char * nom, struct timeval * temps )
{
  fprintf( stderr, " -- Début de [ %s ] \n", nom) ;
  gettimeofday( temps, NULL );
}

/**
 * Affiche la fin du bloc de calcul dans le canal d'erreur en mentionnant le
 * temps d'exécution.
 */
void fin_block ( const char * nom, struct timeval *tv )
{
  int microseconds;
  double milliseconds;
  struct timeval tv2;
  gettimeofday( &tv2, NULL );
  microseconds = ( tv2.tv_sec - tv->tv_sec) * 1000000 
		    + ((int)tv2.tv_usec - (int)tv->tv_usec);
  milliseconds =  ( (double) microseconds ) / 1000.0f;
  fprintf(stderr, " -- Fin de [ %s ] block effectué en %4.4f ms --\n", nom,
          milliseconds);
}


