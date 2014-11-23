/**
	Juliana Leclaire, Céline de Roland
	INFO622 TP3
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <gtk/gtk.h>
#include "gravite.h"

#define RAYON_MAX 8.0L
#define RAYON_MIN 1.0L

#define TAILLE_TAMPON 255

#define DELAI_ENTRE_IMAGES_DEFAUT 2
#define NB_ITER_ENTRE_IMAGES_DEFAUT 15
#define DELTA_T_DEFAUT 60
#define DIMENSION_IMAGE_DEFAUT 700


/* Les données globales du systèmes sont contenues dans un instance de la struc
 * suivante :
 */
typedef struct
{
  /* Le système gravitationnel */
  Systeme * S;

  /* zone de dessin */
  GtkWidget * drawing_area;


  /* Étiquette de délai entre les étapes de la simulation */
  GtkWidget * labelValeurDelai;
  /* Temps d'attente entre deux incréments de la simulation */
  int delai_entre_dessins;


  /* Étiquette de itérations entre les dessin */
  GtkWidget * labelValeurIter;
  /* Nombre d'itérations entre deux dessins */
  int nb_iter;


  /* Étiquette de la durée d'une itération */
  GtkWidget * labelValeurDT;
  /* Durée d'une itération */
  Flottant delta_t;

  /* Étiquette du nombre de pas par secondes */
  GtkWidget * labelValeurPPS;

  /* Étiquette du nombre de coprs du système */
  GtkWidget * labelValeurNbCorps;

  /* Étiquette de la durée d'une itération */
  GtkWidget * labelDuree;
  /* Durée d'une itération */
  Flottant duree;


  /* Indique si la simulation est cours d'exécution ou non */
  gboolean enExecution;

  /* taille de la zone de dessin */
  int taille_dessin;

  /* masse de la plus gros corps à l'initialisation du système */
  Flottant masse_max;

  /* Distance, en norme 1, de l'objet le plus éloigné du centre à
   * l'initialisation. Sert à déterminer le facteur de renormalisation pour lors
   * du dessin.  */
  Flottant coord_max;

  /* Nom du fichier décrivant l'état initial du système */
  const char * fichierSource;

} Donnees; 


/**
 * Indique la syntaxe d'un appel à l'utilisateur.
 */
void usage( const char * nom )
{
  fprintf( stderr, "Usage : %s fichier [options]  \n", nom );
  fprintf( stderr, "  Le fichier spécifié doit contenir la"
          " description d'un système\n");
  fprintf( stderr, "  gravitationnel.\n\n" );
  fprintf( stderr, "  Options :\n" );
  fprintf( stderr, "   -h           Affiche cet aide.\n");
  fprintf( stderr, "   -d delai     Spécifie la valeur initiale"
         " du délai entre"
         "deux images (défaut : %d)\n", DELAI_ENTRE_IMAGES_DEFAUT );
  fprintf( stderr, "   -i nb_iter   Spécifie la valeur initiale du nombre"
          " d'itérations entre deux\n                images (défaut : %d)\n",
          NB_ITER_ENTRE_IMAGES_DEFAUT );
  fprintf( stderr, "   -dt delta_t  Spécifie la valeur initiale de delta_t,"
          " c'est-à-dire le temps\n                écoulé pendant un pas de"
          " simulation ( defaut : %d)\n", DELTA_T_DEFAUT );
  fprintf( stderr, "   -t taille    Spécifie la taille de la zone de dessin"
          " (défaut : 700)\n");
  fprintf( stderr, "\n" );

}


/**
 * Inscrit l'entier 'x' dans le Gtk_label 'widget'.
 */
void intDansLabel( GtkWidget *widget, int x )
{
  char tmp[ 10 ];
  sprintf( tmp, "%d", x );
  gtk_label_set_text( GTK_LABEL( widget ), tmp );
}


