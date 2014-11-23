#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <ncurses.h>

/**
 * INFO622 - TD3
 *
 * Le but de cet exercice est de partager plusieurs copies identiques d'une
 * ressource entre un nombre arbitraire de threads.
 *
 * On simule un bureau de poste où des clients arrivent ( par petits groupes )
 * et attendent qu'un comptoir soit libre pour les servir. Lorsqu'un comptoir
 * est libre, un client en attente se rend à se comptoir pour y être servi.
 *
 * La modélisation se fait au moyen d'un tableau d'entier où chaque case
 * représente un comptoir. Lorsqu'un comptoir est vide, sa case prend la valeur
 * ``LIBRE``. Les clients sont représentés par des numéros ( 1, 2, 3, ... ) et
 * lorsqu'un client est à un comptoir, on inscrit son numéro dans la case
 * correspondant à ce comptoir.
 *
 * Voici comment le programme fonctionne :
 *  - L'utilisateur entre des nombre au clavier. Lorsqu'il entre un nombre
 *  ``n`` et ``n`` nouveaux clients sont alors ajoutés à la file d'attente.
 *
 *  - Pour chaque client, un thread est créé. Ce thread attend qu'un comptoir
 *  soit libre. 
 *  
 *  - Lorsque c'est le cas, il va écrire le numéro de son client dans
 *  la case correspondante. Le thread lance ensuite la fonction
 *  ``traiter_client`` qui attends quelques secondes, ce temps d'attente
 *  correspond au traitement de la demande du client.
 *
 *  - Lorsque la fonction ``traiter_client`` termine, on remet le comptoir à
 *  ``LIBRE`` et s'en est terminé pour ce client.
 *
 *  - On maintient également deux variables : 
 *    ``nb_clients_attente`` le nombre de clients en attente. 
 *    ``nb_clients_traites`` le nombre total de clients qui ont été servis.
 *
 *
 * Le programme ci-dessous est fonctionnel mais on remarque deux problèmes
 * majeurs :
 * 
 * 1. L'utilisation de l'attente active pour faire attendre les threads clients.
 *
 * 2. Un horrible condition de concurrence lorsque plusieurs clients attendent
 * et qu'un place se libère.
 *
 * À vous de régler ces deux problèmes !!!
 *
 *
 * Note : l'affichage est géré via la librairie ncurses, vous n'avez pas à
 * modifier quoi que ce soit relativement à l'affichage.
 *
 ********************************************************************************
 ********************************************************************************
 *
 * Pour ceux que cela intéresse ( indépendant du module INFO622 )
 * 
 * La librairie ncurses permet de simuler un affichage graphique via un
 * terminal. Cette librairie ne gère pas le parallélisme. Pour affichager via
 * ncurses, on procède toujours en deux étables : 
 *  - 1 - On place le curseur dans le terminal.
 *  - 2 - On écrit du texte à cet endroit.
 * En répétant les étapes 1 et 2 rapidement, on peut facilement simuler un
 * affichage graphique. Par contre, on voit bien que si deux threads effectuent
 * 1 et 2 en parallèle, le résultat risque fort d'être décevant !
 *
 * L'affichage via ncurses constitue donc une condition de concurrence ! Le
 * programme ci-dessous gère ce problème via l'utilisation d'un mutex qui permet
 * de s'assurer qu'un seul thread à la fois n'interragit avec ncurses.
 *
 ********************************************************************************
 ********************************************************************************
 *
 */


#define NB_COMPTOIRS 4
#define LIBRE -1
#define TAB_MAX 255

 /* temps min et max d'attente, exprimés en microsecondes. */
#define MIN_SLEEP  2000000
#define MAX_SLEEP 12000000

 /* Les deux macros suivantes, servent à rendre plus "lisibles" les casts
  * effectués lors d'un appel à la fonction ``pthread_create``  */
#define FCT_THREAD(fct) ( void * (*) (void * ) ) fct
#define ARG_THREAD(arg) ( void * ) arg

