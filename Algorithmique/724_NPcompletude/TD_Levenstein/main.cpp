/* 
 * File:   main.cpp
 * Author: celine
 *
 * Created on 19 novembre 2014, 12:58
 */

#include <cstdlib>
#include <iostream>
#include <iomanip>
#define N 500
#define M 500
#define NB_LETTRES 3
#define NB_ITER 10

using namespace std;

char aleat();
int levenstein(char u[], char v[]);
int min(int x, int y, int z);

int main(int argc, char** argv) {
    int d;
    clock_t avant, apres, t;
    double temps_ecoule;
    
    cout << "Distance de Levenstein" << endl;
    srand(time(NULL));
    t = 0;
    
    char u[N];
    char v[M];
    
    
    for (int i = 0; i < NB_ITER; ++i) {
        cout << " u : ";
        for (int i = 0; i < N; i++) { u[i] = aleat();  } //cout << u[i];
        cout << " v : ";
        for (int i = 0; i < M; i++) { v[i] = aleat();  } //cout << v[i];
        
        avant = clock();
        d = levenstein(u,v);
        apres = clock();
        t += 1.0 + apres - avant;
        cout << " => dist : " << d << endl;
    }

    temps_ecoule = (double) t / CLOCKS_PER_SEC;
    //cout << "  Distance : " << d << endl;
    cout << endl << "  Temps de calcul : " << (temps_ecoule / NB_ITER)*1000 << 
            " ms" << endl;
    
    return 0;
}

int levenstein(char u[], char v[]) {
    /* L[i,j] = distance entre le préfixe de taille i de u 
     * et le préfixe de taille j de v */
    int L[N+1][M+1]; 
    
    //Pour passer d'un mot de 0 lettres à 1 lettre, ça coute 1
    for (int i = 0; i < N + 1; i++) { L[i][0] = 1; }
    for (int i = 0; i < M + 1; i++) { L[0][i] = 1; }
    //Pour passer d'un mot vide à un mot vide, ça coute 0
    L[0][0]=0;
    

    int c1,c2,c3;
    // u = u'a ; v = v'b
    for (int i = 1; i < N + 1; i++) {
        for (int j = 1; j < M + 1; j++) {
            
            //Si on fait un changement sur la dernière lettre
            if (u[i-1] != v[j-1]) { c1 = L[i-1][j-1] + 1; }
            else { c1 = L[i-1][j-1]; }

            //Si on transforme u en v', puis on ajoute b
            c2 = L[i][j-1] + 1;

            // Si on transforme u' en v, puis on enlève a
            c3 = L[i-1][j] + 1;
            
            L[i][j] = min(c1,c2,c3);
        }
    }
    
    /*for (int i = 0; i < N + 1; i++) {
        for (int j = 0; j < M + 1; j++) {
            cout << setw(5) << L[i][j] << " ";
        }
        cout << endl;
    }*/
    
    return L[N][M];
}

char aleat() {
    return rand()%NB_LETTRES + 97;
}

int min(int x, int y, int z) {
    if (x < y and x < z) { return x; }
    if (y < z) { return y; }
    return z;
}