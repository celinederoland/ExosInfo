#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>					/* fork, close, pipe, write, read */
#include <sys/wait.h>				/* wait */
#include <sys/mman.h>				/* mmap, munmap, shm_open, shm, unlink */
#include <sys/stat.h>				/* Pour les constantes de mode */
#include <fcntl.h>					 /* Pour les constantes O_ */
#include <semaphore.h>
#include "monTube_exo4.h"
#include "monTube_exo5.h"
#include "monTube_exo6.h"


/*
 * Fichier INFO622-tp2.c
 *
 *
 * DESCRIPTION DU TP :
 *
 * Le but de ce TP est de vous familiariser avec les tubes UNIX et la mémoire
 * partagée. Plus précisément, vous serez amené à créer votre propre
 * implémentation de tubes. Bien entendu, les tubes que nous vous demandons
 * d'implémenter sont bien différents des tubes UNIX. 
 *
 * Une première différence est qu'ils sont implémentés à l'aide de mémoire
 * partagée au lieu d'utiliser des descripteurs de fichiers. De plus, les
 * fonction d'écriture et de lecture qu'on vous demande d'implémenter manipulent
 * un seul octet de données à la fois.	Plus précisément, seul le contenu d'une
 * variable de type ``char`` est écrit ou lu dans le tube lors d'une opération
 * de lecture ou d'écriture.
 *
 * Ce TP comporte 6 exercices qui sont décrits ci-dessous.
 *
 * 
 * REMISE TU TP : 
 *
 * Le TP est à remettre par via la platforme TPLab.
 *
 * Remettez UNIQUEMENT LES FICHIERS SOURCES sous la forme d'une archive de
 * l'un des formats suivants : tar, tgz ou zip.
 *
 *
 *
 * INSTRUCTIONS PARTICULIÈRES : 
 *
 *	- Les fonctions mem_partagee_init, mem_partagee_pere, mem_partagee_fils
 *	ainsi que toutes celles présentes dans le fichier monTube.c doivent être
 *	précédées pas un commentaires qui décrit ce qu'elle fait et comment elle le
 *	fait.
 *
 *	- Tous les types définis dans le fichier monTube.h doivent être commentés.
 *	En particulier, chaque élément d'une structure doit être décrit en
 *	commentaire.
 *
 */


#define UN_GRAND_NOMBRE 10000000







/************************************************************
 *********************	 Exercice 1		**********************
 ************************************************************
 *
 * Utilisation des tubes unix
 *
 * Le code suivant crée un tube non nommé puis effectue un fork. Ensuite, le
 * père écrit les caractères 'a' à 'e' dans le tube alors que le fils lit un
 * caractère à la fois puis l'affiche dans le terminal.
 *
 * Modifiez ce code de manière à ce que le père écrive dans le tube les 10
 * premières puissances de 2 sous la forme d'entiers (int) puis les 10 suivantes
 * sour la forme de flottants double précision (double). 
 *
 * Le fils doit, lire et afficher tout le contenu du tube sous la forme
 * d'entiers. 
 *
 *
 ************************************************************************
 * Écrivez ici ce que vous observez à l'exécution et expliquez pourquoi
 * vous l'observez.
 ************************************************************************
 * Si on laisse le fils lire les données sous la forme d'entiers, il lit 
 * les doubles en deux parties (un double a deux fois la taille d'un entier),
 * donc il affiche n'importe quoi pour les dix puissances suivantes.
 * Conclusion : si un père doit enregistrer des données de type différent,
 * cela rend l'utilisation du tube beaucoup moins souple car le fils doit 
 * savoir d'avance dans quel ordre les données seront enregistrées par le 
 * père.
 *
 */ 
