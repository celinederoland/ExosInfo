#include <stdio.h>
#include <stdlib.h>

/*La structure Arbre possède une valeur et 3 pointeurs vers les arbres parent et enfants, plus un compteur de décalage et un compteur de taille*/
typedef struct SArbre Arbre;
struct SArbre
{
     int valeur;
     Arbre *gauche;
     Arbre *droit;
     Arbre *parent;
     signed int decalage;
     int hauteur;
};

/*Fonctions à développer*/
void afficher_tableau(int t[], int taille);
Arbre *creer_arbre(int val);
void ajout(Arbre *a, int val, int cote); //g=1; d=-1
int maxi(int a, int b);
void calcul_hauteur(Arbre *a);
void afficher_noeud(Arbre *a); //(val (decalage)
void afficher_joli(Arbre *a, int p);
void prefixe(Arbre *a);
void infixe(Arbre *a);
void suffixe(Arbre *a);
void sym(Arbre *a);
int contenu(Arbre *a, Arbre *motif);
Arbre *creer_trie(int t[], int taille);
void placer_trie(Arbre *a, int val);
Arbre *creer_AVL(int t[], int taille);
void placer_AVL(Arbre *a, int val);
void equilibrer_AVL(Arbre *a);
void rotation(Arbre *a);
void rd(Arbre *a);
void rg(Arbre *a);
void rgd(Arbre *a);
void rdg(Arbre *a);









int main()
{

    int t[] = {18,98,51,70,62,13,29,33,57,61,35,15,3};

    afficher_tableau(t,13);
    Arbre *a = creer_AVL(t,13);
    afficher_joli(a,0); printf("\n\n");

    Arbre *motif = creer_arbre(18);
     ajout(motif,33,-1);
     ajout(motif -> droit,29,1);
     ajout(motif -> droit,35,-1);
     afficher_joli(motif,0);
     printf("\n");
     printf("\nContenu ? %d", contenu(a,motif));

    return 0;
}

void afficher_tableau(int t[], int taille)
{
     int i;
     for (i = 0; i < taille; i++)
     {
          printf("%d ; ", t[i]);
     }
     printf("\n");
}

Arbre *creer_arbre(int val)
{
     Arbre *a = malloc(sizeof(*a));

     a -> parent = NULL;
     a -> gauche = NULL;
     a -> droit = NULL;
     a -> valeur = val;
     a -> hauteur = 1;
     a -> decalage = 0;

     return a;
}






void ajout(Arbre *a, int val, int cote)
{
     Arbre *fils = creer_arbre(val);

     fils -> parent = a;

     if (cote == 1) {
          a -> gauche = fils;
     }
     else {
          a -> droit = fils;
     }
     calcul_hauteur(a);
}

int maxi(int a, int b)
{
     if (a >= b) { return a; }
     else { return b; }
}

void calcul_hauteur(Arbre *a)
{
     int hg,hd;
     if (a -> gauche != NULL) { hg = a -> gauche -> hauteur; }
     else { hg = 0; }
     if (a -> droit != NULL) { hg = a -> droit -> hauteur; }
     else { hd = 0; }
     a -> hauteur = maxi(hg,hd) + 1;
     if (a -> parent != NULL)
     {
          calcul_hauteur(a -> parent);
     }
}

void afficher_noeud(Arbre *a)
{
     printf("%d", a -> valeur);
}










void afficher_joli(Arbre *a, int p)
{
     if (a -> droit != NULL)
     {
          afficher_joli(a -> droit, p + 1);
     }

     int i;
     for (i = 0; i < p; i++)
     {
          printf("  __ ");
     }
     afficher_noeud(a);
     printf("\n");

     if (a -> gauche != NULL)
     {
          afficher_joli(a -> gauche, p + 1);
     }
}

void prefixe(Arbre *a)
{
     afficher_noeud(a);printf("  ");
     if (a -> gauche != NULL) { prefixe(a -> gauche); }
     if (a -> droit != NULL) { prefixe(a -> droit); }
}

