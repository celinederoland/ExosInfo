#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <gtk/gtk.h>
#include "points.h"
#include "pile.h"

//-----------------------------------------------------------------------------
// Plan du code
//-----------------------------------------------------------------------------

/*
1 : déclaration des types 
	Contexte

2 : déclaration des fonctions

3 : programme principal

4 : IHM

5 : création de points dans un disque ou un losangeRandom

6 : enveloppe convexe de Graham

7 : enveloppe convexe de Jarvis
*/

//-----------------------------------------------------------------------------
// Déclaration des types
//-----------------------------------------------------------------------------
/**
	 Le contexte contient les informations utiles de l'interface pour
	 les algorithmes de géométrie algorithmique.
*/
typedef struct SContexte {
	int width;
	int height;
	GtkWidget* drawing_area;
	TabPoints P;
	GtkEntry* choixNbPoints;
	GtkWidget* labelNbPoints;
	GtkWidget* labelNbSommets;
	GtkWidget* labelTime;
	int nbPoints;
	PilePoints Pile;
	PilePoints PileJarvis;
	PilePoints PileBisJarvis;
	int nbSommets;
} Contexte;


//-----------------------------------------------------------------------------
// Déclaration des fonctions
//-----------------------------------------------------------------------------

// IHM
//-----------------------------------------------------------------------------

/* Crée l'interface graphique en fonction du contexte pCtxt.*/
GtkWidget* creerIHM( Contexte* pCtxt );

/* Cette réaction est appelée à la création de la zone de dessin. */
gboolean realize_evt_reaction( GtkWidget *widget, gpointer data );
/* Réaction principale qui est appelée pour redessiner la zone de dessin. */
gboolean expose_evt_reaction( GtkWidget *widget, GdkEventExpose *event, gpointer data );

/* Fait la conversion coordonnées réelles vers coordonnées de la zone de dessin. */
Point point2DrawingArea( Point p, Contexte* pCtxt );
/* Affichage des points et segments dans la zone de dessin */
void drawPoint( cairo_t* cr, Point p );
void drawLine( cairo_t* cr, Point p, Point q );

// Génère un certain nombre de points distribués aléatoirement dans le disque ou le losange unité.
//-----------------------------------------------------------------------------
int choixNbPoints(Contexte* pCtxt);
gboolean diskRandom( GtkWidget *widget, gpointer data);
gboolean losangeRandom( GtkWidget *widget, gpointer data );

// Enveloppe convexe par l'algorithme de Graham
//-----------------------------------------------------------------------------
gboolean ConvexHull(GtkWidget *widget, gpointer data);

// Enveloppe convexe par l'algorithme de Jarvis
//-----------------------------------------------------------------------------
gboolean ConvexJarvis(GtkWidget *widget, gpointer data);



//-----------------------------------------------------------------------------
// Programme principal
//-----------------------------------------------------------------------------
int main( int	 argc,
					char* argv[] )
{
	Contexte context;
	TabPoints_init( &(context.P) );
	PilePoints_init( &(context.Pile) );
	PilePoints_init( &(context.PileBisJarvis) );
	PilePoints_init( &(context.PileJarvis) );
	context.nbPoints = 0;

	/* Passe les arguments à GTK, pour qu'il extrait ceux qui le concernent. */
	gtk_init( &argc, &argv );
	
	/* Crée une fenêtre. */
	creerIHM( &context );

	/* Rentre dans la boucle d'événements. */
	gtk_main ();

	/* Libère la mémoire occupée */
	TabPoints_termine(&(context.P));
	return 0;
}

// force un événement "expose" juste derrière.
gboolean realize_evt_reaction( GtkWidget *widget, gpointer data )
{ 
	gtk_widget_queue_draw( widget ); 
	return TRUE;
}