/**
 * Inscrit le flottant 'x' dans le Gtk_label 'widget'.
 */
void flottantDansLabel( GtkWidget *widget, Flottant x )
{
  char tmp[ TAILLE_TAMPON ];
  sprintf( tmp, "%Lf", x );
  gtk_label_set_text( GTK_LABEL( widget ), tmp );
}


void construireIHM( Donnees * D );
Flottant CoordMax( Point lesPoints[], int n );
Flottant tabFlottantMax( Flottant lesMasses[], int n );

int main( int argc, char ** argv )
{
  int i;
  Donnees D;
  Systeme S;

  /* Passe les arguments à GTK, pour qu'il extrait ceux qui le concernent. */
  gtk_init (&argc, &argv);

  /* initialisation des valeurs par défaut */
  D.S = &S;
  D.delai_entre_dessins = DELAI_ENTRE_IMAGES_DEFAUT;
  D.nb_iter = NB_ITER_ENTRE_IMAGES_DEFAUT;
  D.delta_t = DELTA_T_DEFAUT;
  D.taille_dessin = DIMENSION_IMAGE_DEFAUT;
  D.duree = ZERO;
  D.enExecution = FALSE;

  D.fichierSource = NULL;
  /* Lecture des arguments de la ligne de commande */
  for ( i=1; i<argc; ++i )
    {
      if ( strcmp( argv[i], "-h" ) == 0 )
        {
          usage( argv[0] );
          exit( EXIT_SUCCESS );
        }
      if ( i == 1 )
        {
          D.fichierSource = argv[1];
        }
      else if ( strcmp( argv[i], "-d" ) == 0 )
        {
          D.delai_entre_dessins = atoi( argv[++i] );
        }
      else if ( strcmp( argv[i], "-i" ) == 0 )
        {
          D.nb_iter = atoi( argv[++i] );
        }
      else if ( strcmp( argv[i], "-dt" ) == 0 )
        {
          D.delta_t = atof( argv[++i] );
        }
      else if ( strcmp( argv[i], "-t" ) == 0 )
        {
          D.taille_dessin = atoi( argv[++i] );
        }
    }
  if ( D.fichierSource == NULL )
    {
      usage( argv[0] );
      exit( EXIT_FAILURE );
    }
  initSysteme( D.S, D.fichierSource ); 


  D.masse_max = tabFlottantMax( S.lesMasses, S.nb_corps );
  D.coord_max = 1.2L * CoordMax( S.posActuelles, S.nb_corps );

  /* mise en place de l'interface graphique */
  construireIHM( &D );


  /* Évaluation de la distance de collision. En gros, on dit qu'il y a collision
   * lorsque la distance entre deux corps est de l'ordre d'un pixel. Cette
   * distance est immense et crée beaucoup plus de collision qu'il ne devrait.
   * D'un autre côté, c'est visuellement acceptable et, surtout, cela évite les
   * erreurs d'approximations qui deviennent immenses lorsque deux corps sont
   * près l'un de l'autre. */
  setDistMin( &S, ( 1.5L * D.coord_max ) / D.taille_dessin );

  gtk_main();
  return 0;
}

/** 
 * Les corps célestes sont représentés par des cercles. Plus un corps est massif
 * plus son cercle est grand. Malheureusement, si on utilise des tailles
 * correspondant à la réalité (i.e. rayon proportionnel à la racine cubique de
 * la masse) le résultat visuel pour notre système solaire est décevant : on ne
 * voit que le soleil, tous les autres corps étant de rayon inférieurs à un
 * pixel. On utilise donc un calcul qui n'a rien à voir avec la physique en
 * fixant un rayon proportionnel à la racine 10e de la masse pour les corps
 * massifs et on fixe le rayon à 1.0 pour les corps légers.
 */
