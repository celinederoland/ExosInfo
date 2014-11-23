//-----------------------------------------------------------------------------
// INFO626 TP1 Juliana Leclaire, Céline de Roland
//----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Entete
//-----------------------------------------------------------------------------

#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <gtk/gtk.h>
#include <time.h>

//-----------------------------------------------------------------------------
// Declaration des types
//-----------------------------------------------------------------------------

/**
 Le contexte contient les informations utiles de l'interface pour
 les algorithmes de traitement d'image.	
*/
typedef struct SContexte {
// Structure initiale
int width;
int height;
GdkPixbuf* pixbuf_input;
GdkPixbuf* pixbuf_output;
GtkWidget* image;

// Nous avons ajoute :
GtkWidget* seuil;
GtkWidget* alpha;

} Contexte;

/**
 Un pixel est une structure de 3 octets (rouge, vert, bleu). On les
 plaque au bon endroit dans un pixbuf pour modifier les couleurs du pixel.
*/
typedef struct {
guchar rouge;
guchar vert;
guchar bleu;
} Pixel;

/**
 Un Objet stocke un pointeur vers son pixel, son rang et un pointeur vers l'objet pere.
*/
typedef struct SObjet {
Pixel* pixel; // adresse du pixel dans le pixbuf
int rang;
struct SObjet* pere;
} Objet;

/**
 StatCouleur stocke les sommes cumulees de rouge, vert, bleu et le nombre total de pixels ajoutes
*/
typedef struct {
double rouge;
double vert;
double bleu;
int nb;
} StatCouleur;

/**
 TSVCouleur stocke les valeurs de teinte, saturation et valeur d'un pixel
*/
typedef struct {
int t; // teinte comme angle en degres
double s; // saturation comme nombre entre 0 et 1
double v; // valeur ou brillance comme nombre entre 0 et 1
} TSVCouleur;

//-----------------------------------------------------------------------------
// Declaration des fonctions et plan du code
//-----------------------------------------------------------------------------

// Entete (l.6)

// Declaration des types (l. 16)
/*
	Contexte
	Pixel
	Objet
	StatCouleur
	TSVCouleur
*/

// Declaration des fonctions (l. 76)

// Programme principal (l. 167)
int main( int	 argc, char* argv[] );

// question 2 : Seuillage de l'image (ligne 188)
	void seuiller( GtkWidget *widget, gpointer data );

// question 3 : Decoupage de l'image seuillee en composantes connexes (ligne 222)
	// question 3.1 : Structure Union-Find par foret (l. 225)
		Objet* CreerEnsembles( GdkPixbuf* pixbuf );
		gboolean estRepresentant(Objet* obj);
		//Nous avons laissé ces fonctions telles que nous les avons faites à la question 3.1, mais nous les avons commentées entièrement pour utiliser plutôt les fonctions de la question 3.3 par la suite
		//Objet* TrouverEnsemble( Objet* obj );
		//void Union( Objet* obj1, Objet* obj2 );
		//void Lier( Objet* obj1, Objet* obj2 );

	// question 3.2 : Utilisation de cette structure pour le decoupage en composantes connexes. (l. 294)
		void composantesConnexes( GtkWidget *widget, gpointer data );
		void setRandomColor( Pixel* data );
		void copierPixel( Pixel* src, Pixel* dest );

	// question 3.3 : Temps d'execution de Union-Find (l. 355)
		Objet* TrouverEnsembleBete( Objet* obj );
		void UnionBete( Objet* obj1, Objet* obj2 );
		void UnionParRang( Objet* obj1, Objet* obj2 );
		//Nous avons essayé 3 façons différentes de faire la compression de chemin :
			//En utilisant la récursivité
			Objet* TrouverEnsembleCompressionCheminsRec( Objet* obj );
			//En remontant tous les noeuds trouvés sur le chemin sans récursivité
			Objet* TrouverEnsembleCompressionCheminsWhile( Objet* obj );
			//En remontant à la racine seulement le noeud par lequel on est entré dans la fonction à la racine
			Objet* TrouverEnsembleCompressionCheminsUnNoeud( Objet* obj );
		void UnionCompressionCheminsRec( Objet* obj1, Objet* obj2 );
		void UnionCompressionCheminsWhile( Objet* obj1, Objet* obj2 );
		void UnionCompressionCheminsUnNoeud( Objet* obj1, Objet* obj2 );
		void UnionRangEtCompRec( Objet* obj1, Objet* obj2 );
		void UnionRangEtCompWhile( Objet* obj1, Objet* obj2 );
		void UnionRangEtCompUnNoeud( Objet* obj1, Objet* obj2 );
		//Pour faciliter les tests (éviter de commenter et décommenter des lignes à chaque fois), nous proposons donc les 8 méthodes avec 8 boutons
		void composantesConnexesCalculBete( GtkWidget *widget, gpointer data );
		void composantesConnexesCalculRang( GtkWidget *widget, gpointer data );
		void composantesConnexesCalculCompRec( GtkWidget *widget, gpointer data );
		void composantesConnexesCalculCompWhile( GtkWidget *widget, gpointer data );
		void composantesConnexesCalculCompUnNoeud( GtkWidget *widget, gpointer data );
		void composantesConnexesCalculRangEtCompRec( GtkWidget *widget, gpointer data );
		void composantesConnexesCalculRangEtCompWhile( GtkWidget *widget, gpointer data );
		void composantesConnexesCalculRangEtCompUnNoeud( GtkWidget *widget, gpointer data );

// question 4 : Affichage de la couleur moyenne pour chaque region (ligne 1091)
	void composantesConnexesCoulMoy( GtkWidget *widget, gpointer data );
	void setColorFromStat(Pixel* pix, StatCouleur stat);
	void initialiseStatCouleur(StatCouleur* stats, int taille);

// question 5 : Connexite floue et decoupage en composantes connexes floues (ligne 1181)
	double minimum(double x, double y, double z);
	double maximum(double x, double y, double z);
	TSVCouleur tsv( Pixel* pixel );
	double similitude( Pixel* p1, Pixel* p2 );
	//doube similitudeBis( Pixel* p1, Pixel* p2 ); //Autre fonction de similitude possible

// Charge l'image donnee et cree l'interface. (ligne 1326)
// Nous avons placé les boutons en haut de l'image plutôt qu'en dessous. Cela nous a permis de réaliser nos tests sur des images dépassant la taille de notre écran. Nous avons de plus ajouter des boutons également pour faciliter les tests.
	GtkWidget* creerIHM( const char* image_filename, Contexte* pCtxt );

// AnalysePixbuf modifié pour faciliter les tests (ligne 1551)
	void analyzePixbuf( GdkPixbuf* pixbuf );

// Fonctions de l'archive non modifiees (ligne 1586)
	gboolean selectInput( GtkWidget *widget, gpointer data );
	gboolean selectOutput( GtkWidget *widget, gpointer data );
	GdkPixbuf* creerImage( int width, int height );
	unsigned char greyLevel( Pixel* data );
	void setGreyLevel( Pixel* data, unsigned char g );
	Pixel* gotoPixel( GdkPixbuf* pixbuf, int x, int y );
	void disk( GdkPixbuf* pixbuf, int r );

//-----------------------------------------------------------------------------
// Programme principal
//-----------------------------------------------------------------------------
int main( int	 argc,
					char* argv[] )
{
	Contexte context;
	const char* image_filename = argc > 1 ? argv[ 1 ] : "papillon-express.jpg";

	/* Passe les arguments a GTK, pour qu'il extrait ceux qui le concernent. */
	gtk_init( &argc, &argv );
	
	/* Cree une fenetre. */
	creerIHM( image_filename, &context );

	/* Rentre dans la boucle d'evenements. */
	gtk_main ();

	return 0;
}

//-----------------------------------------------------------------------------
// question 2 : Seuillage de l'image
//-----------------------------------------------------------------------------
void seuiller( GtkWidget *widget, gpointer data )
{
	Contexte* pCtxt = (Contexte*) data;
	int largeur = gdk_pixbuf_get_width(pCtxt -> pixbuf_input); 
	int hauteur = gdk_pixbuf_get_height(pCtxt -> pixbuf_input);
	gdouble seuil = gtk_range_get_value( (GtkRange*)(pCtxt -> seuil) );
	int x, y;
	Pixel* pix_input; Pixel* pix_output;

	/* On parcourt l'image source et pour chaque pixel :
		 si le niveau de gris du pixel est inferieur au seuil on met un pixel noir dans l'image de sortie
		 sinon on met un pixel blanc
	*/
	for (x = 0; x < largeur; x++)
	{
		for (y = 0; y < hauteur; y++)
		{
			pix_input = gotoPixel(pCtxt -> pixbuf_input,x,y);
			pix_output = gotoPixel(pCtxt -> pixbuf_output,x,y);
			if (greyLevel(pix_input) < seuil)	{ setGreyLevel(pix_output,0); }
			else { setGreyLevel(pix_output,255); }
		}
	}

	/* On affiche le resultat */
	// Place le pixbuf a visualiser dans le bon widget.
	gtk_image_set_from_pixbuf( GTK_IMAGE( pCtxt->image ), pCtxt->pixbuf_output );
	// Force le reaffichage du widget.
	gtk_widget_queue_draw( pCtxt->image );
}