// c'est la réaction principale qui va redessiner tout.
gboolean expose_evt_reaction( GtkWidget *widget, GdkEventExpose *event, gpointer data )
{
	int i;
	Contexte* pCtxt = (Contexte*) data;
	TabPoints* ptrP = &(pCtxt->P);
	PilePoints* ptrPile = &(pCtxt->Pile);
	PilePoints* ptrPileJarvis = &(pCtxt->PileJarvis);
	PilePoints* ptrPileBis = &(pCtxt->PileBisJarvis);
	cairo_t* cr = gdk_cairo_create (widget->window);
	cairo_set_source_rgb (cr, 1, 1, 1); // choisit le blanc.
	cairo_paint( cr ); // remplit tout dans la couleur choisie.
	
	// Affiche tous les points en bleu.
	if (TabPoints_nb( ptrP ) > 0) 
	{
		cairo_set_source_rgb (cr, 1, 0, 0);
		drawPoint( cr, point2DrawingArea( TabPoints_get( ptrP, 0 ), pCtxt ) );
	}
	// Affiche en rouge le point d'indice 0 (au début, c'est n'importe quel point, ensuite c'est celui le plus en bas à gauche)
	cairo_set_source_rgb (cr, 0, 0, 1);
	for ( i = 1; i < TabPoints_nb( ptrP ); ++i )
		drawPoint( cr, point2DrawingArea( TabPoints_get( ptrP, i ), pCtxt ) );

	// Affiche en vert l'enveloppe convexe de Graham
	cairo_set_source_rgb (cr, 0, 1, 0);
	if(!PilePoints_estVide(ptrPile)){
		
		int n = PilePoints_nb(ptrPile);
		for( i = 1; i < n; i++){
			drawLine(cr, point2DrawingArea(PilePoints_sommet(ptrPile), pCtxt), point2DrawingArea(PilePoints_deuxiemeSommet(ptrPile),pCtxt));
			
			PilePoints_depile(ptrPile); 
		} 
	}

	// Pour Jarvis il y a deux piles car on calcule l'enveloppe convexe en 2 parties (jusqu'au point le plus haut)
	// Affiche en jaune l'enveloppe convexe de Jarvis
	cairo_set_source_rgb (cr, 1, 1, 0);
	if(!PilePoints_estVide(ptrPileJarvis)){
		
		int n = PilePoints_nb(ptrPileJarvis);
		for( i = 1; i < n; i++){
			drawLine(cr, point2DrawingArea(PilePoints_sommet(ptrPileJarvis), pCtxt), point2DrawingArea(PilePoints_deuxiemeSommet(ptrPileJarvis),pCtxt));
			
			PilePoints_depile(ptrPileJarvis); 
		} 
	}
	if(!PilePoints_estVide(ptrPileBis)){
		
		int n = PilePoints_nb(ptrPileBis);
		for( i = 1; i < n; i++){
			drawLine(cr, point2DrawingArea(PilePoints_sommet(ptrPileBis), pCtxt), point2DrawingArea(PilePoints_deuxiemeSommet(ptrPileBis),pCtxt));
			
			PilePoints_depile(ptrPileBis); 
		}
	}

	// On libère la mémoire occupée par les piles
	PilePoints_termine( ptrPile );
	PilePoints_termine( ptrPileJarvis );
	PilePoints_termine( ptrPileBis );

	// On a fini, on peut détruire la structure.
	cairo_destroy (cr);
	return TRUE;
}

/* Fonctions de conversion et d'affichage des points et segments */
Point point2DrawingArea( Point p, Contexte* pCtxt )
{
	Point q;
	q.x = (p.x+1.0)/2.0*pCtxt->width;
	q.y = (1.0-p.y)/2.0*pCtxt->height;
	return q;
}
void drawPoint( cairo_t* cr, Point p )
{
	cairo_arc( cr, p.x, p.y, 1.5, 0., 2.0 * 3.14159626 );
	cairo_fill( cr );
}
void drawLine( cairo_t* cr, Point p, Point q )
{
	cairo_move_to( cr, p.x, p.y );
	cairo_line_to( cr, q.x, q.y );
	cairo_stroke( cr );
}

