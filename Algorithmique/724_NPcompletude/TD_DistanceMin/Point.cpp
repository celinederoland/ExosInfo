/* 
 * File:   Point.cpp
 * Author: celine
 * 
 * Created on 19 novembre 2014, 10:10
 */

#include "Point.h"

int Point::nb_appels_distance = 0;

Point::Point(double x, double y) : x(x), y(y) {
}

double Point::distance(const Point& autre) const {
    ++nb_appels_distance;
    double diff_x = x - autre.x;
    double diff_y = y - autre.y;
    return sqrt(diff_x * diff_x + diff_y * diff_y);
}

bool Point::cmp_x(const Point & p, const Point & q) {
    return p.x < q.x;
}

bool Point::cmp_y(const Point & p, const Point & q) {
    return p.y < q.y;
}

void Point::affiche() {
    std::cout << "(" << x << "," << y << ")";
}