void exo1 () 
{
	int err, pid, tube[2], tube_lecture, tube_ecriture;
	int n; int p = 2;
	double pdouble;

	err = pipe(tube);
	if ( err != 0 )
	{
		fprintf( stderr, "Erreur lors de la création du tube\n" );
		exit( EXIT_FAILURE );
	}
	tube_lecture = tube[0];
	tube_ecriture = tube[1];
	pid = fork();
	if (pid) 
	{
		/* Ici c'est le père, on utilise l'extrémité d'écriture du
		 * tube */
		close(tube_lecture);
		for (n=1; n <= 10; n++, p = p*2) 
		{
			write(tube_ecriture, &p, sizeof(int));
		}
		pdouble = (double)p;
		for (n=1; n <= 10; n++, pdouble = pdouble*2)
		{
			write(tube_ecriture, &pdouble, sizeof(double));
		}
		close(tube_ecriture);
		wait(NULL);
	} 
	else 
	{
		/* Ici c'est le fils, on utilise l'extrémité de lecture du
		 * tube. */
		close(tube_ecriture);
		/*for (n=1; n <= 10 && read(tube_lecture, &p, sizeof(int)) != 0; n++ ) 
		{
			printf("Le fils a lu : %d\n", p);
		}
		for (n=1; n <= 10 && read(tube_lecture, &pdouble, sizeof(double)) != 0; n++ )
		{
			printf("Le fils a lu : %f\n", pdouble);
		}*/
		while (read(tube_lecture, &p, sizeof(int)) != 0) 
		{
			printf("Le fils a lu : %d\n", p);
		}
		exit( EXIT_SUCCESS );
	}
}


/************************************************************
 *********************   Exercice 2    **********************
 ************************************************************
 * Utilisation simple d'un segment de mémoire partagée.
 *
 * Pour cet exercice, il faudra initialiser un segment de mémoire partagée
 * suffisamment grand pour y écrire au moins 10 entiers (int).
 *
 * Ensuite, complétez les fonctions :
 *
 * ``exo2_initialisation`` qui initialise un objet de mémoire partagée contenant
 * une instance du type ``exo2_partagee`` et l'ajoute à l'espace d'adressage du
 * processus.
 *
 * ``exo2_pere`` qui écrit les nombres 3, 1, 4, 1, 5, 9, 2, 6, 5, 3 dans la
 * mémoire partagée.
 *
 * ``exo2_fils`` qui attend que le père ait terminé d'écrire les dix nombres
 * puis les affiche.
 *
 * ``exo2_liberation`` qui libère les ressources allouées.
 *
 * Commencez par vous demander comment faire attendre le fils puis définissez le
 * type de données ``exo2_partagee`` et ``exo2_t`` en conséquence.
 *
 * N'oubliez pas de décrire votre stratégie.
 */

/* Notre stratégie :
 * 
 * Pour faire attendre le fils, nous pouvons utiliser un sémaphore
 */

typedef struct
{
	sem_t sem; /* Le sémaphore pour que le fils attende */
	int tab[10]; /* Un tableau de 10 entiers */
} exo2_partagee;


typedef struct
{
	exo2_partagee* partage; /* La zone de mémoire partagée */
	int df; /* Référence du fichier virtuel pour le partage */
} exo2_t;

/*
 * Déclarations des fonctions relatives à l'exercice 2
 */
void exo2_initialisation( exo2_t * donnees );
void exo2_pere( exo2_t * donnees );
void exo2_fils( exo2_t * donnees );
void exo2_liberation( exo2_t * donnees );

void exo2()
{
	exo2_t donnees;
	int pid; 

	exo2_initialisation( &donnees );
	pid = fork();
	if (pid) 
		{
			exo2_pere( &donnees );
			wait( NULL );
			exo2_liberation( &donnees );
		}
	else 
		{
			exo2_fils( &donnees );
			exit( EXIT_SUCCESS );
		}
}