void infixe(Arbre *a)
{
     if (a -> gauche != NULL) { infixe(a -> gauche); }
     afficher_noeud(a); printf("  ");
     if (a -> droit != NULL) { infixe(a -> droit); }
}

void suffixe(Arbre *a)
{
     if (a -> gauche != NULL) { suffixe(a -> gauche); }
     if (a -> droit != NULL) { suffixe(a -> droit); }
     afficher_noeud(a); printf("  ");
}








void sym(Arbre *a)
{
     if (a -> gauche != NULL)
     {
          sym(a -> gauche);
     }
     if (a -> droit != NULL)
     {
          sym(a -> droit);
     }
     Arbre *ndrt = a -> droit;
     a -> droit = a -> gauche;
     a -> gauche = ndrt;
}

Arbre *creer_trie(int t[], int taille)
{
     Arbre *a = creer_arbre(t[0]);
     int i;
     for (i = 1; i < taille; i++)
     {
          placer_trie(a,t[i]);
     }
     return a;
}

void placer_trie(Arbre *a, int val)
{
     if (val < a -> valeur)
     {
          if (a -> gauche == NULL) { ajout(a,val,1); }
          else { placer_trie(a -> gauche, val); }
     }
     else
     {
          if (a -> droit == NULL) { ajout(a,val,-1); }
          else { placer_trie(a -> droit, val); }
     }
}










int contenu(Arbre *a, Arbre *motif)
{
     int I = 0; int J = 0;

     if (a -> valeur == motif -> valeur)
     {
          if (motif -> gauche != NULL)
          {
               I = contenu(a -> gauche, motif -> gauche);
          }
          else
          {
               I = 1;
          }
          if (motif -> droit != NULL)
          {
               J = contenu(a -> droit, motif -> droit);
          }
          else
          {
               J = 1;
          }
          return (I*J);
     }
     else
     {
          if (a -> gauche != NULL)
          {
               I = contenu(a -> gauche, motif);
          }
          else
          {
               I = 0;
          }
          if (a -> droit != NULL)
          {
               J = contenu(a -> droit, motif);
          }
          else
          {
               J = 0;
          }
          return (I + J - I*J);
     }
     return 0;
}



Arbre *creer_AVL(int t[], int taille)
{
     Arbre *a = creer_arbre(t[0]);
     int i;
     for (i = 1; i < taille; i++)
     {
          placer_AVL(a,t[i]);
     }
     return a;
}

void placer_AVL(Arbre *a, int val)
{
     if (val < a -> valeur)
     {
          if (a -> gauche == NULL) {
               ajout(a,val,1);
               equilibrer_AVL(a);
          }
          else { placer_AVL(a -> gauche, val); }
     }
     else
     {
          if (a -> droit == NULL) {
               ajout(a,val,-1);
               equilibrer_AVL(a);
          }
          else { placer_AVL(a -> droit, val); }
     }
}

void equilibrer_AVL(Arbre *a)
{
     int hg,hd,decalage;
     if (a -> gauche == NULL) { hg = 0;}
     else { hg = a -> gauche -> hauteur; }
     if (a -> droit == NULL) { hd = 0; }
     else { hd = a -> droit -> hauteur;}
     decalage = hg - hd;
     if (decalage > 1 || decalage < -1) {
          rotation(a);
     }
     else
     {
          if (a -> parent != NULL) {
               equilibrer_AVL(a -> parent);
          }
     }
}

void rotation(Arbre *a)
{
     if (a -> droit != NULL && a -> droit -> droit != NULL)
     {
          rg(a);
     }
     else if (a -> gauche != NULL && a -> gauche -> gauche != NULL)
     {
          rd(a);
     }
     else if (a -> gauche != NULL && a -> gauche -> droit != NULL)
     {
          rgd(a);
     }
     else if (a -> droit != NULL && a -> droit -> gauche != NULL)
     {
          rdg(a);
     }
}