/* Charge l'image donnée et crée l'interface. */
GtkWidget* creerIHM( Contexte* pCtxt )
{
	
	GtkWidget* window;
	GtkWidget* vbox1;
	GtkWidget* vbox2;
	GtkWidget* hbox1;
	GtkWidget* button_quit;
	GtkWidget* button_disk_random;
	GtkWidget* button_losange_random;
	GtkWidget* label_nbPoints;
	GtkWidget* entry_nbPoints;
	GtkWidget* button_convex;
	GtkWidget* button_convexJarvis;
	GtkWidget* label_nbSommets;
	GtkWidget* label_time;

	// Crée une fenêtre. 
	window = gtk_window_new( GTK_WINDOW_TOPLEVEL );
	// Crée un conteneur horizontal box.
	hbox1 = gtk_hbox_new( FALSE, 10 );
	// Crée deux conteneurs vertical box.
	vbox1 = gtk_vbox_new( FALSE, 10 );
	vbox2 = gtk_vbox_new( FALSE, 10 );
	// Crée une zone de dessin
	pCtxt->drawing_area = gtk_drawing_area_new();
	pCtxt->width	= 500;
	pCtxt->height = 500;
	gtk_widget_set_size_request ( pCtxt->drawing_area, pCtxt->width, pCtxt->height );
	// Crée les boutons d'actions.
	button_disk_random	= gtk_button_new_with_label( "Points aléatoires dans disque" );
	button_losange_random	= gtk_button_new_with_label( "Points aléatoires dans losange" );
	label_nbPoints = gtk_label_new("0 points");
	label_nbSommets = gtk_label_new("0 sommets");
	label_time = gtk_label_new("0 ms");
	entry_nbPoints = gtk_entry_new();
	gtk_entry_set_text (GTK_ENTRY (entry_nbPoints), "Entrez le nombre de points" );
	button_convex	= gtk_button_new_with_label( "Enveloppe convexe (Graham)" );
	button_convexJarvis = gtk_button_new_with_label( "Enveloppe convexe (Jarvis)" );
	button_quit = gtk_button_new_with_label( "Quitter" );


	// Agencement des composants
	gtk_container_add( GTK_CONTAINER( hbox1 ), pCtxt->drawing_area );
	gtk_container_add( GTK_CONTAINER( hbox1 ), vbox2 );
	gtk_container_add( GTK_CONTAINER( vbox1 ), hbox1 );
	gtk_container_add( GTK_CONTAINER( vbox1 ), button_quit );
	gtk_container_add( GTK_CONTAINER( window ), vbox1 );
	gtk_container_add( GTK_CONTAINER( vbox2 ), button_disk_random );
	gtk_container_add( GTK_CONTAINER( vbox2 ), button_losange_random );
	gtk_container_add( GTK_CONTAINER( vbox2 ), label_nbPoints );
	gtk_container_add( GTK_CONTAINER( vbox2 ), label_nbSommets );
	gtk_container_add( GTK_CONTAINER( vbox2 ), label_time );
	gtk_container_add( GTK_CONTAINER( vbox2 ), entry_nbPoints );
	gtk_container_add( GTK_CONTAINER( vbox2 ), button_convex );
	gtk_container_add( GTK_CONTAINER( vbox2 ), button_convexJarvis );

	// Connection des réactions aux composants
	// Mise à jour du drawing_area
	g_signal_connect( G_OBJECT( pCtxt->drawing_area ), "realize", 
		G_CALLBACK( realize_evt_reaction ), pCtxt );
	g_signal_connect( G_OBJECT( pCtxt->drawing_area ), "expose_event",
		G_CALLBACK( expose_evt_reaction ), pCtxt );
	// Connecte la réaction gtk_main_quit à l'événement "clic" sur ce bouton.
	g_signal_connect( button_quit, "clicked",
		G_CALLBACK( gtk_main_quit ),
		NULL);
	// Création de points dans un disque ou un losange
	g_signal_connect( button_disk_random, "clicked",
		G_CALLBACK( diskRandom ),
		pCtxt );
	g_signal_connect( button_losange_random, "clicked",
		G_CALLBACK( losangeRandom ),
		pCtxt );
	// Enveloppe convexe Graham
	g_signal_connect( button_convex, "clicked",
		G_CALLBACK( ConvexHull ),
		pCtxt );
	// Enveloppe convexe Jarvis
	g_signal_connect( button_convexJarvis, "clicked",
		G_CALLBACK( ConvexJarvis ),
		pCtxt );

	// Enregistrement des composants nécessaires dans le contexte
	pCtxt -> labelNbPoints = label_nbPoints;
	pCtxt -> choixNbPoints = (GtkEntry*)entry_nbPoints;
	pCtxt -> labelNbSommets = label_nbSommets;
	pCtxt -> labelTime = label_time;

	// Rend tout visible
	gtk_widget_show_all( window );
	
	return window;
}

//-----------------------------------------------------------------------------
// Création des points dans un disque ou un losange
//-----------------------------------------------------------------------------

