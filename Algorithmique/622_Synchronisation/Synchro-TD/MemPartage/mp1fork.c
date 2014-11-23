#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>           /* Pour les constantes O_ */
#include <sys/stat.h>        /* Pour les constantes de mode */
#include <sys/mman.h>        /* Pour la manipulation de mémoire partagée */
#include <unistd.h>          /* Pour la fonction sleep */

/**
 * Fichier mp_1.c
 *
 * Exemple d'utilisation de mémoire partagée.
 * Lancer l'exécution avant de démarrer celle de `mp_2`.
 *
 * Crée et initialise un segment de mémoire partagée contenant un entier.
 *
 */

int main()
{
  int * partage;
  int df, err;

  /* Création de l'objet de mémoire partagée. Si `/maRegion` existe déjà, elle
   * est simplement ouverte. Si on avait utilisé le drapeau O_EXCL le fait que
   * `/maRegion` existe déjà entraine une erreur.
   */
  df = shm_open("/maRegion", O_CREAT | O_RDWR, S_IRWXU );
  if ( df == -1 ) {
      fprintf( stderr, "Erreur lors de la création du segment de mémoire partagée\n" );
      exit( EXIT_FAILURE );
  }

  /* La zone est redimensionnée à la taille de `struct region` */
  err = ftruncate(df, sizeof( int ));
  if (err != 0) {
      fprintf( stderr, "Erreur lors du redimensionnement de mémoire partagée\n" );
      exit( EXIT_FAILURE );
  }

  /* Le fichier virtuel est projecté dans l'espace d'adressage et son adresse
   * enregistrée dans ``partage`` */
  partage = ( int * ) mmap( NULL, sizeof( int ), 
				     PROT_READ | PROT_WRITE, MAP_SHARED, df, 0); 
  if ( partage == MAP_FAILED ) {
      fprintf( stderr, "Erreur lors de la projection en mémoire\n" );
      exit( EXIT_FAILURE );
  }

  printf("Mémoire partagée projectée à l'adresse : %p\n", (void *) partage );
  *partage = 0;
  while ( *partage != -1 )
    {
      scanf( "%d", partage );
    }

  /* Libération des ressources */
  munmap ( partage, sizeof( int ) );
  close( df );
  shm_unlink( "/maRegion" );
  return 0;
}