//-----------------------------------------------------------------------------
// question 3 : Decoupage de l'image seuillee en composantes connexes 
//-----------------------------------------------------------------------------

	// question 3.1 : Structure Union-Find par foret
	//-----------------------------------------------------------------------------
	Objet* CreerEnsembles( GdkPixbuf* pixbuf )
	{
		int i;
		int largeur = gdk_pixbuf_get_width(pixbuf); 
		int hauteur = gdk_pixbuf_get_height(pixbuf);
		int taille = largeur * hauteur;

		Objet* objets = malloc( sizeof(Objet) * taille );
		for (i = 0; i < taille; i++)
		{
			objets[i].pixel = gotoPixel(pixbuf, i % largeur, i / largeur);
			objets[i].rang = 0;
			objets[i].pere = &objets[i];
		}

		return objets;
	}

	gboolean estRepresentant(Objet* obj)
	{
		if (obj -> pere == obj) { return TRUE; }
		return FALSE;
	}

	//Nous avons laissé ces fonctions telles que nous les avons faites à la question 3.1, mais nous les avons commentées entièrement pour utiliser plutôt les fonctions de la question 3.3 par la suite

	/*Objet* TrouverEnsemble( Objet* obj )
	{
		if(obj == obj->pere){
			return obj;
		}
		else{
			obj->pere = TrouverEnsemble(obj->pere);
			return obj->pere;
		}
	}*/

	/*void Union( Objet* obj1, Objet* obj2 )
	{
		Objet* representant1 = TrouverEnsemble(obj1);
		Objet* representant2 = TrouverEnsemble(obj2);
		if (representant1 != representant2) {
			if(representant1->rang > representant2->rang){
				Lier(representant2, representant1);
			}
			else if(representant1->rang < representant2->rang){
				Lier(representant1, representant2);
			}
			else{
				representant1->pere = representant2;
				++(representant2->rang);
			}
		}
	}

	void Lier( Objet* obj1, Objet* obj2 )
	{
		if (obj1 -> pere == obj1 && obj2 -> pere == obj2)
		{
			obj1 -> pere = obj2;
		}
		else
		{
			Union( obj1, obj2 );
		}
	}*/

	// question 3.2 : Utilisation de cette structure pour le decoupage en composantes connexes.
	//-----------------------------------------------------------------------------
	void composantesConnexes( GtkWidget *widget, gpointer data )
	{
		Contexte* pCtxt = (Contexte*) data;
		int largeur = gdk_pixbuf_get_width(pCtxt -> pixbuf_input); 
		int hauteur = gdk_pixbuf_get_height(pCtxt -> pixbuf_input);
		int i;
		Objet* objets = CreerEnsembles(pCtxt -> pixbuf_output);

		//On parcourt le tableau d'objet pour realiser l'union des pixels adjacents identiques
		for (i = 0; i < largeur*hauteur; i++)
		{
			//Pixel a sa droite
			if ( (i + 1) % largeur != 0 && greyLevel(objets[i].pixel) == greyLevel(objets[i+1].pixel) )
			{
				UnionRangEtCompUnNoeud(&objets[i],&objets[i+1]);
			}
			//Pixel en dessous
			if ( i + largeur < largeur*hauteur && greyLevel(objets[i].pixel) == greyLevel(objets[i+largeur].pixel) )
			{
				UnionRangEtCompUnNoeud(&objets[i],&objets[i+largeur]);
			} 
		}

		//On attribue une couleur aleatoire aux representants
		for (i = 0; i < largeur*hauteur; i++)
		{
			if (estRepresentant(&objets[i])) { setRandomColor(objets[i].pixel);}
		}

		//On recopie sur chaque pixel la couleur de son representant
		for (i = 0; i < largeur*hauteur; i++)
		{
			if (!estRepresentant(&objets[i])) { copierPixel( TrouverEnsembleCompressionCheminsUnNoeud(&objets[i]) -> pixel, objets[i].pixel );}
		}

		// On affiche le resultat 
		// Place le pixbuf a visualiser dans le bon widget.
		gtk_image_set_from_pixbuf( GTK_IMAGE( pCtxt->image ), pCtxt->pixbuf_output );
		// Force le reaffichage du widget.
		gtk_widget_queue_draw( pCtxt->image );

	}

	// Attribue une couleur aleatoire a un pixel
	void setRandomColor( Pixel* data )
	{
		data->rouge = rand()%255;
		data->vert	= rand()%255;
		data->bleu	= rand()%255;
	}

	// Recopie un pixel
	void copierPixel( Pixel* src, Pixel* dest )
	{
		dest -> rouge = src -> rouge;
		dest -> vert = src -> vert;
		dest -> bleu = src -> bleu;
	}

	// question 3.3 : Temps d'execution de Union-Find
	//-----------------------------------------------------------------------------

	//Bete

	Objet* TrouverEnsembleBete( Objet* obj )
	{
		for (; obj -> pere != obj; obj = obj -> pere) {}
		return obj;
	}

	void UnionBete( Objet* obj1, Objet* obj2 )
	{
		Objet* representant1 = TrouverEnsembleBete(obj1);
		Objet* representant2 = TrouverEnsembleBete(obj2);

		if (representant1 != representant2) {
			representant1->pere = representant2;
			++representant2->rang;
		}
	}

	//Par rang

	void UnionParRang( Objet* obj1, Objet* obj2 )
	{
		Objet* representant1 = TrouverEnsembleBete(obj1);
		Objet* representant2 = TrouverEnsembleBete(obj2);

		if (representant1 != representant2) {
			if(representant1->rang > representant2->rang){
				representant2->pere = representant1;
			}
			else if(representant1->rang < representant2->rang){
				representant1->pere = representant2;
			}
			else{
				representant1->pere = representant2;
				++(representant2->rang);
			}
		}
	}

	//Nous avons essayé 3 façons différentes de faire la compression de chemin :

		//En utilisant la récursivité
		Objet* TrouverEnsembleCompressionCheminsRec( Objet* obj )
		{
			if(obj == obj->pere){
				return obj;
			}
			else{
				obj->pere = TrouverEnsembleCompressionCheminsRec(obj->pere);
				return obj->pere;
			}
		}

		//En remontant tous les noeuds trouvés sur le chemin sans récursivité
		Objet* TrouverEnsembleCompressionCheminsWhile( Objet* obj )
		{
			Objet* y = obj;
			Objet* z = obj;
			Objet* t;
			while (y -> pere != y)
			{
				y = y -> pere;
			}
			//On compresse tous les noeuds par lesquels on est passé
			while (z -> pere != z)
			{
				t = z -> pere;
				z -> pere = y;
				z = t;
			}
			return y;
		}

		//On peut aussi ne compresser que le noeud par lequel on est entré (ça fait remonter ce noeud ainsi que tous ses fils de plusieurs rangs). D'après nos tests c'est plus rapide comme ça d'environ 10%
		Objet* TrouverEnsembleCompressionCheminsUnNoeud( Objet* obj )
		{
			Objet* y = obj;

			while (y -> pere != y)
			{
				y = y -> pere;
			}
			
			obj -> pere = y;

			return y;
		}

		void UnionCompressionCheminsRec( Objet* obj1, Objet* obj2 )
		{
			Objet* representant1 = TrouverEnsembleCompressionCheminsRec(obj1);
			Objet* representant2 = TrouverEnsembleCompressionCheminsRec(obj2);
			if (representant1 != representant2) {
				representant1->pere = representant2;
				++representant2->rang;
			}
		}

		void UnionCompressionCheminsWhile( Objet* obj1, Objet* obj2 )
		{
			Objet* representant1 = TrouverEnsembleCompressionCheminsWhile(obj1);
			Objet* representant2 = TrouverEnsembleCompressionCheminsWhile(obj2);
			if (representant1 != representant2) {
				representant1->pere = representant2;
				++representant2->rang;
			}
		}

		void UnionCompressionCheminsUnNoeud( Objet* obj1, Objet* obj2 )
		{
			Objet* representant1 = TrouverEnsembleCompressionCheminsUnNoeud(obj1);
			Objet* representant2 = TrouverEnsembleCompressionCheminsUnNoeud(obj2);
			if (representant1 != representant2) {
				representant1->pere = representant2;
				++representant2->rang;
			}
		}


		//Avec le rang et la compression de chemin

		void UnionRangEtCompRec( Objet* obj1, Objet* obj2 )
		{
			Objet* representant1 = TrouverEnsembleCompressionCheminsRec(obj1);
			Objet* representant2 = TrouverEnsembleCompressionCheminsRec(obj2);
			if (representant1 != representant2) {
				if(representant1->rang > representant2->rang){
					representant2->pere = representant1;
				}
				else if(representant1->rang < representant2->rang){
					representant1->pere = representant2;
				}
				else{
					representant1->pere = representant2;
					++(representant2->rang);
				}
			}
		}

		void UnionRangEtCompWhile( Objet* obj1, Objet* obj2 )
		{
			Objet* representant1 = TrouverEnsembleCompressionCheminsWhile(obj1);
			Objet* representant2 = TrouverEnsembleCompressionCheminsWhile(obj2);
			if (representant1 != representant2) {
				if(representant1->rang > representant2->rang){
					representant2->pere = representant1;
				}
				else if(representant1->rang < representant2->rang){
					representant1->pere = representant2;
				}
				else{
					representant1->pere = representant2;
					++(representant2->rang);
				}
			}
		}

		void UnionRangEtCompUnNoeud( Objet* obj1, Objet* obj2 )
		{
			Objet* representant1 = TrouverEnsembleCompressionCheminsUnNoeud(obj1);
			Objet* representant2 = TrouverEnsembleCompressionCheminsUnNoeud(obj2);
			if (representant1 != representant2) {
				if(representant1->rang > representant2->rang){
					representant2->pere = representant1;
				}
				else if(representant1->rang < representant2->rang){
					representant1->pere = representant2;
				}
				else{
					representant1->pere = representant2;
					++(representant2->rang);
				}
			}
		}


		//Pour faciliter les tests (éviter de commenter et décommenter des lignes à chaque fois), nous proposons donc les 6 méthodes avec 6 boutons
		void composantesConnexesCalculBete( GtkWidget *widget, gpointer data )
		{

			Contexte* pCtxt = (Contexte*) data;
			int largeur = gdk_pixbuf_get_width(pCtxt -> pixbuf_input); 
			int hauteur = gdk_pixbuf_get_height(pCtxt -> pixbuf_input);
			int i;
			Objet* objets = CreerEnsembles(pCtxt -> pixbuf_output);
			Pixel* pix_input; Pixel* pix_output;
			int x,y;

			// On seuille a 110 pour éviter d'avoir a le refaire a la main pour chaque test
			for (x = 0; x < largeur; x++)
			{
				for (y = 0; y < hauteur; y++)
				{
					pix_input = gotoPixel(pCtxt -> pixbuf_input,x,y);
					pix_output = gotoPixel(pCtxt -> pixbuf_output,x,y);
					if (greyLevel(pix_input) < 110)	{ setGreyLevel(pix_output,0); }
					else { setGreyLevel(pix_output,255); }
				}
			}

			//On démarre l'horloge
			struct timespec myTimerStart;
			clock_gettime(CLOCK_REALTIME, &myTimerStart);

			//On parcourt le tableau d'objet pour realiser l'union des pixels adjacents identiques
			for (i = 0; i < largeur*hauteur; i++)
			{
				//Pixel a sa droite
				if ( (i + 1) % largeur != 0 && greyLevel(objets[i].pixel) == greyLevel(objets[i+1].pixel) )
				{
					UnionBete(&objets[i],&objets[i+1]);
				}
				//Pixel en dessous
				if ( i + largeur < largeur*hauteur && greyLevel(objets[i].pixel) == greyLevel(objets[i+largeur].pixel) )
				{
					UnionBete(&objets[i],&objets[i+largeur]);
				} 
			}

			//On attribue une couleur aleatoire aux representants
			for (i = 0; i < largeur*hauteur; i++)
			{
				if (estRepresentant(&objets[i])) { setRandomColor(objets[i].pixel);}
			}

			//On recopie sur chaque pixel la couleur de son representant
			for (i = 0; i < largeur*hauteur; i++)
			{
				//if (!estRepresentant(&objets[i])) { copierPixel( TrouverEnsembleCompressionChemins(&objets[i]) -> pixel, objets[i].pixel );}
				if (!estRepresentant(&objets[i])) { copierPixel( TrouverEnsembleBete(&objets[i]) -> pixel, objets[i].pixel );}
			}

			//On stop l'horloge
			struct timespec current; 
			clock_gettime(CLOCK_REALTIME, &current);
			double t = (( current.tv_sec - myTimerStart.tv_sec) *1000 +
				    ( current.tv_nsec - myTimerStart.tv_nsec)/1000000.0);
			printf( ",%lf)\n", t );

			// On affiche le resultat 
			// Place le pixbuf a visualiser dans le bon widget.
			gtk_image_set_from_pixbuf( GTK_IMAGE( pCtxt->image ), pCtxt->pixbuf_output );
			// Force le reaffichage du widget.
			gtk_widget_queue_draw( pCtxt->image );

		}

		void composantesConnexesCalculRang( GtkWidget *widget, gpointer data )
		{

			Contexte* pCtxt = (Contexte*) data;
			int largeur = gdk_pixbuf_get_width(pCtxt -> pixbuf_input); 
			int hauteur = gdk_pixbuf_get_height(pCtxt -> pixbuf_input);
			int i;
			Objet* objets = CreerEnsembles(pCtxt -> pixbuf_output);
			Pixel* pix_input; Pixel* pix_output;
			int x,y;

			// On seuille a 110 pour éviter d'avoir a le refaire a la main pour chaque test
			for (x = 0; x < largeur; x++)
			{
				for (y = 0; y < hauteur; y++)
				{
					pix_input = gotoPixel(pCtxt -> pixbuf_input,x,y);
					pix_output = gotoPixel(pCtxt -> pixbuf_output,x,y);
					if (greyLevel(pix_input) < 110)	{ setGreyLevel(pix_output,0); }
					else { setGreyLevel(pix_output,255); }
				}
			}

			//On démarre l'horloge
			struct timespec myTimerStart;
			clock_gettime(CLOCK_REALTIME, &myTimerStart);

			//On parcourt le tableau d'objet pour realiser l'union des pixels adjacents identiques
			for (i = 0; i < largeur*hauteur; i++)
			{
				//Pixel a sa droite
				if ( (i + 1) % largeur != 0 && greyLevel(objets[i].pixel) == greyLevel(objets[i+1].pixel) )
				{
					UnionParRang(&objets[i],&objets[i+1]);
				}
				//Pixel en dessous
				if ( i + largeur < largeur*hauteur && greyLevel(objets[i].pixel) == greyLevel(objets[i+largeur].pixel) )
				{
					UnionParRang(&objets[i],&objets[i+largeur]);
				} 
			}

			//On attribue une couleur aleatoire aux representants
			for (i = 0; i < largeur*hauteur; i++)
			{
				if (estRepresentant(&objets[i])) { setRandomColor(objets[i].pixel);}
			}

			//On recopie sur chaque pixel la couleur de son representant
			for (i = 0; i < largeur*hauteur; i++)
			{
				//if (!estRepresentant(&objets[i])) { copierPixel( TrouverEnsembleCompressionChemins(&objets[i]) -> pixel, objets[i].pixel );}
				if (!estRepresentant(&objets[i])) { copierPixel( TrouverEnsembleBete(&objets[i]) -> pixel, objets[i].pixel );}
			}

			//On stop l'horloge
			struct timespec current; 
			clock_gettime(CLOCK_REALTIME, &current);
			double t = (( current.tv_sec - myTimerStart.tv_sec) *1000 +
				    ( current.tv_nsec - myTimerStart.tv_nsec)/1000000.0);
			printf( ",%lf)\n", t );

			// On affiche le resultat 
			// Place le pixbuf a visualiser dans le bon widget.
			gtk_image_set_from_pixbuf( GTK_IMAGE( pCtxt->image ), pCtxt->pixbuf_output );
			// Force le reaffichage du widget.
			gtk_widget_queue_draw( pCtxt->image );

		}

		void composantesConnexesCalculCompRec( GtkWidget *widget, gpointer data )
		{

			Contexte* pCtxt = (Contexte*) data;
			int largeur = gdk_pixbuf_get_width(pCtxt -> pixbuf_input); 
			int hauteur = gdk_pixbuf_get_height(pCtxt -> pixbuf_input);
			int i;
			Objet* objets = CreerEnsembles(pCtxt -> pixbuf_output);
			Pixel* pix_input; Pixel* pix_output;
			int x,y;

			// On seuille a 110 pour éviter d'avoir a le refaire a la main pour chaque test
			for (x = 0; x < largeur; x++)
			{
				for (y = 0; y < hauteur; y++)
				{
					pix_input = gotoPixel(pCtxt -> pixbuf_input,x,y);
					pix_output = gotoPixel(pCtxt -> pixbuf_output,x,y);
					if (greyLevel(pix_input) < 110)	{ setGreyLevel(pix_output,0); }
					else { setGreyLevel(pix_output,255); }
				}
			}

			//On démarre l'horloge
			struct timespec myTimerStart;
			clock_gettime(CLOCK_REALTIME, &myTimerStart);

			//On parcourt le tableau d'objet pour realiser l'union des pixels adjacents identiques
			for (i = 0; i < largeur*hauteur; i++)
			{
				//Pixel a sa droite
				if ( (i + 1) % largeur != 0 && greyLevel(objets[i].pixel) == greyLevel(objets[i+1].pixel) )
				{
					UnionCompressionCheminsRec(&objets[i],&objets[i+1]);
				}
				//Pixel en dessous
				if ( i + largeur < largeur*hauteur && greyLevel(objets[i].pixel) == greyLevel(objets[i+largeur].pixel) )
				{
					UnionCompressionCheminsRec(&objets[i],&objets[i+largeur]);
				} 
			}

			//On attribue une couleur aleatoire aux representants
			for (i = 0; i < largeur*hauteur; i++)
			{
				if (estRepresentant(&objets[i])) { setRandomColor(objets[i].pixel);}
			}

			//On recopie sur chaque pixel la couleur de son representant
			for (i = 0; i < largeur*hauteur; i++)
			{
				if (!estRepresentant(&objets[i])) { copierPixel( TrouverEnsembleCompressionCheminsRec(&objets[i]) -> pixel, objets[i].pixel );}
			}

			//On stop l'horloge
			struct timespec current; 
			clock_gettime(CLOCK_REALTIME, &current);
			double t = (( current.tv_sec - myTimerStart.tv_sec) *1000 +
				    ( current.tv_nsec - myTimerStart.tv_nsec)/1000000.0);
			printf( ",%lf)\n", t );

			// On affiche le resultat 
			// Place le pixbuf a visualiser dans le bon widget.
			gtk_image_set_from_pixbuf( GTK_IMAGE( pCtxt->image ), pCtxt->pixbuf_output );
			// Force le reaffichage du widget.
			gtk_widget_queue_draw( pCtxt->image );

		}

		void composantesConnexesCalculCompWhile( GtkWidget *widget, gpointer data )
		{

			Contexte* pCtxt = (Contexte*) data;
			int largeur = gdk_pixbuf_get_width(pCtxt -> pixbuf_input); 
			int hauteur = gdk_pixbuf_get_height(pCtxt -> pixbuf_input);
			int i;
			Objet* objets = CreerEnsembles(pCtxt -> pixbuf_output);
			Pixel* pix_input; Pixel* pix_output;
			int x,y;

			// On seuille a 110 pour éviter d'avoir a le refaire a la main pour chaque test
			for (x = 0; x < largeur; x++)
			{
				for (y = 0; y < hauteur; y++)
				{
					pix_input = gotoPixel(pCtxt -> pixbuf_input,x,y);
					pix_output = gotoPixel(pCtxt -> pixbuf_output,x,y);
					if (greyLevel(pix_input) < 110)	{ setGreyLevel(pix_output,0); }
					else { setGreyLevel(pix_output,255); }
				}
			}

			//On démarre l'horloge
			struct timespec myTimerStart;
			clock_gettime(CLOCK_REALTIME, &myTimerStart);

			//On parcourt le tableau d'objet pour realiser l'union des pixels adjacents identiques
			for (i = 0; i < largeur*hauteur; i++)
			{
				//Pixel a sa droite
				if ( (i + 1) % largeur != 0 && greyLevel(objets[i].pixel) == greyLevel(objets[i+1].pixel) )
				{
					UnionCompressionCheminsWhile(&objets[i],&objets[i+1]);
				}
				//Pixel en dessous
				if ( i + largeur < largeur*hauteur && greyLevel(objets[i].pixel) == greyLevel(objets[i+largeur].pixel) )
				{
					UnionCompressionCheminsWhile(&objets[i],&objets[i+largeur]);
				} 
			}

			//On attribue une couleur aleatoire aux representants
			for (i = 0; i < largeur*hauteur; i++)
			{
				if (estRepresentant(&objets[i])) { setRandomColor(objets[i].pixel);}
			}

			//On recopie sur chaque pixel la couleur de son representant
			for (i = 0; i < largeur*hauteur; i++)
			{
				if (!estRepresentant(&objets[i])) { copierPixel( TrouverEnsembleCompressionCheminsWhile(&objets[i]) -> pixel, objets[i].pixel );}
			}

			//On stop l'horloge
			struct timespec current; 
			clock_gettime(CLOCK_REALTIME, &current);
			double t = (( current.tv_sec - myTimerStart.tv_sec) *1000 +
				    ( current.tv_nsec - myTimerStart.tv_nsec)/1000000.0);
			printf( ",%lf)\n", t );

			// On affiche le resultat 
			// Place le pixbuf a visualiser dans le bon widget.
			gtk_image_set_from_pixbuf( GTK_IMAGE( pCtxt->image ), pCtxt->pixbuf_output );
			// Force le reaffichage du widget.
			gtk_widget_queue_draw( pCtxt->image );

		}

		void composantesConnexesCalculCompUnNoeud( GtkWidget *widget, gpointer data )
		{

			Contexte* pCtxt = (Contexte*) data;
			int largeur = gdk_pixbuf_get_width(pCtxt -> pixbuf_input); 
			int hauteur = gdk_pixbuf_get_height(pCtxt -> pixbuf_input);
			int i;
			Objet* objets = CreerEnsembles(pCtxt -> pixbuf_output);
			Pixel* pix_input; Pixel* pix_output;
			int x,y;

			// On seuille a 110 pour éviter d'avoir a le refaire a la main pour chaque test
			for (x = 0; x < largeur; x++)
			{
				for (y = 0; y < hauteur; y++)
				{
					pix_input = gotoPixel(pCtxt -> pixbuf_input,x,y);
					pix_output = gotoPixel(pCtxt -> pixbuf_output,x,y);
					if (greyLevel(pix_input) < 110)	{ setGreyLevel(pix_output,0); }
					else { setGreyLevel(pix_output,255); }
				}
			}

			//On démarre l'horloge
			struct timespec myTimerStart;
			clock_gettime(CLOCK_REALTIME, &myTimerStart);

			//On parcourt le tableau d'objet pour realiser l'union des pixels adjacents identiques
			for (i = 0; i < largeur*hauteur; i++)
			{
				//Pixel a sa droite
				if ( (i + 1) % largeur != 0 && greyLevel(objets[i].pixel) == greyLevel(objets[i+1].pixel) )
				{
					UnionCompressionCheminsUnNoeud(&objets[i],&objets[i+1]);
				}
				//Pixel en dessous
				if ( i + largeur < largeur*hauteur && greyLevel(objets[i].pixel) == greyLevel(objets[i+largeur].pixel) )
				{
					UnionCompressionCheminsUnNoeud(&objets[i],&objets[i+largeur]);
				} 
			}

			//On attribue une couleur aleatoire aux representants
			for (i = 0; i < largeur*hauteur; i++)
			{
				if (estRepresentant(&objets[i])) { setRandomColor(objets[i].pixel);}
			}

			//On recopie sur chaque pixel la couleur de son representant
			for (i = 0; i < largeur*hauteur; i++)
			{
				if (!estRepresentant(&objets[i])) { copierPixel( TrouverEnsembleCompressionCheminsUnNoeud(&objets[i]) -> pixel, objets[i].pixel );}
			}

			//On stop l'horloge
			struct timespec current; 
			clock_gettime(CLOCK_REALTIME, &current);
			double t = (( current.tv_sec - myTimerStart.tv_sec) *1000 +
				    ( current.tv_nsec - myTimerStart.tv_nsec)/1000000.0);
			printf( ",%lf)\n", t );

			// On affiche le resultat 
			// Place le pixbuf a visualiser dans le bon widget.
			gtk_image_set_from_pixbuf( GTK_IMAGE( pCtxt->image ), pCtxt->pixbuf_output );
			// Force le reaffichage du widget.
			gtk_widget_queue_draw( pCtxt->image );

		}

		void composantesConnexesCalculRangEtCompRec( GtkWidget *widget, gpointer data )
		{

			Contexte* pCtxt = (Contexte*) data;
			int largeur = gdk_pixbuf_get_width(pCtxt -> pixbuf_input); 
			int hauteur = gdk_pixbuf_get_height(pCtxt -> pixbuf_input);
			int i;
			Objet* objets = CreerEnsembles(pCtxt -> pixbuf_output);
			Pixel* pix_input; Pixel* pix_output;
			int x,y;

			// On seuille a 110 pour éviter d'avoir a le refaire a la main pour chaque test
			for (x = 0; x < largeur; x++)
			{
				for (y = 0; y < hauteur; y++)
				{
					pix_input = gotoPixel(pCtxt -> pixbuf_input,x,y);
					pix_output = gotoPixel(pCtxt -> pixbuf_output,x,y);
					if (greyLevel(pix_input) < 110)	{ setGreyLevel(pix_output,0); }
					else { setGreyLevel(pix_output,255); }
				}
			}

			//On démarre l'horloge
			struct timespec myTimerStart;
			clock_gettime(CLOCK_REALTIME, &myTimerStart);

			//On parcourt le tableau d'objet pour realiser l'union des pixels adjacents identiques
			for (i = 0; i < largeur*hauteur; i++)
			{
				//Pixel a sa droite
				if ( (i + 1) % largeur != 0 && greyLevel(objets[i].pixel) == greyLevel(objets[i+1].pixel) )
				{
					UnionRangEtCompRec(&objets[i],&objets[i+1]);
				}
				//Pixel en dessous
				if ( i + largeur < largeur*hauteur && greyLevel(objets[i].pixel) == greyLevel(objets[i+largeur].pixel) )
				{
					UnionRangEtCompRec(&objets[i],&objets[i+largeur]);
				} 
			}

			//On attribue une couleur aleatoire aux representants
			for (i = 0; i < largeur*hauteur; i++)
			{
				if (estRepresentant(&objets[i])) { setRandomColor(objets[i].pixel);}
			}

			//On recopie sur chaque pixel la couleur de son representant
			for (i = 0; i < largeur*hauteur; i++)
			{
				if (!estRepresentant(&objets[i])) { copierPixel( TrouverEnsembleCompressionCheminsRec(&objets[i]) -> pixel, objets[i].pixel );}
			}

			//On stop l'horloge
			struct timespec current; 
			clock_gettime(CLOCK_REALTIME, &current);
			double t = (( current.tv_sec - myTimerStart.tv_sec) *1000 +
				    ( current.tv_nsec - myTimerStart.tv_nsec)/1000000.0);
			printf( ",%lf)\n", t );

			// On affiche le resultat 
			// Place le pixbuf a visualiser dans le bon widget.
			gtk_image_set_from_pixbuf( GTK_IMAGE( pCtxt->image ), pCtxt->pixbuf_output );
			// Force le reaffichage du widget.
			gtk_widget_queue_draw( pCtxt->image );

		}

		void composantesConnexesCalculRangEtCompWhile( GtkWidget *widget, gpointer data )
		{

			Contexte* pCtxt = (Contexte*) data;
			int largeur = gdk_pixbuf_get_width(pCtxt -> pixbuf_input); 
			int hauteur = gdk_pixbuf_get_height(pCtxt -> pixbuf_input);
			int i;
			Objet* objets = CreerEnsembles(pCtxt -> pixbuf_output);
			Pixel* pix_input; Pixel* pix_output;
			int x,y;

			// On seuille a 110 pour éviter d'avoir a le refaire a la main pour chaque test
			for (x = 0; x < largeur; x++)
			{
				for (y = 0; y < hauteur; y++)
				{
					pix_input = gotoPixel(pCtxt -> pixbuf_input,x,y);
					pix_output = gotoPixel(pCtxt -> pixbuf_output,x,y);
					if (greyLevel(pix_input) < 110)	{ setGreyLevel(pix_output,0); }
					else { setGreyLevel(pix_output,255); }
				}
			}

			//On démarre l'horloge
			struct timespec myTimerStart;
			clock_gettime(CLOCK_REALTIME, &myTimerStart);

			//On parcourt le tableau d'objet pour realiser l'union des pixels adjacents identiques
			for (i = 0; i < largeur*hauteur; i++)
			{
				//Pixel a sa droite
				if ( (i + 1) % largeur != 0 && greyLevel(objets[i].pixel) == greyLevel(objets[i+1].pixel) )
				{
					UnionRangEtCompWhile(&objets[i],&objets[i+1]);
				}
				//Pixel en dessous
				if ( i + largeur < largeur*hauteur && greyLevel(objets[i].pixel) == greyLevel(objets[i+largeur].pixel) )
				{
					UnionRangEtCompWhile(&objets[i],&objets[i+largeur]);
				} 
			}

			//On attribue une couleur aleatoire aux representants
			for (i = 0; i < largeur*hauteur; i++)
			{
				if (estRepresentant(&objets[i])) { setRandomColor(objets[i].pixel);}
			}

			//On recopie sur chaque pixel la couleur de son representant
			for (i = 0; i < largeur*hauteur; i++)
			{
				if (!estRepresentant(&objets[i])) { copierPixel( TrouverEnsembleCompressionCheminsWhile(&objets[i]) -> pixel, objets[i].pixel );}
			}

			//On stop l'horloge
			struct timespec current; 
			clock_gettime(CLOCK_REALTIME, &current);
			double t = (( current.tv_sec - myTimerStart.tv_sec) *1000 +
				    ( current.tv_nsec - myTimerStart.tv_nsec)/1000000.0);
			printf( ",%lf)\n", t );

			// On affiche le resultat 
			// Place le pixbuf a visualiser dans le bon widget.
			gtk_image_set_from_pixbuf( GTK_IMAGE( pCtxt->image ), pCtxt->pixbuf_output );
			// Force le reaffichage du widget.
			gtk_widget_queue_draw( pCtxt->image );

		}

		void composantesConnexesCalculRangEtCompUnNoeud( GtkWidget *widget, gpointer data )
		{

			Contexte* pCtxt = (Contexte*) data;
			int largeur = gdk_pixbuf_get_width(pCtxt -> pixbuf_input); 
			int hauteur = gdk_pixbuf_get_height(pCtxt -> pixbuf_input);
			int i;
			Objet* objets = CreerEnsembles(pCtxt -> pixbuf_output);
			Pixel* pix_input; Pixel* pix_output;
			int x,y;

			// On seuille a 110 pour éviter d'avoir a le refaire a la main pour chaque test
			for (x = 0; x < largeur; x++)
			{
				for (y = 0; y < hauteur; y++)
				{
					pix_input = gotoPixel(pCtxt -> pixbuf_input,x,y);
					pix_output = gotoPixel(pCtxt -> pixbuf_output,x,y);
					if (greyLevel(pix_input) < 110)	{ setGreyLevel(pix_output,0); }
					else { setGreyLevel(pix_output,255); }
				}
			}

			//On démarre l'horloge
			struct timespec myTimerStart;
			clock_gettime(CLOCK_REALTIME, &myTimerStart);

			//On parcourt le tableau d'objet pour realiser l'union des pixels adjacents identiques
			for (i = 0; i < largeur*hauteur; i++)
			{
				//Pixel a sa droite
				if ( (i + 1) % largeur != 0 && greyLevel(objets[i].pixel) == greyLevel(objets[i+1].pixel) )
				{
					UnionRangEtCompUnNoeud(&objets[i],&objets[i+1]);
				}
				//Pixel en dessous
				if ( i + largeur < largeur*hauteur && greyLevel(objets[i].pixel) == greyLevel(objets[i+largeur].pixel) )
				{
					UnionRangEtCompUnNoeud(&objets[i],&objets[i+largeur]);
				} 
			}

			//On attribue une couleur aleatoire aux representants
			for (i = 0; i < largeur*hauteur; i++)
			{
				if (estRepresentant(&objets[i])) { setRandomColor(objets[i].pixel);}
			}

			//On recopie sur chaque pixel la couleur de son representant
			for (i = 0; i < largeur*hauteur; i++)
			{
				if (!estRepresentant(&objets[i])) { copierPixel( TrouverEnsembleCompressionCheminsUnNoeud(&objets[i]) -> pixel, objets[i].pixel );}
			}

			//On stop l'horloge
			struct timespec current; 
			clock_gettime(CLOCK_REALTIME, &current);
			double t = (( current.tv_sec - myTimerStart.tv_sec) *1000 +
				    ( current.tv_nsec - myTimerStart.tv_nsec)/1000000.0);
			printf( ",%lf)\n", t );

			// On affiche le resultat 
			// Place le pixbuf a visualiser dans le bon widget.
			gtk_image_set_from_pixbuf( GTK_IMAGE( pCtxt->image ), pCtxt->pixbuf_output );
			// Force le reaffichage du widget.
			gtk_widget_queue_draw( pCtxt->image );

		}

