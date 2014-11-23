#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define THREAD_FUNC(f) (void*(*)(void*))f

void threadA( int * px )
{
  int i;
  for ( i=0; i<10; ++i )
    {
      ++(*px);
      printf( "Je suis le thread A et x vaut %d\n", *px );
      sleep( 1 );
    }
  pthread_exit( NULL );
}

void threadB( int * px )
{
  int i;
  for ( i=0; i<10; ++i )
    {
      --(*px);
      printf( "Je suis le thread B et x vaut %d\n", *px );
      sleep( 1 );
    }
  pthread_exit( NULL );
}

int main()
{
  int x = 0;
  pthread_t ta, tb;
  pthread_create( &ta, NULL, THREAD_FUNC( threadA ), (void*) &x );
  pthread_create( &tb, NULL, THREAD_FUNC( threadB ), (void*) &x );
  pthread_join( ta, NULL );
  pthread_join( tb, NULL );
  return 0;
}
