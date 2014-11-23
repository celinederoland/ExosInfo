/**
 * Fichier : triRapide.h
 */

/**
 * Structure représentant un tableau d'entiers (int).
 *
 * les cases de ce tableau sont :
 * val[debut], val[debut+1], val[debut+2], ..., val[fin-1].
 *
 * On remarque que la dernière case du tableau est 'fin-1'.
 *
 * Ainsi la longueur du tableau est : 'fin - debut'.
 *
 * On parcours toutes les cases du tableau 't' en faisant :
 * for ( i=t.debut; i<t.fin; ++i)
 *   { 
 *      Traiter t.val[i]
 *   }
 */
typedef struct 
{
  int * val; /* Adresse du tableau de nombres */
  int debut; /* Indice de la première case */
  int fin;   /* Indice juste après la dernière case+1. */
} tabNombres;


/* Fonctions relatives au tri rapide */
void triRapide ( tabNombres tn );
void triRapideMT ( tabNombres tn );
void triRapideFT ( tabNombres tn );