//-----------------------------------------------------------------------------
// question 4 : Affichage de la couleur moyenne pour chaque region
//-----------------------------------------------------------------------------

void composantesConnexesCoulMoy( GtkWidget *widget, gpointer data )
{
	Contexte* pCtxt = (Contexte*) data;
	int largeur = gdk_pixbuf_get_width(pCtxt -> pixbuf_input); 
	int hauteur = gdk_pixbuf_get_height(pCtxt -> pixbuf_input);
	int i;
	Objet* objets = CreerEnsembles(pCtxt -> pixbuf_output);

	//Creation et initialisation du tableau stats
	StatCouleur* stats = (StatCouleur*) malloc( largeur*hauteur * sizeof( StatCouleur ) );
	initialiseStatCouleur(stats, largeur*hauteur);

	//On parcourt le tableau d'objet pour realiser l'union des pixels adjacents identiques
	for (i = 0; i < largeur*hauteur; i++)
	{
		//Pixel a sa droite
		if ( (i + 1) % largeur != 0 && greyLevel(objets[i].pixel) == greyLevel(objets[i+1].pixel) )
		{
			UnionRangEtCompUnNoeud(&objets[i],&objets[i+1]);
		}
		//Pixel en dessous
		if ( i + largeur < largeur*hauteur && greyLevel(objets[i].pixel) == greyLevel(objets[i+largeur].pixel) )
		{
			UnionRangEtCompUnNoeud(&objets[i],&objets[i+largeur]);
		} 
	}

	//Utilisation du morceau de code propose dans l'enonce pour calculer la couleur moyenne
	guchar* data_input = gdk_pixbuf_get_pixels( pCtxt->pixbuf_input );
	guchar* data_output = gdk_pixbuf_get_pixels( pCtxt->pixbuf_output );
	for ( i = 0; i < largeur*hauteur; ++i )
	{
		Objet* rep = TrouverEnsembleCompressionCheminsUnNoeud( &objets[ i ] );
		long int j = rep - objets;
		Pixel* pixel_src = (Pixel*) ( data_input + ( (guchar*) objets[ i ].pixel - data_output ) );
		// pixel_src est la couleur de ce pixel dans l'image input.
		// On l'ajoute a la stat du representant j.
		stats[ j ].rouge += pixel_src->rouge;
		stats[ j ].vert += pixel_src->vert;
		stats[ j ].bleu += pixel_src->bleu;
		stats[ j ].nb += 1; // On aura donc la somme cumulee
	}

	//On attribue la couleur moyenne de sa region a chaque representant
	for (i = 0; i < largeur*hauteur; i++)
	{
		if (estRepresentant(&objets[i])) { 
			setColorFromStat(objets[i].pixel,stats[i]);
		}
	}

	//On recopie sur chaque pixel la couleur de son representant
	for (i = 0; i < largeur*hauteur; i++)
	{
		if (!estRepresentant(&objets[i])) { copierPixel( TrouverEnsembleCompressionCheminsUnNoeud(&objets[i]) -> pixel, objets[i].pixel );}
	}

	// On affiche le resultat 
	// Place le pixbuf a visualiser dans le bon widget.
	gtk_image_set_from_pixbuf( GTK_IMAGE( pCtxt->image ), pCtxt->pixbuf_output );
	// Force le reaffichage du widget.
	gtk_widget_queue_draw( pCtxt->image );
}

