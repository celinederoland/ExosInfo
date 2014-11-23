#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * Fichier : genAleat.c
 * Auteur : Xavier Provençal
 *
 * Utilitaire qui génère une liste de nombres anéeatoires
 */

#define VALEUR_DEFAUT 1000

int main(int argc, const char * argv[] )
{
  int n,N,i;
  if (argc < 2)
    {
      fprintf (stderr, "Usage : %s n [N]", argv[0]);
      fprintf (stderr, "  Génère une liste de 'n' nombres aléeatoires de 0 à N-1\n");
      fprintf (stderr, "  Si N n'est pas spécifié la valeur %d est utilisée par défaut\n", VALEUR_DEFAUT);
      exit(1);
    }
  n = atoi(argv[1]);
  N = (argc >= 3) ? atoi(argv[2]) : VALEUR_DEFAUT;
  srand( time( NULL ) );
  for (i=0; i<n; ++i)
    {
      printf("%d\n", rand() % N );
    }
  return 0;
}


