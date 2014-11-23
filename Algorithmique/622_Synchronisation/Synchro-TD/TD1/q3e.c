#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define THREAD_FUNC(f) (void*(*)(void*))f

typedef struct
{
  int * x;
  int * verrou;
  int cle;
} data;


/* La section critique est formée d'une seule ligne : 
 * --(*px);
 * car c'est le seul moment où on manipule une donnée qui est commune aux deux
 * threads.
 */
int * decompte( data * pd )
{
  int y;
  int *py;
  while ( *pd->x > 0 )
    {
      ++y;

      /* entrée en zone critique */
      while ( *pd->verrou != pd->cle  )
        { /* ne rien faire */ }

      /* zone critique */
      --(*pd->x);

      /* sortie de la zone critique */
      *pd->verrou = 1 - (pd->cle);

    }
  py = ( int* ) malloc( sizeof( int ) );
  *py = y;
  pthread_exit( py );
}

int main( int argc, const char ** argv )
{
  data da, db;
  int *ya, *yb, x, verrou;
  pthread_t ta, tb;
  (void) argc; /* ne fait rien, évite un warning si compilé avec -Wextra */

  /* initialisation des variables */
  x = atoi( argv[1] );
  verrou = 0;

  /* initialisation des données passées aux threads */
  da.x = &x;
  da.verrou = &verrou;
  da.cle = 0;

  db.x = &x;
  db.verrou = &verrou;
  db.cle = 1;

  pthread_create( &ta, NULL, THREAD_FUNC( decompte ), (void*) &da );
  pthread_create( &tb, NULL, THREAD_FUNC( decompte ), (void*) &db );
  pthread_join( ta, (void**) &ya );
  pthread_join( tb, (void**) &yb );
  printf("%d + %d = %d\n", *ya, *yb, *ya + *yb );
  printf("x = %d\n", x );
  free( ya );
  free( yb );
  return 0;
}