/* Constantes relatives à l'affichage */
#define DELAI_AFFICHAGE 100000
#define LARGEUR_DESSIN 5
#define HAUTEUR_DESSIN 4
#define LARGEUR_CONSOLE 80
#define ESPACE_ENTRE_DESSINS 6
#define DECALAGE_COMPTOIRS(n) (LARGEUR_DESSIN+ESPACE_ENTRE_DESSINS)*n + (ESPACE_ENTRE_DESSINS / 2)
typedef char Dessin[HAUTEUR_DESSIN][LARGEUR_DESSIN+1];
const Dessin bonhomme = { "  O  ", " \\|/  ", "  |  ", " / \\ " };
WINDOW * init_lecture();
WINDOW * init_comptoirs();
int lire_nombre(WINDOW * win, const char * message, pthread_mutex_t * mut_affichage );

/* Données globales du programme */
struct S_donnees_globales
{
  int comptoirs[ NB_COMPTOIRS ]; /* tableau représentant les comptoirs */
	sem_t nb_libre;
	pthread_mutex_t m;
  int nb_clients_attente;        /* nombre de clients présentement en attente */
  int nb_clients_traites;        /* nombre total de clients traités */
  int termine;                   /* indique la fin de l'exécution du programme */

  /* Données relatives à l'affichage */
  pthread_mutex_t * mut_affichage; /* assure l'exclusion mutuelle lors de l'afficahge */
  WINDOW * win_comptoirs;          /* fenêtre où sont affichés les comptoirs et
                                      la file d'attente */
  WINDOW * win_lecture;            /* fenêtre où les nombres entrés par
                                      l'utilisateur sont entrés */

};
typedef struct S_donnees_globales donnees_globales;



/* Structure utilisées pour communiquer de l'information aux threads clients */
struct S_donnees_requete
{
  donnees_globales * dg; /* les données globales du programme */
  int id;                /* numéro qui identifie le client */
};
typedef struct S_donnees_requete donnees_requete;



void traiter_client( donnees_requete *d, int comptoir );


/**
 * Les threads clients sont lancés sur cette fonction.
 * 
 * C'est ici que le cheminement d'un client à travers le bureau de poste est
 * simulé.
 */
void requete( donnees_requete * d )
{
  int i;
  donnees_globales * dg = d->dg;

  /* Première étape : attendre une place libre */

  /* voici une première méthode... avec de l'attente active !!! */
  sem_wait(&(dg -> nb_libre));
  /* C'est horrible mais ça marche... un peu... 
   * en fait, on a ici une horrible condition de concurrence. :-( */


  /* Rendu ici, on sait qu'au moins une place est libre. */
   
  /* Deuxième étape :  on parcout le tableau ``comptoirs`` afin d'identifier un
   * comptoire libre. */
  i = 0;
	pthread_mutex_lock(&(dg -> m));
  while ( dg->comptoirs[i] != LIBRE )
    {
      ++i;
    }
	pthread_mutex_unlock(&(dg -> m));
  /* Le ``i``-ième comptoire est libre ! */

  /* Troisième étable : on marque le comptoir comme étant occupé et on
   * décrémente le nombre de clients en attente */
  dg->comptoirs[i] = d->id;
  dg->nb_clients_attente -= 1;

  /* Quatrième étape, on traite la demande du client */
  traiter_client( d, i );

  /* Cinquième étape, le client quitte le comptoir, on le remet à ``LIBRE`` et
   * on incrémente le nombre de clients traités. */
  dg->comptoirs[i] = LIBRE;
	sem_post(&(dg->nb_libre));
  dg->nb_clients_traites += 1;

  pthread_exit( NULL );
}



/**
 * Gestion de l'arrivé des nouveaux clients.
 *
 * On récupère les nombres entrées au clavier par l'utilisateur et on lance le
 * nombre correspondant de threads sur la fonction ``requete``.
 */