void exo2_initialisation( exo2_t * donnees )
{ 
	int err;
	donnees -> df = shm_open("/monSegmentPartage",O_CREAT|O_RDWR,S_IRWXU);
	if (donnees -> df == -1) 
	{ 
		fprintf(stderr, "Erreur lors de l'ouverture de la mémoire partagée\n"); 
		exit(EXIT_FAILURE); 
	}

	err = ftruncate(donnees -> df, sizeof( exo2_partagee ));
	if (err != 0) 
	{
		fprintf( stderr, "Erreur lors du redimensionnement de mémoire partagée\n" );
		exit( EXIT_FAILURE );
	}

	donnees -> partage = (exo2_partagee *) mmap (NULL, sizeof(exo2_partagee), 
				PROT_READ | PROT_WRITE, MAP_SHARED, donnees -> df, 0);
	if (donnees -> partage == MAP_FAILED) 
	{ 
		fprintf(stderr, "Erreur lors de la projection en mémoire\n"); 
		exit(EXIT_FAILURE); 
	}

	err = sem_init(&(donnees->partage->sem), 1, 0);
	if (err != 0) 
	{ 
		fprintf(stderr, "Erreur lors de l'initialisation du sémaphore\n"); 
		exit(EXIT_FAILURE); 
	}
}

void exo2_pere( exo2_t * donnees )
{ 
	int i;
	int tablocal[10] = {3,1,4,1,5,9,6,2,5,3};
	for (i = 0; i < 10; i++) 
	{
		donnees -> partage -> tab[i] = tablocal[i]; 
	}
	sem_post(&(donnees -> partage -> sem));
}

void exo2_fils( exo2_t * donnees )
{ 
	int i;
	sem_wait(&(donnees -> partage -> sem));
	for (i = 0; i < 10; i++)
	{
		printf("%d ; ", donnees -> partage -> tab[i]);
	}
	printf("\n");
}

void exo2_liberation( exo2_t * donnees )
{ 
	sem_destroy(&(donnees -> partage -> sem));
	munmap( donnees -> partage, sizeof(exo2_partagee));
	close(donnees->df);
	shm_unlink("/monSegmentPartage");
}




/************************************************************
 *********************	 Exercice 3		**********************
 ************************************************************
 *
 * On reprend l'exercice 2 à la différence que cette fois-ci, le fils ne va pas
 * attendre que le père ait terminé d'inscrire les dix nombres avant de
 * commencer à afficher les premiers. On souhaite qu'à chaque fois que le père
 * écrit un nombre dans la mémoire partagée, le fils doit pouvoir lire ce nombre
 * et l'afficher, même si les autres nombres n'ont pas encore été écrits.
 *
 * De plus, une fois que le fils a terminé d'afficher les dix nombres, on
 * inverse les rôles. C'est cette fois-ci le fils qui va inscrire les nombres :
 * 2, 7, 1, 8, 2, 8, 1, 8, 2, 8 et c'est le père qui va les afficher un par un à
 * mesure que le fils les inscrits en mémoire. 
 */


typedef struct
{
	sem_t sem; /* Le sémaphore pour que le fils attende */
	sem_t semfinlecture; /* Le père attend que le fils ait fini de lire avant de lire lui aussi */
	int tab[10]; /* Un tableau de 10 entiers */
} exo3_partagee;


typedef struct
{
	exo3_partagee* partage; /* La zone de mémoire partagée */
	int df; /* Référence du fichier virtuel pour le partage */
} exo3_t;


/*
 * Déclarations des fonctions relatives à l'exercice 3
 */
void exo3_initialisation( exo3_t * donnees );
void exo3_pere( exo3_t * donnees );
void exo3_fils( exo3_t * donnees );
void exo3_liberation( exo3_t * donnees );

void exo3()
{
	exo3_t donnees;
	int pid;

	exo3_initialisation( &donnees );
	pid = fork();
	if (pid) 
		{
			exo3_pere( &donnees );
			wait(NULL);
			exo3_liberation( &donnees );
		}
	else 
		{
			exo3_fils( &donnees );
			exit( EXIT_SUCCESS );
		}
}

