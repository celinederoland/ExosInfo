#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define THREAD_FUNC(f) (void*(*)(void*))f

int * decompte( int * px )
{
  int y;
  int *py;
  while ( *px > 0 )
    {
      ++y;
      --(*px);
    }
  py = ( int* ) malloc( sizeof( int ) );
  *py = y;
  pthread_exit( py );
}

int main( int argc, const char ** argv )
{
  int x = atoi( argv[1] );
  int *ya, *yb;
  pthread_t ta, tb;
  (void) argc; /* ne fait rien, évite un warning si compilé avec -Wextra */
  pthread_create( &ta, NULL, THREAD_FUNC( decompte ), (void*) &x );
  pthread_create( &tb, NULL, THREAD_FUNC( decompte ), (void*) &x );
  pthread_join( ta, (void**) &ya );
  pthread_join( tb, (void**) &yb );
  printf("%d + %d = %d\n", *ya, *yb, *ya + *yb );
  free( ya );
  free( yb );
  return 0;
}