Flottant calculeRayon( Flottant masse, Flottant masse_max )
{
  static Flottant premiereFois = 1;
  static Flottant ajustement;
  Flottant r;
  if ( premiereFois ) 
    {
      ajustement = RAYON_MAX / powl( masse_max, 0.1L );
      premiereFois = 0;
    }
  r = powl( masse, 0.1L ) * ajustement;
  return ( r < RAYON_MIN ) ? RAYON_MIN : r ;
}


/**
 * Mise à jour de l'affichage.
 * On efface toute l'image et on dessine chacun des corps célestes.
 */
gboolean expose_evt_reaction( GtkWidget *widget, GdkEventExpose *event, Donnees * D )
{
  int i;
  cairo_t * cr;
  Flottant rayon, dx, dy, px, py;
  Systeme * S = D->S;

  (void)( event ); /* évite un warning pour paramètre non utilisé */

  /* (dx,dy) est le point de l'image où est dessiné le (0,0) du système. Tous
   * les corps doivent donc être translatés de (dx,dy). */
  dx = D->taille_dessin/2.0L;
  dy = D->taille_dessin/2.0L;

  /* cr est la structure qui permet d'afficher dans une zone de dessin via Cairo  */
  cr = gdk_cairo_create (widget->window);
  cairo_set_source_rgb (cr, 1.0, 1.0, 1.0); /* choisit le blanc. */
  cairo_paint( cr ); /* remplit tout dans la couleur choisie.*/

  /* on dessin les corps un par un. */
  for ( i=0; i < S->nb_corps; ++i )
    {
      /* Si un corps à masse nulle, c'est qu'il n'existe plus soit parce qu'il a
       * quitté le système ou suite à une collision */
      if ( S->lesMasses[i] != ZERO ) 
	{
          rayon = calculeRayon( S->lesMasses[i], D->masse_max );
          cairo_set_source_rgb( cr, 0.0, 0.0, 1.0 ); /* choisi le bleu */
	  px = dx + ( S->posActuelles[i].x / D->coord_max * D->taille_dessin / 2.2L );
	  py = dy + ( S->posActuelles[i].y / D->coord_max * D->taille_dessin / 2.2L );
	  cairo_arc(cr, px, py, rayon, 0, 2 * M_PI); /* dessin un cercle */
	  cairo_fill(cr);
	}
    }

  /* On a fini, on peut détruire la structure. */
  cairo_destroy (cr);
  return TRUE;
}

/* mise en forme de l'étiquette indiquant depuis combien de temps la simulation
 * est en cours. */
void maj_duree( GtkWidget *label, Flottant * duree, Flottant delta_t )
{
  char tmp[ TAILLE_TAMPON ];
  Flottant t,s;
  int a,j,h,m;
  *duree += delta_t;
  t = *duree;

  a  = (int)( t / 31536000.0L );
  t -= ( (Flottant) a ) * 31536000.0L;
  j  = (int)( t / 86400.0L );
  t -= (Flottant) ( j * 86400 );
  h  = (int)( t / 3600 );
  t -= (Flottant) ( h * 3600 );
  m  = (int)( t / 60.0L );
  s = t - (Flottant) ( m * 60 );

  if ( a > 0 )
    sprintf ( tmp, "Temps écoulé : %d a, %3d d, %2d h, %2d m, %2.2Lf s", a, j, h, m, s );
  else if ( j > 0 )
    sprintf ( tmp, "Temps écoulé : %3d d, %2d h, %2d m, %2.1Lf s", j, h, m, s );
  else if ( h > 0 )
    sprintf ( tmp, "Temps écoulé : %2d h, %2d m, %2.1Lf s", h, m, s );
  else if ( m > 0 )
    sprintf ( tmp, "Temps écoulé : %2d m, %2.2Lf s", m, s );
  else 
    sprintf ( tmp, "Temps écoulé : %2.2Lg s", s );
  gtk_label_set_text( GTK_LABEL( label ), tmp );
}