//Attribue la couleur moyenne a un pixel a partir d'une StatCouleur donnee
void setColorFromStat(Pixel* pix, StatCouleur stat)
{
	pix -> rouge = stat.rouge / stat.nb;
	pix -> vert = stat.vert / stat.nb;
	pix -> bleu = stat.bleu / stat.nb;
}

//Initialisation du tableau de stats
void initialiseStatCouleur(StatCouleur* stats, int taille)
{
	int i;
	for (i = 0; i < taille; i++)
	{
		stats[i].rouge = 0;
		stats[i].vert = 0;
		stats[i].bleu = 0;
		stats[i].nb = 0;
	}
}

//-----------------------------------------------------------------------------
// question 5 : Connexite floue et decoupage en composantes connexes floues
//-----------------------------------------------------------------------------
void composantesConnexesFloues( GtkWidget *widget, gpointer data )
{
	Contexte* pCtxt = (Contexte*) data;
	int largeur = gdk_pixbuf_get_width(pCtxt -> pixbuf_input); 
	int hauteur = gdk_pixbuf_get_height(pCtxt -> pixbuf_input);
	int i,x,y;
	Pixel* pix_input; Pixel* pix_output;
	Objet* objets = CreerEnsembles(pCtxt -> pixbuf_output);

	//Recuperation de la valeur de alpha
	gdouble alpha = gtk_range_get_value( (GtkRange*)(pCtxt -> alpha) );

	//Creation et initialisation du tableau stats
	StatCouleur* stats = (StatCouleur*) malloc( largeur*hauteur * sizeof( StatCouleur ) );
	initialiseStatCouleur(stats, largeur*hauteur);

	//On copie l'image d'origine dans le pixbut_output pour travailler directement dessus
	for (x = 0; x < largeur; x++)
	{
		for (y = 0; y < hauteur; y++)
		{
			pix_input = gotoPixel(pCtxt -> pixbuf_input,x,y);
			pix_output = gotoPixel(pCtxt -> pixbuf_output,x,y);
			copierPixel(pix_input,pix_output);
		}
	}

	//On parcourt le tableau d'objet pour realiser l'UnionUnionRangEtComp des pixels adjacents similaires
	for (i = 0; i < largeur*hauteur; i++)
	{
		if ( (i + 1) % largeur != 0 && similitude(objets[i].pixel, objets[i+1].pixel) < alpha )
		{
			UnionRangEtCompUnNoeud(&objets[i],&objets[i+1]);
		}
		if ( i + largeur < largeur*hauteur && similitude(objets[i].pixel, objets[i+largeur].pixel) < alpha )
		{
			UnionRangEtCompUnNoeud(&objets[i],&objets[i+largeur]);
		} 
	}

	//Utilisation du morceau de code propose dans l'enonce pour calculer la couleur moyenne
	guchar* data_input = gdk_pixbuf_get_pixels( pCtxt->pixbuf_input );
	guchar* data_output = gdk_pixbuf_get_pixels( pCtxt->pixbuf_output );
	for ( i = 0; i < largeur*hauteur; ++i )
		{
			Objet* rep = TrouverEnsembleCompressionCheminsUnNoeud( &objets[ i ] );
			long int j = rep - objets;
			Pixel* pixel_src = (Pixel*) ( data_input + ( (guchar*) objets[ i ].pixel - data_output ) );
			// pixel_src est la couleur de ce pixel dans l'image input.
			// On l'ajoute a la stat du representant j.
			stats[ j ].rouge += pixel_src->rouge;
			stats[ j ].vert += pixel_src->vert;
			stats[ j ].bleu += pixel_src->bleu;
			stats[ j ].nb += 1; // On aura donc la somme cumulee
		}

	//On attribue la couleur moyenne de sa region a chaque representant
	for (i = 0; i < largeur*hauteur; i++)
	{
		if (estRepresentant(&objets[i])) { 
			setColorFromStat(objets[i].pixel,stats[i]);
		}
	}

	//On recopie sur chaque pixel la couleur de son representant
	for (i = 0; i < largeur*hauteur; i++)
	{
		if (!estRepresentant(&objets[i])) { copierPixel( TrouverEnsembleCompressionCheminsUnNoeud(&objets[i]) -> pixel, objets[i].pixel );}
	}

	// On affiche le resultat 
	// Place le pixbuf a visualiser dans le bon widget.
	gtk_image_set_from_pixbuf( GTK_IMAGE( pCtxt->image ), pCtxt->pixbuf_output );
	// Force le reaffichage du widget.
	gtk_widget_queue_draw( pCtxt->image );
}

