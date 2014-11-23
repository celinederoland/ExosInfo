#include "monTube_exo4.h"
#include <stdio.h>

int monTube_exo4_init(monTube_exo4_t *mt )
{
	int err;

	mt -> df = shm_open("/monSegmentPartage",O_CREAT|O_RDWR,S_IRWXU);
	if (mt -> df == -1) 
	{ 
		fprintf(stderr, "Erreur lors de l'ouverture de la mémoire partagée\n"); 
		return -1;
	}

	err = ftruncate(mt -> df, sizeof( monTube_exo4_partagee ));
	if (err != 0) 
	{
		fprintf( stderr, "Erreur lors du redimensionnement de mémoire partagée\n" );
		return -1;
	}

	mt -> partage = (monTube_exo4_partagee*) mmap (NULL, sizeof(monTube_exo4_partagee), 
				PROT_READ | PROT_WRITE, MAP_SHARED, mt -> df, 0);

	if (mt -> partage == MAP_FAILED) 
	{ 
		fprintf(stderr, "Erreur lors de la projection en mémoire\n"); 
		return -1;
	}

	/* Initialisation des indices de lecture et d'écriture */
	mt -> partage -> indecriture = 0;
	mt -> partage -> indlecture = 0;

	err = sem_init(&(mt->partage->sem), 1, 0);
	if (err != 0) 
	{ 
		fprintf(stderr, "Erreur lors de l'initialisation du sémaphore sem\n"); 
		return -1; 
	}

	err = sem_init(&(mt->partage->semplace), 1, TAILLE_TUBE);
	if (err != 0) 
	{ 
		fprintf(stderr, "Erreur lors de l'initialisation du sémaphore semplace\n"); 
		return -1; 
	}

	return 0;
}

int monTube_exo4_ecrire(monTube_exo4_t *mt, char c)
{
	int retour = sem_wait(&(mt -> partage -> semplace));
	mt -> partage -> c[mt -> partage -> indecriture] = c;
	mt -> partage -> indecriture = (mt -> partage -> indecriture + 1)%TAILLE_TUBE;
	retour += sem_post(&(mt -> partage -> sem));
	if ( retour < 0) { return -1; }
	return 1;
}


int monTube_exo4_lire(monTube_exo4_t *mt, char *c)
{
	int retour = sem_wait(&(mt -> partage -> sem));
	*c = mt -> partage -> c[mt -> partage -> indlecture];
	mt -> partage -> indlecture = (mt -> partage -> indlecture + 1)%TAILLE_TUBE;
	retour += sem_post(&(mt -> partage -> semplace));
	if ( retour < 0) { return -1; }
	return 1;
}

int monTube_exo4_detruire(monTube_exo4_t *mt)
{
	int retour = sem_destroy(&(mt -> partage -> sem));
	retour += sem_destroy(&(mt -> partage -> semplace));
	retour += munmap( mt -> partage, sizeof(monTube_exo4_partagee));
	retour += close(mt->df);
	retour += shm_unlink("/monSegmentPartage");
	if ( retour < 0) { return -1; }
	return 0;
}