/**
 * Mise-à-jour du gtk_label indiquant le nombre de pas de simulations calculés
 * en une seconde.
 *
 * Malheureusement, ceci est calculé à l'aide de la fonction 'clock' qui est
 * très peu précise.
 */
void maj_PPS( GtkWidget *label, clock_t temps, int nb_pas )
{
  double pas_par_tic = ( (double) nb_pas ) / ( (double) temps );
  double pas_par_sec = pas_par_tic * (double) CLOCKS_PER_SEC;
  if ( temps > 0 )
    intDansLabel( label, (int) pas_par_sec );
  else
    intDansLabel( label, 0 );
}

/**
 * Mise à jour du gtk_label indiquant le nombre de corps célestes dans le
 * système.
 */
void maj_NbCorps( GtkWidget *label, int nb_corps )
{
  intDansLabel( label, nb_corps );
}


/**
 * Effectue un cycle de simulation. Un cycle consiste à faire évoluer le système
 * de 'D->nb_iter' pas de simulations pour ensuite mettre à jour l'affichage.
 */
gboolean unCycle( Donnees * D )
{
  int i;
  clock_t avant,apres;
  avant = clock();
  for ( i=0; i < D->nb_iter; ++i )
    {
      simule_un_pas( D->S, D->delta_t );
    }
  apres = clock();

  /* maj de l'affichage */
  gtk_widget_queue_draw( D->drawing_area );

  /* maj des étiquettes */
  maj_duree( D->labelDuree, &D->duree, D->delta_t*D->nb_iter );
  maj_PPS( D->labelValeurPPS, apres - avant, D->nb_iter );
  maj_NbCorps( D->labelValeurNbCorps, D->S->nb_corps - D->S->nb_collisions );


  /* Si la simulation est toujours en cours d'exécution, on relance un autre
   * cycle qui débutera dans 'D->delai_entre_dessins'o */
  if ( D->enExecution )
    g_timeout_add ( D->delai_entre_dessins, (GSourceFunc) unCycle, (gpointer) D );
  return FALSE;
}

/**
 * Lance la simulation. Pour se faire, il suffit de mettre 'D->enExecution' à
 * vrai et appeler la fonction 'unCycle'
 */
gboolean lancer( GtkWidget *window, Donnees * D )
{
  (void)( window ); /* évite un warning pour paramètre non utilisé */
  if ( ! D->enExecution )
    {
      D->enExecution = TRUE;
      g_timeout_add ( D->delai_entre_dessins, 
		     (GSourceFunc) unCycle, 
		     (gpointer) D );
    }
  return TRUE;
}

/**
 * Pour interrompre la simulation, on n'interrompt jamais un cycle de calcul. On
 * met simplement 'D->enExecution' à faux. Ainsi lorsque le cycle sera terminé,
 * on n'en relancera pas de suivant. ( voir la fonction 'unCycle' )
 */
gboolean stopper( GtkWidget *window, Donnees * D )
{
  (void)( window ); /* évite un warning pour paramètre non utilisé */
  D->enExecution = FALSE;
  return TRUE;
}

/**
 * Demande au système gravitationnel de s'affiche en mode texte.
 */
gboolean afficher( GtkWidget *window, Donnees * D )
{
  (void)( window ); /* évite un warning pour paramètre non utilisé */
  afficherSysteme( D->S );
  return TRUE;
}

/**
 * Réinitialisatin du système à partir du fichier source. La simulation en cours
 * est donc perdu.
 *
 * Ne fonctionne que si on a d'abord arrêté la simulation.
 */
gboolean reInit( GtkWidget *window, Donnees * D )
{
  (void)( window ); /* évite un warning pour paramètre non utilisé */
  if ( ! D->enExecution )
    {
      D->enExecution = FALSE;
      D->duree = ZERO;
      maj_duree( D->labelDuree, &D->duree, ZERO );
      detruireSysteme( D->S );
      initSysteme( D->S, D->fichierSource );
      setDistMin( D->S, RAYON_MIN * ( 1.5L * D->coord_max ) / D->taille_dessin );
      gtk_widget_queue_draw( D->drawing_area );
    }
  else
    {
      fprintf( stderr, "Arrêtez d'abord la simulation\n" );
    }
  return TRUE;
}