//Utilitaires
double minimum(double x, double y, double z)
{
	if (x < y) { if (x < z) { return x; } return z; } return y;
}
double maximum(double x, double y, double z)
{
	if (x > y) { if (x > z) { return x; } return z; } return y;
}

//Utilisation des formules de la page wikipedia pour transformer rgb en tsv
TSVCouleur tsv( Pixel* pixel )
{
	int t;
	double s,v;
	TSVCouleur retour;

	//rgb doivent etre donnes par un double entre 0 et 1
	double r = (double) (pixel -> rouge) / (double)255;
	double g = (double) (pixel -> vert) / (double)255;
	double b = (double) (pixel -> bleu) / (double)255;

	double min = minimum(r, g, b);
	double max = maximum(r, g, b);

	//Application des formules
	if (min == max) { t = 0; }
	else
	{
		if (min == r) { t = ((int)(60*(g-b)/(max-min)+360))%360; }
		else if (min == g) { t = (60*(b-r)/(max-min)+120); }
		else if (min == b) { t = (60*(r-g)/(max-min)+240); }
	}

	if (max == 0) { s = 0; }
	else { s = 1 - min/max; }

	v = max;

	//Retourner la couleur TSV
	retour.t = t;
	retour.s = s;
	retour.v = v;
	return retour;

}