void generer_clients ( donnees_globales * dg )
{
  int id_client = 0;
  int i,n,err;
  pthread_t td;
  donnees_requete dr[ TAB_MAX ];

  n = 0;
  while ( n != -1 )
    {
      for ( i=0; i<n; ++i )
        {
          ++id_client;
          dr[ id_client % TAB_MAX ].dg = dg;
          dr[ id_client % TAB_MAX ].id = id_client;
          dg->nb_clients_attente += 1;
          err = pthread_create( &td, NULL, 
                         FCT_THREAD(requete), 
                         ARG_THREAD( &dr[ id_client % TAB_MAX ] ) );
          if ( err != 0 )
            {
              fprintf( stderr, "Erreur lors de la création d'un thread\n" );
              exit( EXIT_FAILURE );
            }
					else
					{
						printf("Création d'un thread réussie\n");
					}
        }
      n = lire_nombre( dg->win_lecture, 
                      "Entrez le nombre de nouveaux clients ( -1 pour quitter ) : ", 
                      dg->mut_affichage );
    }
  dg->termine = 1;
}



void fct_affichage( donnees_globales * dg );


int main( ) 
{
  int i;
  pthread_t affichage;           /* l'affichage est gérée par un threads indépendant */
  pthread_mutex_t mut_affichage; /* mutex utilisé pour assurer l'exclusion
                                    mutuelle lors de l'affichage */
  donnees_globales dg;

  /* Initialisation des données */
  for ( i=0; i<NB_COMPTOIRS; ++i )
    {
      dg.comptoirs[i] = LIBRE;
    }
  dg.nb_clients_attente = 0; 
  dg.nb_clients_traites = 0;
  dg.termine = 0;
	sem_init(&(dg.nb_libre),0,NB_COMPTOIRS);
	pthread_mutex_init(&(dg.m),NULL);

  /* Initialisation de l'affichage via ncurses */
  pthread_mutex_init( &mut_affichage, NULL );
  dg.mut_affichage = &mut_affichage;
  initscr();
  curs_set(FALSE);
  clear();
  dg.win_lecture = init_lecture();
  dg.win_comptoirs = init_comptoirs();

  /* Lancement du thread d'affichage */
  pthread_create( &affichage, NULL, FCT_THREAD( fct_affichage ) , ARG_THREAD( &dg ) );

  /* La simulation commence ici avec l'appel à ``générer_clients`` */
  generer_clients( &dg );

  /* On attend que le thread d'affichage se termine */
  pthread_join( affichage, NULL );

  return 0;
}



/**
 * La simulation du traitement de la requête d'un client se fait simplement avec
 * un ``sleep`` d'une durée aléatoire.
 */
void traiter_client( donnees_requete *d, int comptoir )
{
  fprintf( stderr, "Client %d arrive au comptoir %d\n", d->id, comptoir );
  usleep( MIN_SLEEP + ( rand() % ( MAX_SLEEP - MIN_SLEEP ) ) ) ;
  fprintf( stderr, "Client %d quitte le comptoir %d\n", d->id, comptoir );
}







/***************************
 * Gestion de l'affichage
 ****************************/

/*
 * Initialisation d'un fenêtre ncurses avec une bordure.
 */
WINDOW *nouvelle_fenetre(int hauteur, int largeur, int debuty, int debutx)
{
  WINDOW *win;
  win = newwin(hauteur, largeur, debuty, debutx);
  box(win, 0 , 0);   /* Pour afficher une bordure */
  wrefresh(win);     /* Pour afficher la bordure */
  return win;
}

/*
 * Libération des ressources détenues par une fenêtre ncurses. 
 */
void detruire_fenetre(WINDOW *win)
{
  /* Destruction de la fenetre pour libérer les ressources */
  delwin(win);
}


void dessine_comptoir_vide( WINDOW * win, int pos )
{
  int i,j,x,y;
  for ( i=0; i<HAUTEUR_DESSIN; ++i ) 
    {
      for ( j=0; j<LARGEUR_DESSIN; ++j ) 
        {
          x = DECALAGE_COMPTOIRS( pos ) + j;
          y = i+2;
          wmove( win, y, x );
          waddch( win, ' ' );
        }
    }
  wmove( win, 1, DECALAGE_COMPTOIRS( pos ) );
  wprintw( win, "   " );
}