/**
 * Recadre la zone de dessin en mettant à jour 'D->coordMax'
 */
gboolean recadrer( GtkWidget *window, Donnees * D )
{
  (void)( window ); /* évite un warning pour paramètre non utilisé */
  D->coord_max = 1.2L * CoordMax( D->S->posActuelles, D->S->nb_corps );
  if ( ! D->enExecution ) /* si la simulation est arrêtée, on force l'affichage */
    {
      gtk_widget_queue_draw( D->drawing_area );
    }
  return TRUE;
}


/**
 * Met fin à l'exécution du programme, ne fonctionne que si on a arrêté le
 * système.
 */
gboolean terminer( GtkWidget *window, Donnees * D )
{
  (void)( window ); /* évite un warning pour paramètre non utilisé */
  if ( ! D->enExecution )
    {
      detruireSysteme( D->S );
      gtk_main_quit();
    }
  else
    {
      fprintf( stderr, "Arrêtez d'abord la simulation\n" );
    }
  return TRUE;
}


/**
 * Fonction appelée lorsqu'on clique sur le bouton demandant d'incrémenter le
 * délai entre deux cycles de simulation.
 */
gboolean incDelai( GtkWidget *bouton, Donnees * D )
{
  (void)( bouton ); /* évite un warning pour paramètre non utilisé */
  if ( D->delai_entre_dessins < 10 )
    {
      D->delai_entre_dessins += 1;
    }
  else
    {
      D->delai_entre_dessins = ( D->delai_entre_dessins * 3 ) / 2;
    }
  intDansLabel( D->labelValeurDelai, D->delai_entre_dessins );
  return TRUE;
}


/**
 * Fonction appelée lorsqu'on clique sur le bouton demandant de décrémenter le
 * délai entre deux cycles de simulation.
 */
gboolean decDelai( GtkWidget *bouton, Donnees * D )
{
  (void)( bouton ); /* évite un warning pour paramètre non utilisé */
  if ( D->delai_entre_dessins == 1 )
    return FALSE;
  if ( D->delai_entre_dessins <= 10 )
    {
      D->delai_entre_dessins -= 1;
    }
  else
    {
      D->delai_entre_dessins = ( D->delai_entre_dessins * 2 ) / 3;
    }
  intDansLabel( D->labelValeurDelai, D->delai_entre_dessins );
  return TRUE;
}


/**
 * Fonction appelée lorsqu'on clique sur le bouton demandant d'incrémenter le
 * le nombre de pas de simulation effectués entre deux mise à jour du dessin.
 */
gboolean incIter( GtkWidget *bouton, Donnees * D )
{
  (void)( bouton ); /* évite un warning pour paramètre non utilisé */
  if ( D->nb_iter < 10 )
    {
      D->nb_iter += 1;
    }
  else
    {
      D->nb_iter = ( D->nb_iter * 3 ) / 2;
    }
  intDansLabel( D->labelValeurIter, D->nb_iter );
  return TRUE;
}


/**
 * Fonction appelée lorsqu'on clique sur le bouton demandant de décrémenter le
 * le nombre de pas de simulation effectués entre deux mise à jour du dessin.
 */
gboolean decIter( GtkWidget *bouton, Donnees * D )
{
  (void)( bouton ); /* évite un warning pour paramètre non utilisé */
  if ( D->nb_iter == 0 )
    return FALSE;
  if ( D->nb_iter <= 10 )
    {
      D->nb_iter -= 1;
    }
  else
    {
      D->nb_iter = ( D->nb_iter * 2 ) / 3;
    }
  intDansLabel( D->labelValeurIter, D->nb_iter );
  return TRUE;
}