//Fonction de similitude par distance euclidienne
double similitude( Pixel* p1, Pixel* p2 ) 
{
	TSVCouleur tsv1 = tsv( p1 );
	TSVCouleur tsv2 = tsv( p2 );
	return abs((tsv1.t - tsv2.t)*(tsv1.t - tsv2.t) + ( tsv1.s - tsv2.s )*( tsv1.s - tsv2.s ) + ( tsv1.v - tsv2.v )*( tsv1.v - tsv2.v ));
}

//Fonction de similitude proposée dans l'énoncé
/*double similitudeBis( Pixel* p1, Pixel* p2 ) 
{
	TSVCouleur tsv1 = tsv( p1 );
	TSVCouleur tsv2 = tsv( p2 );
	int diff = tsv1.t - tsv2.t;
	while ( diff >= 180 ) diff -= 360;
	while ( diff <= -180 ) diff += 360;
	return (abs( (double) diff ) + 5.0 * abs( tsv1.s - tsv2.s ) + 10.0 * abs( tsv1.v - tsv2.v ));
}*/

//-----------------------------------------------------------------------------
// Charge l'image donnee et cree l'interface.
//-----------------------------------------------------------------------------
// Nous avons placé les boutons en haut de l'image plutôt qu'en dessous. Cela nous a permis de réaliser nos tests sur des images dépassant la taille de notre écran. Nous avons de plus ajouter des boutons également pour faciliter les tests.
GtkWidget* creerIHM( const char* image_filename, Contexte* pCtxt )
{
	GtkWidget* window;
	GtkWidget* vbox1;
	GtkWidget* vbox2;
	GtkWidget* hbox1;
	GtkWidget* button_quit;
	GtkWidget* button_select_input;
	GtkWidget* button_select_output;
	GError** error = NULL;

	//Declaration des composants que nous avons ajoutes
	GdkColor rouge, orange, marron, rose, bleuc, bleuf, vert, violet;
	GtkWidget* seuil_button;
	GtkWidget* seuil_widget;
	GtkWidget* compCo_button;
	GtkWidget* compCoCalculBete_button;
	GtkWidget* compCoCalculRang_button;
	GtkWidget* compCoCalculCompRec_button;
	GtkWidget* compCoCalculCompWhile_button;
	GtkWidget* compCoCalculCompUnNoeud_button;
	GtkWidget* compCoCalculRangEtCompRec_button;
	GtkWidget* compCoCalculRangEtCompWhile_button;
	GtkWidget* compCoCalculRangEtCompUnNoeud_button;
	GtkWidget* compCoCoulMoy_button;
	GtkWidget* compCoFloues_button;
	GtkWidget* compCoFloues_widget;
	GtkWidget* tropgrand_label;
	GtkWidget* hbox2;

	/* Cree une fenetre. */
	window = gtk_window_new( GTK_WINDOW_TOPLEVEL );
	gtk_window_maximize (GTK_WINDOW (window));

	// Cree deux conteneurs horitzontaux box.
	hbox1 = gtk_hbox_new( FALSE, 10 ); //image suivie de vbox2
	hbox2 = gtk_hbox_new( FALSE, 10 ); //Conteneur principal

	// Cree deux conteneurs vertical box.
	vbox1 = gtk_vbox_new( FALSE, 10 ); //bts d'action
	vbox2 = gtk_vbox_new( FALSE, 10 ); //bts input et output


	// Cree le pixbuf source et le pixbuf destination
	pCtxt->pixbuf_input = gdk_pixbuf_new_from_file( image_filename, error );
	pCtxt->width = gdk_pixbuf_get_width( pCtxt->pixbuf_input ); // Largeur de l'image en pixels
	pCtxt->height = gdk_pixbuf_get_height( pCtxt->pixbuf_input ); // Hauteur de l'image en pixels
	pCtxt->pixbuf_output = creerImage( pCtxt->width, pCtxt->height );
	analyzePixbuf( pCtxt->pixbuf_input );
	disk( pCtxt->pixbuf_output, 100 );
	// Cree le widget qui affiche le pixbuf image.
	pCtxt->image = gtk_image_new_from_pixbuf( pCtxt->pixbuf_input );

	// Rajoute le 2eme vbox dans le conteneur hbox (pour mettre les boutons selecteur d'image).
	//gtk_container_add( GTK_CONTAINER( hbox1 ), vbox2 );

	// Cree les boutons de selection "source"/"destination".
	button_select_input	= gtk_button_new_with_label( "Input" );
	button_select_output = gtk_button_new_with_label( "Output" );
	// Connecte les reactions selectInput et selectOutput a l'evenement "clic" sur ces boutons.
	g_signal_connect( button_select_input, "clicked",
										G_CALLBACK( selectInput ),
										pCtxt );
	g_signal_connect( button_select_output, "clicked",
										G_CALLBACK( selectOutput ),
										pCtxt );
	gtk_container_add( GTK_CONTAINER( vbox2 ), button_select_input );
	gtk_container_add( GTK_CONTAINER( vbox2 ), button_select_output );
	// Cree le bouton quitter.
	button_quit = gtk_button_new_with_label( "Quitter" );
	// Connecte la reaction gtk_main_quit a l'evenement "clic" sur ce bouton.
	g_signal_connect( button_quit, "clicked",
										G_CALLBACK( gtk_main_quit ),
										NULL);

	// Cree nos composants
	seuil_widget = gtk_hscale_new_with_range( 0, 255, 1 );
	pCtxt->seuil = seuil_widget;
	seuil_button = gtk_button_new_with_label( "Seuiller" );
	compCo_button = gtk_button_new_with_label("Composantes connexes (seuillez d'abord svp)");
	compCoCalculBete_button = gtk_button_new_with_label("Calcul bete");
	rouge.pixel = 2;
	rouge.red = 255*257;
	rouge.green = 0*257;
	rouge.blue = 0*257;
	gtk_widget_modify_bg (compCoCalculBete_button, GTK_STATE_PRELIGHT, &rouge);
	gtk_widget_modify_bg (compCoCalculBete_button, GTK_STATE_ACTIVE, &rouge);
	compCoCalculRang_button = gtk_button_new_with_label("Calcul rang");
	orange.pixel = 2;
	orange.red = 255*257;
	orange.green = 180*257;
	orange.blue = 0*257;
	gtk_widget_modify_bg (compCoCalculRang_button, GTK_STATE_PRELIGHT, &orange);
	gtk_widget_modify_bg (compCoCalculRang_button, GTK_STATE_ACTIVE, &orange);
	compCoCalculCompRec_button = gtk_button_new_with_label("Calcul compression recursive");
	marron.pixel = 2;
	marron.red = 150*257;
	marron.green = 60*257;
	marron.blue = 0*257;
	gtk_widget_modify_bg (compCoCalculCompRec_button, GTK_STATE_PRELIGHT, &marron);
	gtk_widget_modify_bg (compCoCalculCompRec_button, GTK_STATE_ACTIVE, &marron);
	compCoCalculCompWhile_button = gtk_button_new_with_label("Calcul compression while");
	rose.pixel = 2;
	rose.red = 255*257;
	rose.green = 0*257;
	rose.blue = 255*257;
	gtk_widget_modify_bg (compCoCalculCompWhile_button, GTK_STATE_PRELIGHT, &rose);
	gtk_widget_modify_bg (compCoCalculCompWhile_button, GTK_STATE_ACTIVE, &rose);
	compCoCalculCompUnNoeud_button = gtk_button_new_with_label("Calcul compression un noeud");
	violet.pixel = 2;
	violet.red = 140*257;
	violet.green = 0*257;
	violet.blue = 255*257;
	gtk_widget_modify_bg (compCoCalculCompUnNoeud_button, GTK_STATE_PRELIGHT, &violet);
	gtk_widget_modify_bg (compCoCalculCompUnNoeud_button, GTK_STATE_ACTIVE, &violet);
	compCoCalculRangEtCompRec_button = gtk_button_new_with_label("Calcul rang + compression recursive");
	bleuc.pixel = 2;
	bleuc.red = 0*257;
	bleuc.green = 255*257;
	bleuc.blue = 255*257;
	gtk_widget_modify_bg (compCoCalculRangEtCompRec_button, GTK_STATE_PRELIGHT, &bleuc);
	gtk_widget_modify_bg (compCoCalculRangEtCompRec_button, GTK_STATE_ACTIVE, &bleuc);
	compCoCalculRangEtCompWhile_button = gtk_button_new_with_label("Calcul rang + compression while");
	bleuf.pixel = 2;
	bleuf.red = 0*257;
	bleuf.green = 0*257;
	bleuf.blue = 255*257;
	gtk_widget_modify_bg (compCoCalculRangEtCompWhile_button, GTK_STATE_PRELIGHT, &bleuf);
	gtk_widget_modify_bg (compCoCalculRangEtCompWhile_button, GTK_STATE_ACTIVE, &bleuf);
	compCoCalculRangEtCompUnNoeud_button = gtk_button_new_with_label("Calcul rang + compression un noeud");
	vert.pixel = 2;
	vert.red = 0*257;
	vert.green = 255*257;
	vert.blue = 0*257;
	gtk_widget_modify_bg (compCoCalculRangEtCompUnNoeud_button, GTK_STATE_PRELIGHT, &vert);
	gtk_widget_modify_bg (compCoCalculRangEtCompUnNoeud_button, GTK_STATE_ACTIVE, &vert);
	compCoCoulMoy_button = gtk_button_new_with_label("Composantes connexes couleurs moyennes (seuillez d'abord svp)");
	compCoFloues_widget = gtk_hscale_new_with_range( 0, 20, 1 );
	pCtxt->alpha = compCoFloues_widget;
	compCoFloues_button = gtk_button_new_with_label("Composantes connexes floues (ne seuillez pas svp)");
	tropgrand_label = gtk_label_new( "  Image trop grande pour être affichée  ... les calculs auront quand même lieu" );

	// Connecte nos composants aux actions concernees
	g_signal_connect( seuil_button, "clicked",
										G_CALLBACK( seuiller ),
										pCtxt);
	g_signal_connect( compCo_button, "clicked",
										G_CALLBACK( composantesConnexes ),
										pCtxt);
	g_signal_connect( compCoCalculBete_button, "clicked",
										G_CALLBACK( composantesConnexesCalculBete ),
										pCtxt);
	g_signal_connect( compCoCalculRang_button, "clicked",
										G_CALLBACK( composantesConnexesCalculRang ),
										pCtxt);
	g_signal_connect( compCoCalculCompRec_button, "clicked",
										G_CALLBACK( composantesConnexesCalculCompRec ),
										pCtxt);
	g_signal_connect( compCoCalculCompWhile_button, "clicked",
										G_CALLBACK( composantesConnexesCalculCompWhile ),
										pCtxt);
	g_signal_connect( compCoCalculCompUnNoeud_button, "clicked",
										G_CALLBACK( composantesConnexesCalculRangEtCompUnNoeud ),
										pCtxt);
	g_signal_connect( compCoCalculRangEtCompRec_button, "clicked",
										G_CALLBACK( composantesConnexesCalculRangEtCompRec ),
										pCtxt);
	g_signal_connect( compCoCalculRangEtCompWhile_button, "clicked",
										G_CALLBACK( composantesConnexesCalculRangEtCompWhile ),
										pCtxt);
	g_signal_connect( compCoCalculRangEtCompUnNoeud_button, "clicked",
										G_CALLBACK( composantesConnexesCalculRangEtCompUnNoeud ),
										pCtxt);
	g_signal_connect( compCoCoulMoy_button, "clicked",
										G_CALLBACK( composantesConnexesCoulMoy ),
										pCtxt);
	g_signal_connect( compCoFloues_button, "clicked",
										G_CALLBACK( composantesConnexesFloues ),
										pCtxt);



	// Rajoute nos composants dans le conteneur vbox
	gtk_container_add( GTK_CONTAINER( vbox1 ), seuil_widget );
	gtk_container_add( GTK_CONTAINER( vbox1 ), seuil_button );
	gtk_container_add( GTK_CONTAINER( vbox1 ), compCo_button );

	gtk_container_add( GTK_CONTAINER( vbox1 ), compCoCalculBete_button );
	gtk_container_add( GTK_CONTAINER( vbox1 ), compCoCalculRang_button );
	gtk_container_add( GTK_CONTAINER( vbox1 ), compCoCalculCompRec_button );
	gtk_container_add( GTK_CONTAINER( vbox1 ), compCoCalculCompWhile_button );
	gtk_container_add( GTK_CONTAINER( vbox1 ), compCoCalculCompUnNoeud_button );
	gtk_container_add( GTK_CONTAINER( vbox1 ), compCoCalculRangEtCompRec_button );
	gtk_container_add( GTK_CONTAINER( vbox1 ), compCoCalculRangEtCompWhile_button );
	gtk_container_add( GTK_CONTAINER( vbox1 ), compCoCalculRangEtCompUnNoeud_button );

	gtk_container_add( GTK_CONTAINER( vbox1 ), compCoCoulMoy_button );
	gtk_container_add( GTK_CONTAINER( vbox1 ), compCoFloues_widget );
	gtk_container_add( GTK_CONTAINER( vbox1 ), compCoFloues_button );
	// Rajoute le bouton quit dans le vbox
	gtk_container_add( GTK_CONTAINER( vbox1 ), button_quit );

	// Rajoute le hbox dans le conteneur principal.
	gtk_container_add( GTK_CONTAINER( hbox2 ), vbox1 );
	gtk_container_add( GTK_CONTAINER( hbox2 ), vbox2 );
	if (pCtxt -> height < 1000) {
		gtk_container_add( GTK_CONTAINER( hbox2 ), pCtxt->image );
	}
	else {
		gtk_container_add( GTK_CONTAINER( hbox2 ), tropgrand_label);
	}

	// Rajoute la vbox	dans le conteneur window.
	gtk_container_add( GTK_CONTAINER( window ), hbox2 );

	// Rend tout visible
	gtk_widget_show_all( window );

	return window;
}

