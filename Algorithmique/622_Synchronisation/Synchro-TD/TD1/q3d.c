#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define THREAD_FUNC(f) (void*(*)(void*))f

typedef struct
{
  int x;
  int verrou;
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
  while ( pd->x > 0 )
    {
      ++y;
      while ( pd->verrou == 1 )
        { /* ne rien faire */ }
      pd->verrou = 1;
      --(pd->x);
      pd->verrou = 0;
    }
  py = ( int* ) malloc( sizeof( int ) );
  *py = y;
  pthread_exit( py );
}

int main( int argc, const char ** argv )
{
  data d;
  int *ya, *yb;
  pthread_t ta, tb;
  (void) argc; /* ne fait rien, évite un warning si compilé avec -Wextra */
  d.x = atoi( argv[1] );
  d.verrou = 0;
  pthread_create( &ta, NULL, THREAD_FUNC( decompte ), (void*) &d );
  pthread_create( &tb, NULL, THREAD_FUNC( decompte ), (void*) &d );
  pthread_join( ta, (void**) &ya );
  pthread_join( tb, (void**) &yb );
  printf("%d + %d = %d\n", *ya, *yb, *ya + *yb );
  free( ya );
  free( yb );
  return 0;
}
