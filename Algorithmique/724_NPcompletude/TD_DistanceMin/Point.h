/* 
 * File:   Point.h
 * Author: celine
 *
 * Created on 19 novembre 2014, 10:10
 */

#include <iostream>
#include <math.h>

#ifndef POINT_H
#define	POINT_H


class Point {
public:
    double x;
    double y;
    static int nb_appels_distance;
    
    Point(double x, double y);
    ~Point() {}
    double distance( const Point & autre ) const;
    static bool cmp_x( const Point &  p, const Point & q );
    static bool cmp_y( const Point &  p, const Point & q );

    void affiche();
private:

};

#endif	/* POINT_H */