//-----------------------------------------------------------------------------
// AnalysePixbuf modifié pour faciliter les tests
//-----------------------------------------------------------------------------
/** 
		Utile pour verifier que le GdkPixbuf a un formal usuel: 3 canaux RGB, 24 bits par pixel,
		et que la machine supporte l'alignement de la structure sur 3 octets.
*/
// Nous avons modifié l'affichage pour avoir directement le couple (tailleImage,tpsCalcul), plus simple pour utiliser nos résultats de test à la question 3.3 //
void analyzePixbuf( GdkPixbuf* pixbuf )
{
	int n_channels = gdk_pixbuf_get_n_channels( pixbuf ); // Nb de canaux (Rouge, Vert, Bleu, potentiellement Alpha)
	int has_alpha = gdk_pixbuf_get_has_alpha( pixbuf ); // Dit s'il y a un canal Alpha (transparence).
	int bits_per_sample = gdk_pixbuf_get_bits_per_sample( pixbuf ); // Donne le nombre de bits par echantillon (8 bits souvent).
	guchar* data = gdk_pixbuf_get_pixels( pixbuf ); // Pointeur vers le tampon de donnees
	int width = gdk_pixbuf_get_width( pixbuf ); // Largeur de l'image en pixels
	int height = gdk_pixbuf_get_height( pixbuf ); // Hauteur de l'image en pixels
	int rowstride = gdk_pixbuf_get_rowstride( pixbuf ); // Nombre d'octets entre chaque ligne dans le tampon de donnees
	/*printf( "n_channels = %d\n", n_channels );
	printf( "has_alpha = %d\n", has_alpha );
	printf( "bits_per_sa= %d\n", bits_per_sample );*/
	printf( "(%d,", width*height );
	/*printf( "height = %d\n", height );
	printf( "data = %p\n", data );
	printf( "rowstride = %d\n", rowstride );*/
	Pixel*	pixel = (Pixel*) data;
	//printf( "sizeof(Pixel)=%d\n", sizeof(Pixel) );
	size_t diff = ((guchar*) (pixel+1)) - (guchar*) pixel;
	//printf( "(pixel+1) - pixel=%d\n", diff );
	assert( n_channels == 3 );
	assert( has_alpha == FALSE );
	assert( bits_per_sample == 8 );
	assert( sizeof(Pixel) == 3 );
	assert( diff == 3 );
}