/**
 * Fonction appelée lorsqu'on clique sur le bouton demandant d'incrémenter le
 * l'intervalle de temps d'un pas de simulation.
 */
gboolean incDT( GtkWidget *bouton, Donnees * D )
{
  (void)( bouton ); /* évite un warning pour paramètre non utilisé */
  D->delta_t *= 1.5L;
  flottantDansLabel( D->labelValeurDT, D->delta_t );
  return TRUE;
}


/**
 * Fonction appelée lorsqu'on clique sur le bouton demandant de décrémenter le
 * l'intervalle de temps d'un pas de simulation.
 */
gboolean decDT( GtkWidget *bouton, Donnees * D )
{
  (void)( bouton ); /* évite un warning pour paramètre non utilisé */
  D->delta_t /= 1.5L;
  flottantDansLabel( D->labelValeurDT, D->delta_t );
  return TRUE;
}


/**
 * Étant donné un tableau de points, détermine la coordonnées maximale en valeur
 * absolue.
 *
 * Parametres :
 *  - lesPoints - tableau de points
 *  - n - taille du tableau 'lesPoints'
 */
Flottant CoordMax( Point lesPoints[], int n )
{
  Flottant max = ZERO;
  int i;
  for ( i=0; i<n; ++i )
    {
      if ( lesPoints[i].x > max ) 
        max = lesPoints[i].x;
      else if ( -lesPoints[i].x > max )
        max = -lesPoints[i].x;
      if ( lesPoints[i].y > max )
        max = lesPoints[i].y;
      else if ( -lesPoints[i].y > max )
        max = -lesPoints[i].y;
    }
  return max;
}


/**
 * Étant donné un tableau de flottants, retourne la valeur max.
 *
 * Parametres :
 *  - tab - tableau de flottants
 *  - n - taille de 'tab'
 */
Flottant tabFlottantMax( Flottant tab[], int n )
{
  Flottant max = ZERO;
  int i;
  for ( i=0; i<n; ++i )
    {
      max = ( tab[i] > max ) ? tab[i] : max ;
    }
  return max;
}

/**
 * Construction de l'interface.
 */