void dessine_comptoir_occupe( WINDOW * win, int pos, int id_client )
{
  int i,j,x,y;
  for ( i=0; i<HAUTEUR_DESSIN; ++i ) 
    {
      for ( j=0; j<LARGEUR_DESSIN; ++j ) 
        {
          x = DECALAGE_COMPTOIRS( pos ) + j;
          y = i+2;
          wmove( win, y, x );
          waddch( win, bonhomme[i][j] );
        }
    }
  wmove( win, 1, DECALAGE_COMPTOIRS( pos ) );
  wprintw( win, "%3d", id_client );
}


void maj_affichage( WINDOW * win, donnees_globales * dg )
{
  int i;
  pthread_mutex_lock( dg->mut_affichage );
  for ( i=0; i<NB_COMPTOIRS; ++i )
    {
      if ( dg->comptoirs[i] == LIBRE )
        {
          dessine_comptoir_vide( win, i );
        }
      else
        {
          dessine_comptoir_occupe( win, i, dg->comptoirs[i] );
        }
    }

  wmove( win, HAUTEUR_DESSIN+6, 15 );
  wprintw( win, "%3d", dg->nb_clients_attente );
  wrefresh(win);
  pthread_mutex_unlock( dg->mut_affichage );
}


/*
 * Le dessin des comptoirs dans la fenêtre du haut. Elle contient NB_COMPTOIRS 
 * emplacements.
 */ 
WINDOW * init_comptoirs()
{
  int hauteur = HAUTEUR_DESSIN + 8;
  int largeur = (LARGEUR_DESSIN + ESPACE_ENTRE_DESSINS) * NB_COMPTOIRS;
  int debutx  = 0;
  int debuty  = LINES - ( 5 + hauteur );
  int pos;
  WINDOW * win = nouvelle_fenetre(hauteur, largeur, debuty, debutx);

  for ( pos=0; pos<NB_COMPTOIRS; ++pos )
    {
      wmove(win, HAUTEUR_DESSIN+2, DECALAGE_COMPTOIRS( pos ));
      wprintw( win , "=====" );
      wmove(win, HAUTEUR_DESSIN+3, DECALAGE_COMPTOIRS( pos ));
      wprintw( win , "|| ||" );
    }
  wmove( win, HAUTEUR_DESSIN+6, 2 );
  wprintw( win, "En attente : ");
  wrefresh(win);
  return win;
}


/*
 * La fenêtre de lecture fait seulement une ligne de haut (3 avec la bordure)
 * sert à lire les nombres entrés par l'utilisateur
 */ 
WINDOW * init_lecture()
{
  int hauteur = 3;
  int largeur = LARGEUR_CONSOLE;
  int debutx  = 0;
  int debuty  = LINES - 3;
  return nouvelle_fenetre(hauteur, largeur, debuty, debutx);
}

/**
 * L'affichage à partir des données globales et est actualisé à chaque seconde.
 */
void fct_affichage( donnees_globales * dg )
{
  while ( dg->termine == 0 )
    {
      maj_affichage( dg->win_comptoirs, dg );
      usleep( DELAI_AFFICHAGE );
    }

  /* Libération des ressources */
  detruire_fenetre( dg->win_comptoirs );
  endwin();
  pthread_exit( NULL );
}


/**
 * Renouvelle l'affichage de la fenêtre de lecture et demande à l'utilisateur
 * d'entrer un nombre.
 */
int lire_nombre(WINDOW * win, const char * message, pthread_mutex_t * mut_affichage )
{
  int tmp;
  pthread_mutex_lock( mut_affichage );
  wclear(win);
  box(win, 0, 0);
  wmove(win, 1,2);
  wprintw( win, "%s" , message);
  wrefresh(win);
  pthread_mutex_unlock( mut_affichage );
  wscanw(win, "%d", & tmp);
  return tmp;
}