void exo3_initialisation( exo3_t * donnees ) 
{ 
	int err;
	donnees -> df = shm_open("/monSegmentPartage",O_CREAT|O_RDWR,S_IRWXU);
	if (donnees -> df == -1) 
	{ 
		fprintf(stderr, "Erreur lors de l'ouverture de la mémoire partagée\n"); 
		exit(EXIT_FAILURE); 
	}

	err = ftruncate(donnees -> df, sizeof( exo3_partagee ));
	if (err != 0) 
	{
		fprintf( stderr, "Erreur lors du redimensionnement de mémoire partagée\n" );
		exit( EXIT_FAILURE );
	}

	donnees -> partage = (exo3_partagee *) mmap (NULL, sizeof(exo3_partagee), 
				PROT_READ | PROT_WRITE, MAP_SHARED, donnees -> df, 0);
	if (donnees -> partage == MAP_FAILED) 
	{ 
		fprintf(stderr, "Erreur lors de la projection en mémoire\n"); 
		exit(EXIT_FAILURE); 
	}

	err = sem_init(&(donnees->partage->sem), 1, 0);
	if (err != 0) 
	{ 
		fprintf(stderr, "Erreur lors de l'initialisation du sémaphore sem\n"); 
		exit(EXIT_FAILURE); 
	}

	err = sem_init(&(donnees->partage->semfinlecture), 1, 0);
	if (err != 0) 
	{ 
		fprintf(stderr, "Erreur lors de l'initialisation du sémaphore semfinlecture\n"); 
		exit(EXIT_FAILURE); 
	}
}

void exo3_pere( exo3_t * donnees )
{ 
	int i;
	int tablocal[10] = {3,1,4,1,5,9,6,2,5,3};
	for (i = 0; i < 10; i++) 
	{
		donnees -> partage -> tab[i] = tablocal[i]; 
		sem_post(&(donnees -> partage -> sem));
	}
	sem_wait(&(donnees -> partage -> semfinlecture));
	for (i = 0; i < 10; i++)
	{
		sem_wait(&(donnees -> partage -> sem));
		printf("%d ; ", donnees -> partage -> tab[i]);
	}
	printf("\n");
}

void exo3_fils( exo3_t * donnees )
{ 
	int i;
	int tablocal[10] = {2, 7, 1, 8, 2, 8, 1, 8, 2, 8};
	for (i = 0; i < 10; i++)
	{
		sem_wait(&(donnees -> partage -> sem));
		printf("%d ; ", donnees -> partage -> tab[i]);
	}
	printf("\n");
	sem_post(&(donnees -> partage -> semfinlecture));
	for (i = 0; i < 10; i++) 
	{
		donnees -> partage -> tab[i] = tablocal[i]; 
		sem_post(&(donnees -> partage -> sem));
	}
}

void exo3_liberation( exo3_t * donnees )
{ 
	sem_destroy(&(donnees -> partage -> sem));
	sem_destroy(&(donnees -> partage -> semfinlecture));
	munmap( donnees -> partage, sizeof(exo3_partagee));
	close(donnees->df);
	shm_unlink("/monSegmentPartage");
}


/************************************************************
 *********************	 Exercice 4		**********************
 ************************************************************
 *
 * Implémentation de tubes simplifiés.
 *
 * Complétez les types ``monTube_exo4_t`` ``monTube_exo4_partagee`` définis dans
 * le fichier monTube_exo4.h et complétez les fonctions :
 *
 *	- monTube_exo4_init
 *	- monTube_exo4_ecrire
 *	- monTube_exo4_lire
 *	- monTube_exo4_detruire
 *
 * dans le fichier monTube_exo4.c
 *
 * Dans un premier temps :
 *
 *	- Ne vous souciez pas de la fermeture du tube.
 *
 *	- La taille du tableau utilisé comme tampon de communication est déterminée par
 *	la constante TAILLE_TUBE_EXO4.
 *
 * Attention de vérifier que tous les appels systèmes que vous effectuez se
 * sont bien déroulés. Si ce n'est pas le cas, vos fonctions doivent le
 * signaler (la fonction n'essaie pas de terminer son travail, elle retourne
 * simplement une valeur indiquant qu'une erreur s'est produite).
 *
 */

void exo4_lecture( monTube_exo4_t * mt );
void exo4_ecriture( monTube_exo4_t * mt );

