/* 
 * File:   main.cpp
 * Author: celine
 *
 * Created on 19 novembre 2014, 10:08
 */

#define TAILLE 200
#define NB_ITER 10

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <limits>
#include <vector>
#include <algorithm>

#include "Point.h"

typedef double (*dmin_func)(const std::vector<Point> &);
typedef bool (*ordrePts)( const Point &, const Point & );

double double_aleat();
void ptsAleat(std::vector<Point> & T, int n);
double distance_min_naif(const std::vector<Point> & T);
double distance_min_dpr(const std::vector<Point> & T);
double distance_min_rec( std::vector<Point> & T, int debut, int fin );
double test_dmin_func(dmin_func f, const std::vector<Point> & T);
void trier_sous_tableau( std::vector<Point> &T, int debut, int fin, ordrePts cmp );
double min(double i, double j);

int main(int argc, char** argv) {
    
    std::vector<Point> T;
    srand(time(NULL));
    ptsAleat(T,TAILLE);
    
    std::cout << "[Algo naif]" << std::endl;
    test_dmin_func(distance_min_naif, T);
    
    std::cout << "[Algo dpr]" << std::endl;
    test_dmin_func(distance_min_dpr, T);

    return 0;
}


double test_dmin_func(dmin_func f, const std::vector<Point> & T) {
    double dmin, temps_ecoule;
    clock_t avant, apres;
    Point::nb_appels_distance = 0; // (ré)initialise le compteur d'appels

    avant = clock();
    for (int i = 0; i < NB_ITER; ++i) {
        dmin = f(T);
    }
    apres = clock();

    temps_ecoule = (1.0 + apres - avant) / CLOCKS_PER_SEC;
    std::cout << "  Distance min : " << dmin << std::endl;
    std::cout << "  Temps de calcul : " << (temps_ecoule / NB_ITER)*1000 << 
            " ms" << std::endl;
    std::cout << "  Nombre d'appels à la fonction distance par itération : " <<
            Point::nb_appels_distance / NB_ITER << std::endl;
    
    return dmin;
}

double distance_min_naif(const std::vector<Point> & T) {
    double dmin = std::numeric_limits<double>::max();
    int n = T.size();

    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            double d = T[i].distance(T[j]);
            if (i != j & d < dmin) {
                dmin = d;
            }
        }
    }
    return dmin;
}

double distance_min_dpr(const std::vector<Point> & T) {
    std::vector<Point> T2 = T;
    trier_sous_tableau(T2,0,T2.size() - 1,Point::cmp_x);
    return distance_min_rec( T2, 0, T2.size()-1 );
}

double distance_min_rec(std::vector<Point> & T, int debut, int fin) {
    
    int n = fin - debut + 1;

    // conditions d'arrêt
    if (n < 2) { return std::numeric_limits<double>::max(); }
    else if (n == 2) { return T[debut].distance(T[debut + 1]); }
    
    // récursivité
    int mid = (debut + fin) / 2;
    double dgauche = distance_min_rec(T,debut, mid );
    double ddroite = distance_min_rec(T,mid + 1, fin );
    double dmin = min(dgauche, ddroite);
    
    
    // vérification de la bande centrale
    double separation = ( T[mid].x + T[mid+1].x ) / 2.0;  
    //points contenus dans la bande
    trier_sous_tableau(T, debut, fin, Point::cmp_x);
    int cmin = debut;
    while (T[cmin].x < separation - dmin) { cmin ++; }
    int cmax = fin;
    while (T[cmax].x > separation + dmin ) { cmax --; }
    //comparaison de chaque point avec les 7 suivants
    trier_sous_tableau(T, cmin, cmax, Point::cmp_y);
    for (int i = cmin; i < cmax + 1; i++) {
        for (int j = i + 1; j < min(i + 7, cmax); j++) {
            double tmp = T[i].distance( T[j] );
            if (tmp < dmin) { dmin = tmp; }
        }
    }
    return dmin;
}

void trier_sous_tableau( std::vector<Point> &T, int debut, int fin, ordrePts cmp ) {
    sort( T.begin()+debut, T.begin()+fin+1, cmp );
}

double double_aleat() {
    return ( 1.0 * rand()) / RAND_MAX;
}

/**
 * Génère un tableau aléatoire de ``n`` points.
 */
void ptsAleat(std::vector<Point> & T, int n) {
    T.clear();
    for (int i = 0; i < n; ++i) {
        double x = double_aleat();
        double y = double_aleat();
        T.push_back(Point(x, y));
    }
}

double min(double i, double j) {
    if (i < j) { return i; }
    return j;
}