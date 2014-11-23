#ifndef _GRAYLEVELIMAGE2D_HPP_
#define _GRAYLEVELIMAGE2D_HPP_

#include <iostream>
#include <vector>
#include <exception>

// On utilise les coordonnées affines (abscisse, ordonnées), gche -> drte, haut -> bas :

class GrayLevelImage2D {

public:
  typedef unsigned char GrayLevel;         // le type pour les niveaux de gris.
  typedef std::vector<GrayLevel> Container;// le type pour stocker les niveaux de gris de l'image.

  /**
     Représente un itérateur sur toutes les valeurs d'une image.

     Model of DefaultConstructible, CopyConstructible, Assignable,
     RandomAccessIterator. */
  struct Iterator : public Container::iterator {
    Iterator( GrayLevelImage2D& Image, int x, int y );
  };


public:
  GrayLevelImage2D();
  GrayLevelImage2D( int w, int h, GrayLevel g = 0 );
  void fill( GrayLevel g );

  //! [gli2d-sec3]
  /// @return la largeur de l'image.
  int w() const;
  /// @return la hauteur de l'image.
  int h() const;
  
  //Indexes
  int index(int i, int j) const;

  /**
     Accesseur read-only à la valeur d'un pixel.
     @return la valeur du pixel(i,j)
  */
  GrayLevel at( int i, int j ) const;

  /**
     Accesseur read-write à la valeur d'un pixel.
     @return une référence à la valeur du pixel(i,j)
  */
  GrayLevel& at( int i, int j );
  //! [gli2d-sec3]

  Container getData();
  void setData(Container d);


  GrayLevelImage2D::Iterator begin();
  GrayLevelImage2D::Iterator end();
  GrayLevelImage2D::Iterator start( int x, int y );
  
  std::vector<GrayLevelImage2D::GrayLevel>::iterator sbegin();
  std::vector<GrayLevelImage2D::GrayLevel>::iterator send();
  
  std::pair<int,int> position( Iterator it ) const;
  
  bool importPGM( std::istream & input );
  bool exportPGM( std::ostream & output, bool ascii = true );

  

private:
  int offset( int x, int y ) const;
  Container m_data;
  int m_width;
  int m_height;
};


#endif // #ifndef _GRAYLEVELIMAGE2D_HPP_