void exo4()
{
	/*
	 * Voici un exemple d'utilisation de ces tubes simplifies
	 */
	monTube_exo4_t mt;
	int pid, err;

	err = monTube_exo4_init( &mt );
	if ( err )
		{
			fprintf( stderr, "Erreur lors de la création du tube\n" );
			exit( EXIT_FAILURE );
		}
	pid = fork();
	if ( pid ) 
		{
			exo4_ecriture( &mt );
			wait(NULL);
			monTube_exo4_detruire( &mt );
		}
	else 
		{
			exo4_lecture( &mt );
			exit( EXIT_SUCCESS );
		}
}


/* 
 * Fonctions pour vous aider à tester votre code de la question 4.
 *
 * Trois phases :
 *
 * 1. lecture lente
 * 2. écriture lente
 * 3. écriture massive ( communication de 10 Mo )
 */
void exo4_lecture( monTube_exo4_t *mt )
{
	char c,test;
	int i;
	for (i=0; i<3; i++) {
			sleep( 1 );
			monTube_exo4_lire( mt, &c );
			printf( "-> %c\n", c );
	}
	for (i=3; i<10; i++) { 
			monTube_exo4_lire( mt, &c );
			printf( "-> %c\n", c );
	}
	sleep(1);
	test = c;
	for (i=10; i<UN_GRAND_NOMBRE; ++i )
		{
			monTube_exo4_lire( mt, &c );
			++test;
			if ( c != test )
				{
					fprintf( stderr, "Erreur de communication\n" );
					exit( EXIT_FAILURE );
				}
			if ( (i+1) % 10000 == 0 )
				{
					printf( "." );
					fflush( stdout );
				}
		}
	printf( "\n%d lectures effectuées avec succès\n", UN_GRAND_NOMBRE );
}

void exo4_ecriture( monTube_exo4_t *mt )
{
	char c = 'a';
	int i;
	printf( "Test de lecture lente\n" );
	for (i=0; i<5; i++) {
			monTube_exo4_ecrire( mt, c++ );
	}
	printf( "5 écritures effectuées\n" );
	sleep( 6 );
	printf( "Test d'écriture lecture lente\n" );
	for (i=5; i<10; i++) {
			monTube_exo4_ecrire( mt, c++ );
			sleep( 1 );
	}
	printf( "Test d'écriture massive\n" );
	for (i=10; i<UN_GRAND_NOMBRE; ++i )
		{
			monTube_exo4_ecrire( mt, c++ );
		}
}




/************************************************************
 *********************	 Exercice 5		**********************
 ************************************************************
 *
 * Implémentation de tubes de taille variable.
 *
 * Implémentez une nouvelle version de vos tubes 
 * et complétez les fonctions :
 *
 *	- monTube_exo5_init
 *	- monTube_exo5_ecrire
 *	- monTube_exo5_lire
 *	- monTube_exo5_detruire
 *
 * dans le fichier monTube.c
 *
 *	- Ne vous souciez pas de la fermeture du tube.
 *
 * Cette fois-ci, on va remplacer le tableau statique en mémoire partagée par
 * un tableau dynamique dont la taille spécifiée par le paramètre ``taille`` de
 * la fonction ``monTube_exo5_init``.
 *
 * Attention de vérifié que toutes les appels systèmes que vous effectués se
 * sont bien déroulés. Si ce n'est pas le cas, vos fonctions doivent le
 * signaler (la fonction n'essaie pas de terminer son travail, elle retourne
 * simplement une valeur indiquant qu'une erreur s'est produite).
 *
 */

void exo5_test( int n );
void exo5_lecture( monTube_exo5_t * mt );
void exo5_ecriture( monTube_exo5_t * mt );

void exo5()
{
	/*
	 * Voici un exemple d'utilisation de ces tubes simplifies
	 */

	printf( "Exo5, test avec un tube de taille 4\n" );
	exo5_test( 4 );
	printf( "Exo5, test avec un tube de taille 512\n" );
	exo5_test( 512 );
	printf( "Exo5, test avec un tube de taille 65536\n" );
	exo5_test( 65536 );
}

