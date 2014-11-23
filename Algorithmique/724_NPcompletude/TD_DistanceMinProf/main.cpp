#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <limits>
#include <algorithm>


/**
 * INFO724
 *
 * Comparaison entre deux méthodes pour calculer la distance minimale entre deux
 * points parmis un ensemble de points.
 *
 * 1. Algorithme naif consistant à calculer la distance entrer toutes les paires
 * de points et à retenir la plus petite.
 *
 * 2. Algorithme ``Diviser pour régner``, tiré de Cormen, Leiserson, Rivest,
 * Stein section 33.4, p.956 )
 *
 */
 
using namespace std;

class Point
{
public :
  Point ( double x, double y ) : x(x), y(y) {}
  ~Point() {}

  double distance( const Point & autre ) const
    {
      ++nb_appels_distance;
      double diff_x = x-autre.x;
      double diff_y = y-autre.y;
      return sqrt( diff_x*diff_x + diff_y*diff_y );
    }

  static bool cmp_x( const Point &  p, const Point & q )
    {
      return p.x < q.x;
    }

  static bool cmp_y( const Point &  p, const Point & q )
    {
      return p.y < q.y;
    }

  friend ostream & operator<<( ostream& os, const Point &p );

public :
  double x;
  double y;
  static int nb_appels_distance;
};

int Point::nb_appels_distance = 0;

ostream & operator<<( ostream& os, const Point &p )
{
  os << "("  << p.x << "," << p.y << ")";
  return os;
}


/**
 * Génère un nombre aléatoire dans l'intervale [0, 1].
 */
double double_aleat( )
{
  //return static_cast<double>( rand() ) / static_cast<double>( RAND_MAX );
  return ( 1.0 * rand() ) / RAND_MAX;
}


/**
 * Génère un tableau aléatoire de ``n`` points.
 * Params : 
 *  - T - ( sortie ) tableau dans lequel les points sont inscrits,
 *  - n - le nombre de points à générer.
 */
void ptsAleat( vector<Point> & T,  int n ) 
{
  T.clear();
  for ( int i=0; i<n; ++i )
    {
      double x = double_aleat();
      double y = double_aleat();
      T.push_back( Point( x, y)  );
    }
}


double distance_min_naif( const vector<Point> & T )
{
  double dmin = numeric_limits<double>::max();
  int n = T.size();
  for ( int i=0; i<n-1; ++i )
    {
      for ( int j=i+1; j<n; ++j )
        {
          double tmp = T[i].distance( T[j] );
          if ( tmp < dmin ) 
            {
              dmin = tmp;
            }
        }
    }
  return dmin;
}



typedef bool (*ordrePts)( const Point &, const Point & );
void trier_sous_tableau( vector<Point> &T, int debut, int fin, ordrePts cmp )
{
  sort( T.begin()+debut, T.begin()+fin+1, cmp );
}


double distance_min_rec( vector<Point> & T, int debut, int fin )
{
  int n = fin-debut+1;

  // cas de base
  if ( n < 2 ) 
    return numeric_limits<double>::max();
  else if ( n == 2 ) 
    return T[debut].distance( T[debut+1] );

  int mid = (debut+fin)/2;
  double split_x = ( T[mid].x + T[mid+1].x ) / 2.0;

  // récursivité
  double d_g = distance_min_rec( T, debut, mid );
  double d_d = distance_min_rec( T, mid+1, fin );

  double dmin = min( d_g, d_d );

  int c_min = debut;
  int c_max = fin;
  trier_sous_tableau( T, debut, fin, Point::cmp_x );
  while ( T[c_min].x < split_x - dmin )
    {
      ++c_min;
    }
  while ( T[c_max].x > split_x + dmin )
    {
      --c_max;
    }
  trier_sous_tableau( T, c_min, c_max, Point::cmp_y );

  for (int i=c_min; i<=c_max; ++i )
    {
      for ( int j=i+1; j <= min( i+7, c_max ); ++j )
        {
          double tmp = T[i].distance( T[j] );
          if ( tmp < dmin ) 
            {
              dmin = tmp;
            }
        }
    }
  return dmin;
}





double distance_min_dpr( const vector<Point> & T )
{
  vector<Point> T2 = T;
  trier_sous_tableau( T2, 0, T2.size()-1, Point::cmp_x );
  return distance_min_rec( T2, 0, T2.size()-1 );
}




typedef double (*dmin_func)( const vector<Point> &);
double test_dmin_func( dmin_func f, const vector<Point> & T, int nb_iter )
{
  double dmin, temps_ecoule;
  clock_t avant, apres;
  Point::nb_appels_distance = 0;  // (ré)initialise le compteur d'appels
  
  avant = clock();
  for ( int i=0; i<nb_iter; ++i )
    {
      dmin = f( T );
    }
  apres = clock();

  temps_ecoule = ( 1.0 + apres - avant ) / CLOCKS_PER_SEC;
  cout << "  Distance min : " << dmin << "\n";
  cout << "  Temps de calcul : " << temps_ecoule / nb_iter << " sec./itération\n";
  cout << "  Nombre d'appels à la fonction distance par itération : " <<
    Point::nb_appels_distance / nb_iter << endl;
  return dmin;
}





void usage( const char commande[] )
{
  cout << "Usage : " << commande << " taille [nb_iter]\n";
  cout << "Génère des points aléatoirement puis calcul la distance minimum entre deux points via deux algorithmes, un premier naif et un second utilisant le principe ``diviser pour régner``.\n";
  cout << "  - taille -  nombre de points générés\n";
  cout << "  - nb_iter - nombre de fois que le calcul est répété ( optionnel, valeur\n";
  cout << "              par défaut : 1 )" << endl;
}

int main( int argc, const char* argv[] )
{
  int taille = 200, nb_iter = 10;
  vector<Point> T;

  /*if ( argc < 2 ) 
    {
      usage( argv[0] );
      exit( EXIT_FAILURE );
    }
  taille = atoi( argv[1] );
  nb_iter = 1;
  if ( argc >= 3 )
      nb_iter = atoi( argv[2] );*/


  srand (time(NULL));
  ptsAleat( T, taille );

  cout << "[Algo naif]\n"; 
  test_dmin_func( distance_min_naif, T, nb_iter );

  cout << "[Algo DPR]\n"; 
  test_dmin_func( distance_min_dpr, T, nb_iter );
  return 0;
}



