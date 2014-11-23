#include <iostream>
#include <fstream>
#include "GrayLevelImage2D.hpp"

using namespace std;

int main( int argc, char** argv )
{
  /**
   * Section 3
   */
  {
    //! [tgli2d-sec3]
    GrayLevelImage2D img( 8, 8, 5 ); // imagette 8x8 remplie de 5
    
    std::cout << "Image de départ affichée en ligne :" << std::endl;
    for ( int y = 0; y < img.h(); ++y )
        for ( int x = 0; x < img.w(); ++x )
            std::cout << " " << (int) img.at( x, y ); // la conversion permet de voir les caractères sous forme d'entiers.
    std::cout << std::endl << std::endl;

    std::cout << "Image modifiée affichée en matrice (on affiche seulement les valeurs modifiées) :" << std::endl;
    img.at(2,6) = 8; // colonne 2, ligne 6
    img.at(0,5) = 9;
    img.at(1,1) = 7;
    img.at(1,0) = 2;
    std::cout << "  ";
    for ( int x = 0; x < img.w(); ++x ) {
        std::cout << " " << x;
    }
    std::cout << std::endl;

    for ( int y = 0; y < img.h(); ++y ) {
        std::cout << y << ":";
        for ( int x = 0; x < img.w(); ++x ) {
            if ((int) img.at(x,y) != 5) {
                std::cout << " " << (int) img.at( x, y ); // la conversion permet de voir les caractères sous forme d'entiers.
            }
            else {
                std::cout << "  "; // la conversion permet de voir les caractères sous forme d'entiers.
            }
        }
    std::cout << std::endl;
    }
    //! [tgli2d-sec3]
  }
  /**
   * Section, 4
   */
  {
    GrayLevelImage2D img( 8, 8, 5 ); // imagette 8x8 remplie de 5
    std::cout << "Utilisation de l'itérateur pour afficher l'image en ligne :" << std::endl;
    
    std::cout << "Avec getData : ça fonctionne" << std::endl;
    GrayLevelImage2D::Container d = img.getData();
    for (std::vector<GrayLevelImage2D::GrayLevel>::iterator it = d.begin(); it != d.end(); ++it) {
         std::cout << " - " << (int) *it;
    }
    std::cout << std::endl;

    std::cout << "Avec sbegin et send : ça fonctionne " << std::endl;
    for ( std::vector<GrayLevelImage2D::GrayLevel>::iterator it = img.sbegin(), itE = img.send(); it != itE; ++it )
	std::cout << " - " << (int) *it; 
    std::cout << std::endl;

    std::cout << "Avec constructeur de classe mère : ça fonctionne" << std::endl;
    std::vector<GrayLevelImage2D::GrayLevel>::iterator itM = std::vector<GrayLevelImage2D::GrayLevel>::iterator(img.getData().begin());
    std::vector<GrayLevelImage2D::GrayLevel>::iterator itFin = std::vector<GrayLevelImage2D::GrayLevel>::iterator(img.getData().end());
    for ( ; itM != itFin; ++itM )
	std::cout << " - " << (int) *itM; 
    std::cout << std::endl;
            
    std::cout << "Avec un itérateur créé via notre constructeur : ça fonctionne" << std::endl;
    GrayLevelImage2D::Iterator itC(img,0,0);
    std::cout << "Début : " << (int) *itC << " ... "; 
    for ( int i = 0; i < 5;++i, ++itC )
	std::cout << " - " << (int) *itC; 
    std::cout << std::endl;
    
    //! [tgli2d-sec4]
    std::cout << "Test du prof : ça fonctionne" << std::endl;
    for ( GrayLevelImage2D::Iterator it = img.begin(), itE = img.end(); it != itE; ++it )
	std::cout << " - " << (int) *it; // la conversion permet de voir les caractères sous forme d'entiers.*/
    //! [tgli2d-sec4]
  }

  /*
  typedef GrayLevelImage2D::GrayLevel GrayLevel;
  typedef GrayLevelImage2D::Iterator Iterator;
  GrayLevelImage2D img;
  ifstream input( argv[1] );
  try {
    img.importPGM( input );
  }
  catch ( char const * msg ) {
    std::cerr << "Exception: " << msg << std::endl;
  }
  catch (...) {
    std::cerr << "Exception." << std::endl;
  }
  input.close();
  for ( Iterator it = img.begin(), itE = img.end(); it != itE; ++it )
    {
      *it = (13*((int)(*it))) % 256;
    }
  ofstream output( argv[2] );
  img.exportPGM( output, false );
  output.close();
  std::cout << std::endl;
  return 0;*/
}