/* Lit le nombre de points demandés par l'utilisateurs et les ajoute au compteur du contexte */
int choixNbPoints(Contexte* pCtxt)
{
	
	int nbPointsAJout = atoi(gtk_entry_get_text(pCtxt -> choixNbPoints));
	char textlabel[150];
	pCtxt -> nbPoints += nbPointsAJout;
	sprintf(textlabel,"%d points",pCtxt -> nbPoints);
	gtk_label_set_text((GtkLabel*)(pCtxt -> labelNbPoints),textlabel);
	
	return nbPointsAJout;
}

gboolean diskRandom( GtkWidget *widget, gpointer data )
{
	
	Contexte* pCtxt = (Contexte*) data;
	TabPoints* ptrP = &(pCtxt->P);
	int nbPointsAjout = choixNbPoints(pCtxt);
	for ( int i = 0; i < nbPointsAjout; ++i )
		{
			Point p;
			//Crée des points dans le carré de diagonale (-1;-1)(1;1) et ne garde que ceux contenus dans le cercle de centre O et de rayon 1
			do {
				p.x = 2.0 * ( rand() / (double) RAND_MAX ) - 1.0;
				p.y = 2.0 * ( rand() / (double) RAND_MAX ) - 1.0;
			} while ( (p.x*p.x+p.y*p.y) > 1.0 );
			TabPoints_ajoute( ptrP, p );
		}
	gtk_widget_queue_draw( pCtxt->drawing_area );
	
	return TRUE;
}

gboolean losangeRandom( GtkWidget *widget, gpointer data )
{
	
	Contexte* pCtxt = (Contexte*) data;
	TabPoints* ptrP = &(pCtxt->P);
	int nbPointsAjout = choixNbPoints(pCtxt);
	Point pInt;
	double R2Sur4 = sqrt(2)/4; //Pour éviter d'appeler la fonction sqrt(2) nbPointsAJout*4 fois
	for ( int i = 0; i < nbPointsAjout; ++i )
		{
			Point p;
			//Crée un point dans le carré de diagonale (-1;-1)(1;1)
			pInt.x = 2.0 * ( rand() / (double) RAND_MAX ) - 1.0;
			pInt.y = 2.0 * ( rand() / (double) RAND_MAX ) - 1.0;
			//Applique la transformation pour le placer dans le losange
			p.x = R2Sur4 * pInt.x + R2Sur4 * pInt.y;
			p.y = -R2Sur4 * pInt.x + R2Sur4 * pInt.y;
			TabPoints_ajoute( ptrP, p );
		}
	gtk_widget_queue_draw( pCtxt->drawing_area );
	
	return TRUE;
}


//-----------------------------------------------------------------------------
// Enveloppe convexe par Parcours de Graham.
//-----------------------------------------------------------------------------

gboolean ConvexHull(GtkWidget *widget, gpointer data)
{
	
	int i;
	Contexte* pCtxt = (Contexte*) data;
	TabPoints* ptrP = &(pCtxt->P);
	PilePoints* ptrPile = &(pCtxt->Pile);
	PilePoints_init( ptrPile );

	struct timespec myTimerStart;
	clock_gettime(CLOCK_REALTIME, &myTimerStart); // Démarre l'horloge

	//On récupère le point le plus en bas et à gauche à l'indice 0 du tableau de points
	i = TabPoints_indexBasGauche(ptrP);
	Echange( 0, i, ptrP ); // ptrP->points[0] est le pivot

	// On trie les points 1 à n-1 suivant l'angle T[0]T[i].
	TabPoints_triSelonT0(ptrP);

	// On empile le premier segment
	PilePoints_empile(ptrPile, ptrP->points[0]);
	PilePoints_empile(ptrPile, ptrP->points[1]);

	// On empile les points qui sont à gauche du dernier segment empilé, et on dépile le dernier point empilé sinon. De façon à toujours obtenir une liste de points qui ne nous fait jamais "revenir plus à droite" car cela entrainerait la non convexité.
	for(i = 2; i < TabPoints_nb(ptrP); i++){
		while( !EstAGauche(PilePoints_deuxiemeSommet(ptrPile), PilePoints_sommet(ptrPile), TabPoints_get(ptrP,i)))
		{
			PilePoints_depile(ptrPile);
		}
		PilePoints_empile(ptrPile, TabPoints_get(ptrP,i));
	}
	
	pCtxt -> nbSommets = PilePoints_nb(ptrPile);
	char textlabel[150];
	sprintf(textlabel,"%d sommets",pCtxt -> nbSommets);
	gtk_label_set_text((GtkLabel*)(pCtxt -> labelNbSommets),textlabel);

	struct timespec current; // Stoppe l'horloge
	clock_gettime(CLOCK_REALTIME, &current); //Linux gettime
	double t = (( current.tv_sec - myTimerStart.tv_sec) *1000 + ( current.tv_nsec - myTimerStart.tv_nsec)/1000000.0);
	//printf( "(%d;%lf)\n", pCtxt -> nbPoints, t );
	printf( "(%d;%d)\n", pCtxt -> nbPoints, pCtxt -> nbSommets );

	sprintf(textlabel,"%lf ms",t);
	gtk_label_set_text((GtkLabel*)(pCtxt -> labelTime),textlabel);

	// P contient la liste des sommets de l'enveloppe convexe dans l'ordre inverse.
	gtk_widget_queue_draw( pCtxt->drawing_area );
	
	return TRUE;
}


