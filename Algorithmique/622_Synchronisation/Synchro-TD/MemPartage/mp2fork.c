#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>           /* Pour les constantes O_ */
#include <sys/stat.h>        /* Pour les constantes de mode */
#include <sys/mman.h>        /* Pour la manipulation de mémoire partagée */
#include <unistd.h>          /* Pour la fonction sleep */

/**
 * Fichier mp_2.c
 *
 * Exemple d'utilisation de mémoire partagée.
 * Lancer l'exécution après avoir démarré celle de `mp_1`.
 *
 * Lit un entier situé en mémoire partagée.
 */

int main()
{
  int * partage;
  int df;
	int PID;

  /* Ouverture de l'objet mémoire partagée */
  df = shm_open("/maRegion", O_RDONLY ,  S_IRWXU );
  if ( df == -1 ) 
    {
      fprintf( stderr, "Erreur lors de l'ouverture de la mémoire partagée\n" );
      exit( EXIT_FAILURE );
    }

  /* Le fichier virtuel est projecté dans l'espace d'adressage et son adresse
   * enregistrée dans partage */
  partage = ( int * ) mmap( NULL, sizeof( int ), PROT_READ, MAP_SHARED, df, 0); 
  if ( partage == MAP_FAILED )
    {
      fprintf( stderr, "Erreur lors de la projection en mémoire\n" );
      exit( EXIT_FAILURE );
    }

  printf("Mémoire partagée projectée à l'adresse : %p\n", (void *) partage );
	PID = fork();
  while ( *partage != -1 ) 
    {
      sleep( 1 );
			if (PID == 0)
			{
      	printf( "FILS : Valeur de l'entier partagé : %d\n", *partage );
			}
			else
			{
      	printf( "PERE : Valeur de l'entier partagé : %d\n", *partage );
			}
    }
  munmap ( partage, sizeof( int ) );
  close( df );
  return 0;
}

