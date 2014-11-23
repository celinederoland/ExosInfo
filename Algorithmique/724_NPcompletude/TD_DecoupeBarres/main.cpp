/* 
 * File:   main.cpp
 * Author: celine
 *
 * Created on 19 novembre 2014, 06:35
 */

#include <iostream>
#include <time.h>
#define NB_ITER 10
#define TAILLE_MAX_REC 23
#define TAILLE_MAX_DYN 200

int decoupe_naif(int n, int prix[], int& nb_appels);
int decoupe_dyn(int n, int prix[]);
int min(int i, int j);
void exec_naif(int prix[]);
void exec_dyn(int prix[]);

/*
 * Quel est le prix d'une découpe optimale d'une barre de longueur n ?
 */
int main(int argc, char** argv) {

    /**
     * Les prix sont fixés via le tableau ``prix``
     *  0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10| 11
     * ---|---|---|---|---|---|---|---|---|---|---|---
     *  0 | 1 | 5 | 7 | 11| 13| 15| 17| 19| 22| 26| 27
     */
    std::cout << "Exercice de découpe de barres" << std::endl;
    int prix[] = {0, 1, 5, 7, 11, 13, 15, 17, 19, 22, 26, 27};
    exec_naif(prix);
    exec_dyn(prix);
    
    return 0;
}

int decoupe_naif(int n, int prix[], int& nb_appels) {
    nb_appels++;
    int prix_opt = 0;
    int m = min(n, 11);
    for (int i = 1; i <= min(n, 11); i++) {
        int tmp = prix[i] + decoupe_naif(n - i, prix, nb_appels);
        if (tmp > prix_opt) {
            prix_opt = tmp;
        }
    }
    return prix_opt;
}

int decoupe_dyn(int n, int prix[]) {
    int solutions[n+1];
    //int prix_opt = 0;
    solutions[0] = 0;
    for (int i = 1; i <= n; i++) {
        solutions[i] = 0;
        int m = min(i, 11);
        for (int j = 1; j <= m; j++) {
            int tmp = prix[j] + solutions[i - j];
            if (tmp > solutions[i]) {
                solutions[i] = tmp;
            }
        }
    }
    return solutions[n];
}

void exec_naif(int prix[]) {
    std::cout << "Avec l'algorithme naif" << std::endl;
    int reponses[TAILLE_MAX_REC + 1];
    int nbappels[TAILLE_MAX_REC + 1];
    double temps[TAILLE_MAX_REC + 1];
    
    int prix_opt = 0;
    int nb_appels = 0;
    int taille;

    for (taille = 0; taille <= TAILLE_MAX_REC; taille++) {
        clock_t avant, apres;
        avant = clock();
        for (int i = 0; i < NB_ITER; ++i) {
            nb_appels = 0;
            prix_opt = decoupe_naif(taille, prix, nb_appels);
        }
        apres = clock();

        double temps_ecoule = (1.0 + apres - avant) / CLOCKS_PER_SEC;
        reponses[taille] = prix_opt;
        nbappels[taille] = nb_appels;
        temps[taille] = (temps_ecoule / NB_ITER)*1000;
        
    }
    
    std::cout << "Réponses : " ;
    for (int i = 0; i <= TAILLE_MAX_REC ; i++) {
        std::cout << reponses[i] << " - ";
    }
    std::cout << std::endl;
    std::cout << "nb_appels : " ;
    for (int i = 0; i <= TAILLE_MAX_REC ; i++) {
        std::cout << nbappels[i] << " - ";
    }
    std::cout << std::endl;
    std::cout << "temps réponse moyen : " ;
    for (int i = 0; i <= TAILLE_MAX_REC ; i++) {
        std::cout << temps[i] << " - ";
    }
    std::cout << std::endl;
}

void exec_dyn(int prix[]) {
    std::cout << std::endl << "Avec l'algorithme dynamique" << std::endl;
    int reponses[TAILLE_MAX_DYN + 1];
    double temps[TAILLE_MAX_DYN + 1];
    
    int prix_opt = 0;
    int taille;

    for (taille = 0; taille <= TAILLE_MAX_DYN; taille++) {
        clock_t avant, apres;
        avant = clock();
        for (int i = 0; i < NB_ITER; ++i) {
            prix_opt = decoupe_dyn(taille, prix);
        }
        apres = clock();

        double temps_ecoule = (1.0 + apres - avant) / CLOCKS_PER_SEC;
        reponses[taille] = prix_opt;
        temps[taille] = (temps_ecoule / NB_ITER)*1000;
        
    }
    
    std::cout << "Réponses : " ;
    for (int i = 0; i <= TAILLE_MAX_DYN ; i++) {
        std::cout << reponses[i] << " - ";
    }
    std::cout << std::endl;
    std::cout << "temps réponse moyen : " ;
    for (int i = 0; i <= TAILLE_MAX_DYN ; i++) {
        std::cout << temps[i] << " - ";
    }
    std::cout << std::endl;
}

int min(int i, int j) {
    if (i < j) {
        return i;
    }
    return j;
}