//-----------------------------------------------------------------------------
// Fonctions de l'archive non modifiees
//-----------------------------------------------------------------------------
/// Fonction appelee lorsqu'on clique sur "Input".
gboolean selectInput( GtkWidget *widget, gpointer data )
{
	// Recupere le contexte.
	Contexte* pCtxt = (Contexte*) data;
	// Place le pixbuf a visualiser dans le bon widget.
	gtk_image_set_from_pixbuf( GTK_IMAGE( pCtxt->image ), pCtxt->pixbuf_input );
	// Force le reaffichage du widget.
	gtk_widget_queue_draw( pCtxt->image );
	return TRUE;
}

/// Fonction appelee lorsqu'on clique sur "Output".
gboolean selectOutput( GtkWidget *widget, gpointer data )
{
	// Recupere le contexte.
	Contexte* pCtxt = (Contexte*) data;
	// Place le pixbuf a visualiser dans le bon widget.
	gtk_image_set_from_pixbuf( GTK_IMAGE( pCtxt->image ), pCtxt->pixbuf_output );
	// Force le reaffichage du widget.
	gtk_widget_queue_draw( pCtxt->image );
	return TRUE;
}

/**
	 Cree un image vide de taille width x height.
*/
GdkPixbuf* creerImage( int width, int height )
{
	GdkPixbuf* img = gdk_pixbuf_new(GDK_COLORSPACE_RGB, 0, 8, width, height );
	return img;
}

/**
	 Retourne le niveau de gris du pixel.
*/
unsigned char greyLevel( Pixel* data )
{
	return (data->rouge+data->vert+data->bleu)/3;
}

/**
	 Met le pixel au niveau de gris \a g.
*/
void setGreyLevel( Pixel* data, unsigned char g )
{
	data->rouge = g;
	data->vert = g;
	data->bleu = g;
}

/** 
		Va au pixel de coordonnees (x,y) dans le pixbuf.
*/
Pixel* gotoPixel( GdkPixbuf* pixbuf, int x, int y )
{
	 int rowstride = gdk_pixbuf_get_rowstride( pixbuf );
	 guchar* data	= gdk_pixbuf_get_pixels( pixbuf );
	 return (Pixel*)( data + y*rowstride + x*3 );
}

/**
	 Cree une image sous forme de disque.
*/
void disk( GdkPixbuf* pixbuf, int r )
{
	int x,y;
	int width = gdk_pixbuf_get_width( pixbuf ); // Largeur de l'image en pixels
	int height = gdk_pixbuf_get_height( pixbuf );// Hauteur de l'image en pixels
	int x0 = width/2;
	int y0 = height/2;
	for ( y = 0; y < height; ++y )
		{
			Pixel* pixel = gotoPixel( pixbuf, 0, y ); // les lignes peuvent etre realignees
			for ( x = 0; x < width; ++x )
				{
					int d2 = (x-x0)*(x-x0)+(y-y0)*(y-y0);
					if ( d2 >= r*r ) setGreyLevel( pixel, 0 );
					else setGreyLevel( pixel, 255-(int) sqrt(d2));
					++pixel; // sur une ligne, les pixels se suivent
				}
		}
}