void exo5_test( int taille )
{
	int pid, err;
	monTube_exo5_t mt;
	err = monTube_exo5_init( &mt, taille );
	if ( err )
		{
			fprintf( stderr, "Erreur lors de la création du tube\n" );
			exit( EXIT_FAILURE );
		}

	pid = fork();
	if ( pid ) 
		{
			exo5_ecriture( &mt );
			wait(NULL);
			monTube_exo5_detruire( &mt );
		}
	else 
		{
			exo5_lecture( &mt );
			exit( EXIT_SUCCESS );
		}
}


/* 
 * Fonctions pour vous aider à tester votre code de la question 5.
 *
 * Même tests que pour l'exo4
 *
 */
void exo5_lecture( monTube_exo5_t	*mt )
{
	char c,test;
	int i;
	for (i=0; i<3; i++) {
			sleep( 1 );
			monTube_exo5_lire( mt, &c );
			printf( "-> %c\n", c );
	}
	for (i=3; i<10; i++) { 
			monTube_exo5_lire( mt, &c );
			printf( "-> %c\n", c );
	}
	sleep(1);
	test = c;
	for (i=10; i<UN_GRAND_NOMBRE; ++i )
		{
			monTube_exo5_lire( mt, &c );
			++test;
			if ( c != test )
				{
					fprintf( stderr, "Erreur de communication\n" );
					exit( EXIT_FAILURE );
				}
			if ( (i+1) % 10000 == 0 )
				{
					printf( "." );
					fflush( stdout );
				}
		}
	printf( "\n%d lectures effectuées avec succès\n", UN_GRAND_NOMBRE );
}

void exo5_ecriture( monTube_exo5_t *mt )
{
	char c = 'a';
	int i;
	printf( "Test de lecture lente\n" );
	for (i=0; i<5; i++) {
			monTube_exo5_ecrire( mt, c++ );
	}
	printf( "5 écritures effectuées\n" );
	sleep( 6 );
	printf( "Test d'écriture lecture lente\n" );
	for (i=5; i<10; i++) {
			monTube_exo5_ecrire( mt, c++ );
			sleep( 1 );
	}
	printf( "Test d'écriture massive\n" );
	for (i=10; i<UN_GRAND_NOMBRE; ++i )
		{
			monTube_exo5_ecrire( mt, c++ );
		}
}




/************************************************************
 *********************	 Exercice 6		**********************
 ************************************************************
 *
 * Écrivez maintenant votre code de manière à ce qu'un appel à
 * ``monTube_exo6_lire`` effectué sur un tube vide ayant déjà été fermé en
 * écriture ne bloque pas mais retourne immédiatement la valeur 0.
 *
 * Si le tube est fermé alors qu'un processus est déjà bloqué sur un
 * appel à ``monTube_exo6_lire`` on ne s'attend pas à ce que ce processus
 * soit débloqué.
 *
 */

void exo6_lecture( monTube_exo6_t	*mt );
void exo6_ecriture( monTube_exo6_t *mt );

void exo6()
{
	monTube_exo6_t mt;
	int pid;

	monTube_exo6_init( &mt, 4 );
	pid = fork();
	if ( pid ) 
		{
			exo6_ecriture( &mt );
			wait( NULL );
		}
	else 
		{
			exo6_lecture( &mt );
			exit( EXIT_SUCCESS );
		}
}


/* 
 * Fonctions pour vous aider à tester votre code de la question 6.
 */
void exo6_lecture( monTube_exo6_t	*mt )
{
	char c;
	while ( monTube_exo6_lire( mt, &c ) != 0 ) {
			printf( "-> %c\n", c );
			sleep( 1 );
	}
	if ( monTube_exo6_detruire( mt ) != 0 ) {
			fprintf( stderr, "Erreur lors de la destruction du tube\n" );
			exit( EXIT_FAILURE );
	}
}

void exo6_ecriture( monTube_exo6_t *mt )
{
	char c = 'a';
	int i;
	for (i=0; i<10; i++) {
			monTube_exo6_ecrire( mt, c++ );
	}
	if ( monTube_exo6_fermer( mt ) != 0 ) {
			fprintf( stderr, "Erreur lors de la fermeture du tube\n" );
			exit( EXIT_FAILURE );
	}
	printf("tube fermé\n");
}