//-----------------------------------------------------------------------------
// Enveloppe convexe par Parcours de Jarvis.
//-----------------------------------------------------------------------------

gboolean ConvexJarvis(GtkWidget *widget, gpointer data)
{
	int i, indPointInitial, indPointFinal;
	int indAnglePoFaible, indAnglePoEleve;

	Contexte* pCtxt = (Contexte*) data;

	TabPoints* ptrP = &(pCtxt->P);

	PilePoints* ptrPile = &(pCtxt->PileJarvis);
	PilePoints* ptrPileBis = &(pCtxt->PileBisJarvis);
	PilePoints_init( ptrPile );
	PilePoints_init( ptrPileBis );

	struct timespec myTimerStart;
	clock_gettime(CLOCK_REALTIME, &myTimerStart); // Démarre l'horloge

	i = TabPoints_indexBasGauche(ptrP);
	Echange(0, i, ptrP); // ptrP->points[0] est le point initial
	i = 0;
	indPointInitial = i;
	// On empile le point le plus bas
	PilePoints_empile(ptrPile, ptrP->points[indPointInitial]);
	indPointFinal = TabPoints_indexHaut(ptrP);

	// On parcours les points restants jusqu'au point le plus haut
	while(indPointInitial != indPointFinal){
		indAnglePoFaible = ChercheAngleFaible(ptrP, indPointInitial, indPointFinal);
		PilePoints_empile(ptrPile, TabPoints_get(ptrP, indAnglePoFaible));
		// Element suivant.
		// Le point initial devient le point qui a l'angle polaire le plus faible
		indPointInitial = indAnglePoFaible;
	}
	// Traitement de l element final. 
	// On empile le point le plus haut
	PilePoints_empile(ptrPile, TabPoints_get(ptrP, indPointFinal));
	// On recommence dans le sens inverse
	indPointInitial = i;
	while(indPointInitial != indPointFinal){
		indAnglePoEleve = ChercheAngleEleve(ptrP, indPointInitial, indPointFinal);
		PilePoints_empile(ptrPileBis, TabPoints_get(ptrP, indAnglePoEleve));
		// Element suivant.
		// Le point initial devient le point qui a l'angle polaire le plus eleve
		indPointInitial = indAnglePoEleve;
	}

	pCtxt -> nbSommets = PilePoints_nb(ptrPile) + PilePoints_nb(ptrPileBis);
	char textlabel[150];
	sprintf(textlabel,"%d sommets",pCtxt -> nbSommets);
	gtk_label_set_text((GtkLabel*)(pCtxt -> labelNbSommets),textlabel);

	struct timespec current; // Stoppe l'horloge
	clock_gettime(CLOCK_REALTIME, &current); //Linux gettime
	double t = (( current.tv_sec - myTimerStart.tv_sec) *1000 + ( current.tv_nsec - myTimerStart.tv_nsec)/1000000.0);
	printf( "(%d;%lf)\n", pCtxt -> nbPoints, t );

	sprintf(textlabel,"%lf ms",t);
	gtk_label_set_text((GtkLabel*)(pCtxt -> labelTime),textlabel);

	gtk_widget_queue_draw( pCtxt->drawing_area );
	return TRUE;
}
