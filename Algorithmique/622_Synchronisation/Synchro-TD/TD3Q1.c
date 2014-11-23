#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>

void f(sem_t *ps)
{
	sem_wait(ps);
	int val_sem;
	sem_getvalue(ps,&val_sem);
	printf("Après WAIT, valeur du sémaphore : %d \n",val_sem);
}

void g(sem_t *ps)
{
	int i, val_sem;
	for (i = 0; i < 5; i++)
	{
		sleep(1);
		printf("Je vais poster\n");
		sem_post(ps);
	}
	sem_getvalue(ps,&val_sem);
	printf("Après la boucle, valeur du sémaphore : %d \n",val_sem);
}

int main()
{
	sem_t s;
	int i;
	pthread_t t[4];
	sem_init( &s,0,1);
	pthread_create(&t[0], NULL, (void*(*)(void*)) f, (void*) &s);
	pthread_create(&t[1], NULL, (void*(*)(void*)) f, (void*) &s);
	pthread_create(&t[2], NULL, (void*(*)(void*)) f, (void*) &s);

	pthread_create(&t[3], NULL, (void*(*)(void*)) g, (void*) &s);

	for (i=0;i<4;i++)
	{
		pthread_join(t[i],NULL);
	}
	return 0;
}