void construireIHM( Donnees * D )
{ 
  /* Les widgets */
  GtkWidget *window;
  GtkWidget *hbox, *vbox, *vbox2;
  GtkWidget *boutonStart, *boutonStop, *boutonAfficher, 
            *boutonReset, *boutonRecadrer, *boutonQuitter;
  GtkWidget *hboxDelai, *vboxDelai, *boutonPlusDelai, *boutonMoinsDelai, *labelDelai;
  GtkWidget *hboxIter,  *vboxIter,  *boutonPlusIter,  *boutonMoinsIter,  *labelIter;
  GtkWidget *hboxDT,    *vboxDT,    *boutonPlusDT,    *boutonMoinsDT,    *labelDT;
  GtkWidget *hboxNbCorps, *labelNbCorps;
  GtkWidget *hboxPPS, *labelPPS;

  /* construction des boites */
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  D->drawing_area = gtk_drawing_area_new();
  hbox = gtk_hbox_new( FALSE, 10 );
  vbox = gtk_vbox_new( FALSE, 10 );
  vbox2 = gtk_vbox_new( FALSE, 10 );
  hboxDelai = gtk_hbox_new( TRUE, 10 );
  vboxDelai = gtk_vbox_new( FALSE, 10 );
  hboxIter = gtk_hbox_new( TRUE, 10 );
  vboxIter = gtk_vbox_new( FALSE, 10 );
  hboxDT = gtk_hbox_new( TRUE, 10 );
  vboxDT = gtk_vbox_new( FALSE, 10 );
  hboxNbCorps = gtk_hbox_new( TRUE, 10 );
  hboxPPS = gtk_hbox_new( TRUE, 10 );

  /* construction des label */
  boutonStart = gtk_button_new_with_label( "Lancer" );
  boutonStop = gtk_button_new_with_label( "Arrêter" );
  boutonAfficher = gtk_button_new_with_label( "Afficher" );
  boutonReset = gtk_button_new_with_label( "Réinitialiser" );
  boutonRecadrer = gtk_button_new_with_label( "Recadrer" );
  boutonQuitter = gtk_button_new_with_label( "Quitter" );

  /* L'utilisateur peut modifier trois informations : 
   *  - le délai à attendre après une mise à jour de l'image,
   *  - le nombre de pas de simulation entre deux images,
   *  - l'intervalle de temps d'un pas de simulation.
   *
   * Pour chacun, on met en place un label et deux boutons : '+', '-' pour
   * permettre de modifier la valeur.
   */
  labelDelai = gtk_label_new( "Délai entre images (ms)" );
  D->labelValeurDelai = gtk_label_new( "" );
  intDansLabel( D->labelValeurDelai, D->delai_entre_dessins );
  boutonPlusDelai = gtk_button_new_with_label( "+" );
  boutonMoinsDelai = gtk_button_new_with_label( "-" );

  labelIter = gtk_label_new( "Itérations par image" );
  D->labelValeurIter = gtk_label_new( "" );
  intDansLabel( D->labelValeurIter, D->nb_iter );
  boutonPlusIter = gtk_button_new_with_label( "+" );
  boutonMoinsIter = gtk_button_new_with_label( "-" );

  labelDT = gtk_label_new( "Intervalle de temps (s)" );
  D->labelValeurDT = gtk_label_new( "" );
  flottantDansLabel( D->labelValeurDT, D->delta_t );
  boutonPlusDT = gtk_button_new_with_label( "+" );
  boutonMoinsDT = gtk_button_new_with_label( "-" );

  /* Des labels informent l'utilisateur :
   *  - Temps écoulé depuis le début de la simulation,
   *  - Le nombre de pas par seconde,
   *  - Le nombre de corps célestes du système.
   */
  D->labelDuree = gtk_label_new( "temps écoulé : 0,00 s" );
  gtk_label_set_width_chars( GTK_LABEL( D->labelDuree) , 15 );

  labelPPS = gtk_label_new( "Pas par secondes : " );
  D->labelValeurPPS = gtk_label_new( "0" );
  gtk_label_set_width_chars( GTK_LABEL( D->labelValeurPPS ), 15 );

  labelNbCorps = gtk_label_new( "Nombre de corps : " );
  D->labelValeurNbCorps = gtk_label_new( "" );
  intDansLabel( D->labelValeurNbCorps, D->S->nb_corps );

  /* On met tous les éléments dans les boites */
  gtk_container_add ( GTK_CONTAINER (window), hbox );
  gtk_container_add ( GTK_CONTAINER (hbox), D->drawing_area );
  gtk_container_add ( GTK_CONTAINER (hbox), vbox );
  gtk_container_add ( GTK_CONTAINER (vbox), boutonStart );
  gtk_container_add ( GTK_CONTAINER (vbox), boutonStop );
  gtk_container_add ( GTK_CONTAINER (vbox), boutonAfficher );
  gtk_container_add ( GTK_CONTAINER (vbox), boutonReset );
  gtk_container_add ( GTK_CONTAINER (vbox), boutonRecadrer );
  gtk_container_add ( GTK_CONTAINER (vbox), boutonQuitter );
  gtk_container_add ( GTK_CONTAINER (vbox), vbox2 );
  gtk_container_add ( GTK_CONTAINER (vbox2), labelDelai );
  gtk_container_add ( GTK_CONTAINER (vbox2), hboxDelai );
  gtk_container_add ( GTK_CONTAINER (hboxDelai), vboxDelai );
  gtk_container_add ( GTK_CONTAINER (vboxDelai), boutonPlusDelai );
  gtk_container_add ( GTK_CONTAINER (vboxDelai), boutonMoinsDelai );
  gtk_container_add ( GTK_CONTAINER (hboxDelai), D->labelValeurDelai );
  gtk_container_add ( GTK_CONTAINER (vbox2), labelIter );
  gtk_container_add ( GTK_CONTAINER (vbox2), hboxIter );
  gtk_container_add ( GTK_CONTAINER (hboxIter), vboxIter );
  gtk_container_add ( GTK_CONTAINER (vboxIter), boutonPlusIter );
  gtk_container_add ( GTK_CONTAINER (vboxIter), boutonMoinsIter );
  gtk_container_add ( GTK_CONTAINER (hboxIter), D->labelValeurIter );
  gtk_container_add ( GTK_CONTAINER (vbox2), labelDT );
  gtk_container_add ( GTK_CONTAINER (vbox2), hboxDT );
  gtk_container_add ( GTK_CONTAINER (hboxDT), vboxDT );
  gtk_container_add ( GTK_CONTAINER (vboxDT), boutonPlusDT );
  gtk_container_add ( GTK_CONTAINER (vboxDT), boutonMoinsDT );
  gtk_container_add ( GTK_CONTAINER (hboxDT), D->labelValeurDT );
  gtk_container_add ( GTK_CONTAINER (vbox), D->labelDuree );
  gtk_container_add ( GTK_CONTAINER (vbox), hboxPPS );
  gtk_container_add ( GTK_CONTAINER (hboxPPS), labelPPS );
  gtk_container_add ( GTK_CONTAINER (hboxPPS), D->labelValeurPPS );
  gtk_container_add ( GTK_CONTAINER (vbox), hboxNbCorps );
  gtk_container_add ( GTK_CONTAINER (hboxNbCorps), labelNbCorps );
  gtk_container_add ( GTK_CONTAINER (hboxNbCorps), D->labelValeurNbCorps );

  /* On donne la taille voulue à la zone de dessins */
  gtk_widget_set_size_request( D->drawing_area, D->taille_dessin, D->taille_dessin );

  /* connexion des boutons aux fonction d'événement */
  g_signal_connect( G_OBJECT( D->drawing_area ), "expose_event",
		   G_CALLBACK( expose_evt_reaction ), (gpointer) D );
  g_signal_connect( boutonStart,      "clicked", G_CALLBACK( lancer ),   (gpointer) D );
  g_signal_connect( boutonStop,       "clicked", G_CALLBACK( stopper ),  (gpointer) D );
  g_signal_connect( boutonAfficher,   "clicked", G_CALLBACK( afficher ), (gpointer) D );
  g_signal_connect( boutonReset,      "clicked", G_CALLBACK( reInit ),   (gpointer) D );
  g_signal_connect( boutonRecadrer,   "clicked", G_CALLBACK( recadrer ), (gpointer) D );
  g_signal_connect( boutonQuitter,    "clicked", G_CALLBACK( terminer ), (gpointer) D );
  g_signal_connect( boutonPlusDelai,  "clicked", G_CALLBACK( incDelai ), (gpointer) D );
  g_signal_connect( boutonMoinsDelai, "clicked", G_CALLBACK( decDelai ), (gpointer) D );
  g_signal_connect( boutonPlusIter,   "clicked", G_CALLBACK( incIter ),  (gpointer) D );
  g_signal_connect( boutonMoinsIter,  "clicked", G_CALLBACK( decIter ),  (gpointer) D );
  g_signal_connect( boutonPlusDT,     "clicked", G_CALLBACK( incDT ),    (gpointer) D );
  g_signal_connect( boutonMoinsDT,    "clicked", G_CALLBACK( decDT ),    (gpointer) D );

  /* c'est fini, on affiche le tout ! */
  gtk_widget_show_all( window );

}