void rd(Arbre *q)
{
     Arbre *U = q -> gauche -> gauche;
     Arbre *V = q -> gauche -> droit;
     Arbre *W = q -> droit;

     int valp = q -> gauche -> valeur;
     int valq = q -> valeur;

     q -> valeur = valp;
     Arbre *d = creer_arbre(valq);

     q -> gauche = U; if (U != NULL) { U -> parent = q; }
     q -> droit = d;

     d -> parent = q;
     d -> gauche = V; if (V != NULL) { V -> parent = d; }
     d -> droit = W; if (W != NULL) { W -> parent = d; }
     calcul_hauteur(d);
}








void rg(Arbre *q)
{
     Arbre *U = q -> gauche;
     Arbre *V = q -> droit -> gauche;
     Arbre *W = q -> droit -> droit;

     int valp = q -> droit -> valeur;
     int valq = q -> valeur;

     q -> valeur = valp;
     Arbre *g = creer_arbre(valq);

     q -> droit = W; if (W != NULL) { W -> parent = q; }
     q -> gauche = g;

     g -> parent = q;
     g -> gauche = U; if (U != NULL) { U -> parent = g; }
     g -> droit = V; if (V != NULL) { W -> parent = g; }
     calcul_hauteur(g);
}

void rgd(Arbre *r)
{
     int valr, valq, valp;
     valr = r -> valeur;
     valq = r -> gauche -> valeur;
     valp = r -> gauche -> droit -> valeur;
     Arbre *T = r -> gauche ->gauche;
     Arbre *W = r -> droit;
     Arbre *U = r -> gauche -> droit -> gauche;
     Arbre *V = r -> gauche -> droit -> droit;
     Arbre *d = creer_arbre(valr);
     Arbre *q = creer_arbre(valq);
     r -> valeur = valp;
     r -> gauche = q; q -> parent = r;
     r -> droit = d; d -> parent = r;
     q -> gauche = T; if (T != NULL) { T -> parent = q; }
     q -> droit = U; if (U != NULL) { U -> parent = q; }
     int hg,hd;
     if (q -> gauche != NULL) { hg = q -> gauche -> hauteur; }
     else { hg = 0; }
     if (q -> droit != NULL) { hg = q -> droit -> hauteur; }
     else { hd = 0; }
     q -> hauteur = maxi(hg,hd) + 1;

     d -> gauche = V; if (V != NULL) { V -> parent = d; }
     d -> droit = W; if (W != NULL) { W -> parent = d; }
     calcul_hauteur(d);
}

void rdg(Arbre *r)
{
     int valr, valq, valp;
     valr = r -> valeur;
     valq = r -> droit -> valeur;
     valp = r -> droit -> gauche -> valeur;
     Arbre *T = r -> gauche;
     Arbre *W = r -> droit -> droit;
     Arbre *U = r -> droit -> gauche -> gauche;
     Arbre *V = r -> droit -> gauche -> droit;
     Arbre *g = creer_arbre(valr);
     Arbre *q = creer_arbre(valq);
     r -> valeur = valp;
     r -> gauche = g; g -> parent = r;
     r -> droit = q; q -> parent = r;
     g -> gauche = T; if (T != NULL) { T -> parent = g; }
     g -> droit = U; if (U != NULL) { U -> parent = g; }
     int hg,hd;
     if (g -> gauche != NULL) { hg = g -> gauche -> hauteur; }
     else { hg = 0; }
     if (g -> droit != NULL) { hg = g -> droit -> hauteur; }
     else { hd = 0; }
     g -> hauteur = maxi(hg,hd) + 1;

     q -> gauche = V; if (V != NULL) { V -> parent = q; }
     q -> droit = W; if (W != NULL) { W -> parent = q; }
     calcul_hauteur(q);

}
