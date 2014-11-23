#include "gravite.h"

#define TAILLE_TAMPON 255

/**
 * INFO 622 TP3
 * De Roland Céline et Leclaire Juliana
 * L3-STIC-INFO-3-2
 */

/**
 * La fonction ci-dessous sert à découper un intervalle sous-intervalles de
 * taille égale.
 *
 * Paramètres : 
 * - debut_si - ( sortie ) adresse de la variable où sera écrit le début du
 *							sous-intervalle.
 * - fin_si - ( sortie ) adresse de la variable où sera écrit la fin du
 *						sous-intervalle (un de plus que le dernier).
 * - debut - debut de l'intervalle à découper.
 * - fin - fin de l'intervalle à découper (un de plus que le dernier).
 * - N - nombre de sous-intervalles.
 * - k - indice du sous-intervalle demandeé.
 *
 * Exemple, pour découper [4,5...,10,11] en 3 sous-intervalles, on fera :
 *
 *	- sousIntervalle( &x, &y, 4, 12, 3, 0 ) --> [x, ..., y-1] == [4, 5, 6]
 *	- sousIntervalle( &x, &y, 4, 12, 3, 1 ) --> [x, ..., y-1] == [7, 8, 9]
 *	- sousIntervalle( &x, &y, 4, 12, 3, 2 ) --> [x, ..., y-1] == [10, 11]
 */
void sousIntervalle( int * debut_si, int * fin_si, int debut, int fin , int N, int k )
{
	int taille = fin - debut;
	*debut_si = debut + ( k*taille / N );
	*fin_si = debut + ( (k+1)*taille / N );
}



/**
 * Initialisation du système à partir d'un fichier de description.
 * La description de chacun des corps célestes est lue dans le fichier et les
 * données sont initialisées conséquemment.
 */
void initSysteme( Systeme * S, const char * fichier )
{
	int i,err;
	FILE * donnees = fopen( fichier, "r" ); /* ouverture du fichier en lecture */
	char tampon[ TAILLE_TAMPON ];
	S->tabThreads = malloc(NBTHREADS * sizeof(pthread_t));
	Data* data;
	S->dist_min = ZERO; /* valeur par défaut, sera remplacée par un appel à 'setDistMin' */

	/* la première ligne du fichier doit indiquer le nombre de corps célestes à
	 * lire */
	fgets( tampon, TAILLE_TAMPON, donnees); 
	sscanf( tampon, "%d", & S->nb_corps ); 
	S->nb_collisions = 0;

	/* allocation des différents tableaux. */
	S->lesMasses =		 ( Flottant * ) malloc( S->nb_corps*sizeof(Flottant) );
	S->posActuelles =	( Point * )		malloc( S->nb_corps*sizeof(Point) );
	S->vitesses =			( Vecteur * )	malloc( S->nb_corps*sizeof(Vecteur) );
	S->nouvellesPos =	( Point * )		malloc( S->nb_corps*sizeof(Point) );

	/* Initialisation du mutex */
	err = pthread_mutex_init(&(S->mutexFusion), NULL);
	if(err != 0){
		fprintf(stderr, "Erreur lors de l'initialisation du mutex mutexFusion\n");
		exit(EXIT_FAILURE);
	}

	/* Initialisation du sem */
	err = sem_init(&(S->travailleSimu), 0, 0);
	if(err == -1){
		fprintf(stderr, "Erreur lors de l'initialisation du sémaphore travailleSimu\n");
		exit(EXIT_FAILURE);
	}
	err = sem_init(&(S->arreteSimu), 0, 0);
	if(err == -1){
		fprintf(stderr, "Erreur lors de l'initialisation du sémaphore arreteSimu\n");
		exit(EXIT_FAILURE);
	}

	S-> arrete = 0;

	/* Initialisation des threads */
	for(i=0; i<NBTHREADS; i++){
		data = malloc(sizeof(Data));
		data->systeme = S;
		sousIntervalle( &(data->debIntervalle), &(data->finIntervalle), 0, S->nb_corps , NBTHREADS, i);
		err = pthread_create(&(S->tabThreads[i]), NULL, (void*(*)(void *))threadAction, (void*)data);
		if(err != 0){
			fprintf(stderr, "Erreur lors de la création des threads\n");
			exit(EXIT_FAILURE);
		}
	}

	/* lecture de chacun des corps célestes */
	i = 0;
	while ( ( i < S->nb_corps ) && fgets( tampon, TAILLE_TAMPON, donnees) )
		{
			sscanf( tampon, "%Lf %Lf %Lf %Lf %Lf", 
						 &( S->lesMasses[i] ),
						 &( S->posActuelles[i].x ),
						 &( S->posActuelles[i].y ),
						 &( S->vitesses[i].x ),
						 &( S->vitesses[i].y ) );
			++i;
		}

	/* affichage de l'état initial du système */
	printf( "État initial : \n" );
	for ( i=0; i<S->nb_corps; ++i )
		{
			printf( "%2d. - M=%Lg, pos=(%Lg ; %Lg), v=(%Lg ; %Lg)\n", 
			 i,
			 S->lesMasses[i], 
			 S->posActuelles[i].x,
			 S->posActuelles[i].y,
			 S->vitesses[i].x,
			 S->vitesses[i].y );
		}
	printf("fin initialisation\n");
}