/************************************************************
 ******************	 Exercice 7 (bonus)	*******************
 ************************************************************
 *
 * Gérez correctement la fermeture du tube, même lorsqu'un processus
 * est déjà bloqué sur une opération de lecture.
 *
 *
 */
void exoB_test( int n );
void exoB_lecture( monTube_exo6_t * mt );
void exoB_ecriture( monTube_exo6_t * mt );

void exoBonus()
{
	/*
	 * Voici un exemple d'utilisation de ces tubes simplifies
	 */

	printf( "ExoB, test avec un tube de taille 4\n" );
	exoB_test( 4 );
	printf( "ExoB, test avec un tube de taille 512\n" );
	exoB_test( 512 );
	printf( "ExoB, test avec un tube de taille 65536\n" );
	exoB_test( 65536 );
}

void exoB_test( int taille )
{
	int pid, err;
	monTube_exo6_t mt;
	err = monTube_exo6_init( &mt, taille );
	if ( err )
		{
			fprintf( stderr, "Erreur lors de la création du tube\n" );
			exit( EXIT_FAILURE );
		}

	pid = fork();
	if ( pid ) 
		{
			exoB_ecriture( &mt );
			wait(NULL);
			monTube_exo6_detruire( &mt );
		}
	else 
		{
			exoB_lecture( &mt );
			exit( EXIT_SUCCESS );
		}
}


/* 
 * Fonctions pour vous aider à tester votre code de la question 5.
 *
 * Même tests que pour l'exo4
 *
 */
void exoB_lecture( monTube_exo6_t	*mt )
{
	char c,test;
	int i;
	for (i=0; i<3; i++) {
			monTube_exo6_lire( mt, &c );
			printf( "-> %c\n", c );
	}
	for (i=3; i<10; i++) { 
			monTube_exo6_lire( mt, &c );
			printf( "-> %c\n", c );
	}
	test = c;
	for (i=10; i<UN_GRAND_NOMBRE; ++i )
		{
			monTube_exo6_lire( mt, &c );
			++test;
			if ( c != test )
				{
					fprintf( stderr, "Erreur de communication\n" );
					exit( EXIT_FAILURE );
				}
			if ( (i+1) % 10000 == 0 )
				{
					printf( "." );
					fflush( stdout );
				}
		}
	printf( "\n%d lectures effectuées avec succès\n", UN_GRAND_NOMBRE );
}

void exoB_ecriture( monTube_exo6_t *mt )
{
	char c = 'a';
	int i;
	printf( "Test de lecture lente\n" );
	for (i=0; i<5; i++) {
			monTube_exo6_ecrire( mt, c++ );
	}
	printf( "5 écritures effectuées\n" );
	sleep( 6 );
	printf( "Test d'écriture lecture lente\n" );
	for (i=5; i<10; i++) {
			monTube_exo6_ecrire( mt, c++ );
			sleep( 1 );
	}
	printf( "Test d'écriture massive\n" );
	for (i=10; i<UN_GRAND_NOMBRE; ++i )
		{
			monTube_exo6_ecrire( mt, c++ );
		}
	if ( monTube_exo6_fermer( mt ) != 0 ) {
			fprintf( stderr, "Erreur lors de la fermeture du tube\n" );
			exit( EXIT_FAILURE );
	}
	printf("tube fermé\n");
}


/************************************************************
 ******************	 Exercice 8 (bonus)	*******************
 ************************************************************
 *
 * Vos tubes sont-ils robuste si plusieurs processus les utilisent simultanément
 * en lecture et en écriture ? Qu'en est-il des appels bloquants lorsque le tube
 * est fermé ?
 */




int main()
{

	/* exo1(); */
	/* exo2(); */
	/* exo3(); */
	/* exo4(); */
	/* exo5(); */
	/* exo6(); */
	 exoBonus(); 

	printf( "Exécution terminée\n" );
	return 0;
}
