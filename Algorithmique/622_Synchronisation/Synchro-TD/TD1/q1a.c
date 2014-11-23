#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
  int pid, x, i;
  x = 0;
  pid = fork();
  if ( pid > 0 )
    {
      for ( i=0; i<10; ++i )
        {
          ++x;
          printf( "Je suis le père et x vaut %d\n", x );
          sleep(1);
        }
      wait( NULL ); /* un père devrait toujours attendre son fils. */
    }
  else
    {
      for ( i=0; i<10; ++i )
        {
          --x;
          printf( "Je suis le fils et x vaut %d\n", x );
          sleep(1);
        }
    }
  return 0;
}