void setDistMin ( Systeme * S, Flottant dist_min )
{
	S->dist_min = dist_min;
}

/**
 * Calcule la distance entre deux points.
 */
inline
Flottant distance( Point P, Point Q )
{
	return sqrt( pow( P.x - Q.x, 2.0 ) + pow( P.y - Q.y, 2.0 ) );
}

/**
 * Étant donné deux points P et Q, retourne le vecteur normalisé indiquant la
 * direction de Q à partir de P.
 */
inline
void direction ( Vecteur * dir, Point P, Point Q )
{
	Flottant norme, dx, dy;
	dx = Q.x - P.x;
	dy = Q.y - P.y;
	norme = sqrt( dx*dx + dy*dy );
	dir->x = dx / norme;
	dir->y = dy / norme;
}

/**
 * Fusionne deux corps en respectant les masses et la quantité de mouvement.
 * Par convention, le premier corps absorbe le second. Ainsi le second voit sa
 * masse fixée à 0 indiquant qu'il n'existe plus.
 */
void fusionnerCorps( Systeme * S, int i, int j )
{
	Flottant m_i, m_j, m_tot;
	printf( "Collision %d des corps %d et %d\n", S->nb_collisions, i, j );

	m_i = S->lesMasses[i];
	m_j = S->lesMasses[j];
	m_tot = m_i + m_j;


	/* mise à jour des masses */
	S->lesMasses[i] = m_tot;
	S->lesMasses[j] = ZERO;

	/* mise a jour de la vitesse du premier, il n'est pas nécessaire de modifier
	 * la vitesse du second puisqu'il n'existe plus */
	S->vitesses[i].x = ( m_i * S->vitesses[i].x + m_j * S->vitesses[j].x ) / m_tot;
	S->vitesses[i].y = ( m_i * S->vitesses[i].y + m_j * S->vitesses[j].y ) / m_tot;

	S->nb_collisions += 1;
}

/**
 * Mise à jour de la position et de la vitesse du i-ème corps.
 * La mise a jour se fait sur l'intervalle de temps 'S->delta_t'.
 */
void majCorps( Systeme * S, int i )
{
	Flottant d,fg;
	Vecteur direction_relative, fg_vec, acc_vec;
	int j,err;

	Point P = S->posActuelles[i];
	Vecteur V = S->vitesses[i];
	Flottant delta_t = S->delta_t;

	/* si la masse du corps est ZERO c'est qu'il n'existe plus. On ne fait rien */
	if ( S->lesMasses[i] == ZERO )
		return;

	/* fg_vec est le vecteur force, il est mis a jour en fonction de chacun des
	 * autres corps présents dans le système */
	fg_vec.x = ZERO;
	fg_vec.y = ZERO;
	for ( j=0; j<S->nb_corps; ++j )
		{
			if ( ( i != j )	&& ( S->lesMasses[j] != ZERO ) )
				{
					d = distance( P, S->posActuelles[j] );
		if ( d < S->dist_min ) 
			{
				err = pthread_mutex_lock(&(S->mutexFusion));	
				if(err != 0){
					fprintf(stderr, "Erreur lors du blocage du mutex mutexFusion\n");
					exit(EXIT_FAILURE);
				}			
				fusionnerCorps( S, i, j );
				err = pthread_mutex_unlock(&(S->mutexFusion));	
				if(err != 0){
					fprintf(stderr, "Erreur lors du déblocage du mutex mutexFusion\n");
					exit(EXIT_FAILURE);
				}			
			}
		else
			{
				fg = G * S->lesMasses[i] * S->lesMasses[j] / ( d*d ); 
							/* Le vecteur force est mise à jour en décomposant la force
							 * gravitationnelle en deux composantes (x,y) en fonction de la
							 * direction relative des deux corps */
							direction( &direction_relative, P, S->posActuelles[j] );
							fg_vec.x += fg * direction_relative.x;
							fg_vec.y += fg * direction_relative.y;
			}
				}
		}
	if(S->lesMasses[i]){
		acc_vec.x = fg_vec.x / S->lesMasses[i];
		acc_vec.y = fg_vec.y / S->lesMasses[i];
	}
	else{
		acc_vec.x = 0;
		acc_vec.y = 0;
	}
	S->vitesses[i].x = V.x + ( acc_vec.x * delta_t );
	S->vitesses[i].y = V.y + ( acc_vec.y * delta_t );
	S->nouvellesPos[i].x = S->posActuelles[i].x + ( V.x * delta_t );
	S->nouvellesPos[i].y = S->posActuelles[i].y + ( V.y * delta_t );
}



