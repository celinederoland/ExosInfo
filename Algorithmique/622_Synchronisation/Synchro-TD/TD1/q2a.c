#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int x = 0;

#define THREAD_FUNC(f) (void*(*)(void*))f

void threadA( )
{
  int i;
  for ( i=0; i<10; ++i )
    {
      ++x;
      printf( "Je suis le thread A et x vaut %d\n", x );
      sleep( 1 );
    }
  pthread_exit( NULL );
}

void threadB( )
{
  int i;
  for ( i=0; i<10; ++i )
    {
      --x;
      printf( "Je suis le thread B et x vaut %d\n", x );
      sleep( 1 );
    }
  pthread_exit( NULL );
}

int main()
{
  pthread_t ta, tb;
  pthread_create( &ta, NULL, THREAD_FUNC( threadA ), NULL );
  pthread_create( &tb, NULL, THREAD_FUNC( threadB ), NULL );
  pthread_join( ta, NULL );
  pthread_join( tb, NULL );
  return 0;
}