/**
 * Effectue un pas de simulation. Chaque corps doit être mis à jour.
 */
void simule_un_pas ( Systeme * S, Flottant delta_t )
{
	Point * pointsTmp;
	int i,err;

	S->delta_t = delta_t;

	/* Les threads peuvent commencer à travailler */
	for ( i=0; i < NBTHREADS; ++i )
		{
			err = sem_post(&(S->travailleSimu));
			if(err == -1){
				fprintf(stderr, "Erreur lors du post du sémaphore travailleSimu\n");
				exit(EXIT_FAILURE);
			}
		}

	/* Attente pour que tous les threads aient fini de travailler */
	for ( i=0; i < NBTHREADS; ++i )
		{
			err = sem_wait(&(S->arreteSimu));
			if(err == -1){
				fprintf(stderr, "Erreur lors du wait du sémaphore arreteSimu\n");
				exit(EXIT_FAILURE);
			}
		}

	pointsTmp = S->posActuelles;
	S->posActuelles = S->nouvellesPos;
	S->nouvellesPos = pointsTmp;
}

void threadAction( void* d)
{
	int i, err;
	Data* data = (Data*) d;
	while(data->systeme->arrete == 0) {
		/* On attend que simule_un_pas avertisse le départ pour les threads */
		err = sem_wait(&(data->systeme->travailleSimu));
		if(err == -1){
			fprintf(stderr, "Erreur lors du wait du sémaphore travailleSimu\n");
			exit(EXIT_FAILURE);
		}
		for(i = data->debIntervalle; i < data->finIntervalle; i++){
			majCorps(data->systeme, i);
		}
		/* On indique que le thread a fini son intervalle de travail */
		err = sem_post(&(data->systeme->arreteSimu));
		if(err == -1){
			fprintf(stderr, "Erreur lors du post du sémaphore arreteSimu\n");
			exit(EXIT_FAILURE);
		}
	}
	/* On indique que le systeme s'est arreté */
	err = sem_post(&(data->systeme->arreteSimu));
	if(err == -1){
		fprintf(stderr, "Erreur lors du post du sémaphore arreteSimu\n");
		exit(EXIT_FAILURE);
	}
	free(data);
}



/**
 * Libération des ressources occupées par le systèmes
 */
void detruireSysteme( Systeme * S )
{
	int i,err;
	S->arrete == 1;
	/* Si le système s'est arreté les threads finissent de travailler */
	for ( i=0; i < NBTHREADS; ++i )
		{
			err = sem_post(&(S->travailleSimu));
			if(err == -1){
				fprintf(stderr, "Erreur lors du post du sémaphore travailleSimu\n");
				exit(EXIT_FAILURE);
			}
		}
	/* On attend la fin de tous les threads pour pouvoir détruire les ressources */
	for ( i=0; i < NBTHREADS; ++i )
		{
			err = sem_wait(&(S->arreteSimu));
			if(err == -1){
				fprintf(stderr, "Erreur lors du wait du sémaphore arreteSimu\n");
				exit(EXIT_FAILURE);
			}
		}
	free( S->lesMasses );
	free( S->posActuelles );
	free( S->vitesses);
	free( S->nouvellesPos );
	sem_destroy(&(S->travailleSimu));
	sem_destroy(&(S->arreteSimu));
	pthread_mutex_destroy(&(S->mutexFusion));
	free(S->tabThreads);
}


/**
 * Affiche les données du système dans la sortie standard.
 */
void afficherSysteme( Systeme * S)
{
	int i;
	Vecteur quantite_mouvement;
	quantite_mouvement.x = ZERO;
	quantite_mouvement.y = ZERO;
	printf("%d\n", S->nb_corps );
	for ( i=0; i < S->nb_corps; ++i )
		{
			if ( S->lesMasses[i] )
				{
					printf( "%Lg %Lg %Lg %Lg %Lg\n", 
						 S->lesMasses[i],
						 S->posActuelles[i].x,
						 S->posActuelles[i].y,
						 S->vitesses[i].x,
						 S->vitesses[i].y );
					quantite_mouvement.x += S->vitesses[i].x * S->lesMasses[i];
					quantite_mouvement.y += S->vitesses[i].y * S->lesMasses[i];
				}
			else
				{
					printf("0 0 0 0 0\n");
				}
		}
	printf( "Quantité de mouvement : %Lg %Lg\n", quantite_mouvement.x, quantite_mouvement.y );
}


