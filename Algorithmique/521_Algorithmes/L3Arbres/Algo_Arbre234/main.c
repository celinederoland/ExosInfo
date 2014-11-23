#include <stdio.h>
#include <stdlib.h>
#include <time.h>
typedef struct SArbre Arbre;
struct SArbre
{
     int val[3];
     Arbre *fils[4];
     int nbElt;
};

Arbre *creer(int val);
void ajouter(Arbre *a, int val);
void tri_tableau(int t[], Arbre *a, int *i);
void afficher(Arbre *a, int p);
void afficher_noeud(Arbre *a);
void eclater(Arbre *n, int e);
void ajouter_tri(Arbre *a, int val);
Arbre *creer234(int t[], int taille);
void trier(int t[]);
int mini(int a, int b);
int mini3(int a, int b, int c);
int maxi(int a, int b);
int maxi3(int a, int b, int c);

int main()
{
     int t[200];
     int cpt = 0;
     srand(time(NULL)); // initialisation de rand
     int i; for ( i = 0; i < 200; i++)
     {
          t[i] = rand()%998;
     }
     Arbre *a = creer234(t,200);
     tri_tableau(t,a,&cpt);
     for ( i = 0; i < 200; i++)
     {
          printf("%d ; ",t[i]);
     }
     printf("\n\n");
     afficher(a,0);
     printf("\n\nfin");
     return 0;
}




void tri_tableau(int t[], Arbre *a, int *i)
{
    if (a != NULL)
    {
        tri_tableau(t, a -> fils[3], i);
        if (a -> nbElt == 3) { t[199-*i] = a -> val[2]; (*i)++; }
        tri_tableau(t, a -> fils[2], i);
        if (a -> nbElt >= 2) { t[199-*i] = a -> val[1]; (*i)++; }
        tri_tableau(t, a -> fils[1], i);
        if (a -> nbElt >= 1) { t[199-*i] = a -> val[0]; (*i)++; }
        tri_tableau(t, a -> fils[0], i);
    }
}

Arbre *creer(int val)
{
     Arbre *a = malloc(sizeof(*a));

     a -> val[0] = val;
     a -> nbElt = 1;
     int i; for (i = 0; i < 4; i++) { a -> fils[i] = NULL; }
     for (i = 1; i < 3; i++) { a -> val[i] = -1; }
     return a;
}

void ajouter(Arbre *a, int val)
{
     int i;
     if (val < a -> val[0])
     {
          for (i = a -> nbElt; i > 0; i--)
          {
               a -> val[i] = a -> val[i-1];
          }
          a -> val[0] = val;
     }
     else if (a -> nbElt == 1 || val < a -> val[1])
     {
          for (i = a -> nbElt; i > 1; i--)
          {
               a -> val[i] = a -> val[i-1];
          }
          a -> val[1] = val;
     }





     else
     {
          for (i = a -> nbElt; i > 2; i--)
          {
               a -> val[i] = a -> val[i-1];
          }
          a -> val[2] = val;
     }
     a -> nbElt = (a -> nbElt) + 1;
}

void afficher_noeud(Arbre *a)
{
     int i;
     for (i = 0; i < a -> nbElt; i++)
     {
          printf("| %d ", a -> val[i]);
     }
     printf("|");
}

void afficher(Arbre *a, int p)
{
     int i;
     if (a != NULL)
     {
          afficher(a -> fils[3], p+1);
          if (a -> nbElt == 3) {
               for (i = 0; i < p; i++) { printf("     ");}
               printf("%d\n",a -> val[2]);
          }
          afficher(a -> fils[2], p+1);
          if (a -> nbElt >= 2) {
               for (i = 0; i < p; i++) { printf("     ");}
               printf("%d\n",a -> val[1]);
          }
          afficher(a -> fils[1], p+1);
          if (a -> nbElt >= 1) {
               for (i = 0; i < p; i++) { printf("     ");}
               printf("%d\n",a -> val[0]);
          }
          afficher(a -> fils[0], p+1);
     }
}





void eclater_racine(Arbre *n)
{
     int a = n -> val[0];
     int b = n -> val[1];
     int c = n -> val[2];

     Arbre *A = n -> fils[0];
     Arbre *B = n -> fils[1];
     Arbre *C = n -> fils[2];
     Arbre *D = n -> fils[3];

     Arbre *g = creer(a);
     g -> fils[0] = A;
     g -> fils[1] = B;
     Arbre *d = creer(c);
     d -> fils[0] = C;
     d -> fils[1] = D;

     n -> val[0] = b;
     n -> nbElt = 1;
     n -> fils[0] = g;
     n -> fils[1] = d;
     n -> fils[2] = NULL;
     n -> fils[3] = NULL;
}

void eclater(Arbre *n, int e)
{
     Arbre *g = creer(n -> fils[e] -> val[0]);
     g -> fils[0] = n -> fils[e] -> fils[0];
     g -> fils[1] = n -> fils[e] -> fils[1];
     g -> nbElt = 1;
     Arbre *d = creer(n -> fils[e] -> val[2]);
     d -> fils[0] = n -> fils[e] -> fils[2];
     d -> fils[1] = n -> fils[e] -> fils[3];
     d -> nbElt = 1;
     if (n -> nbElt == 1)
     {
          if (e == 0)
          {
               n -> val[1] = n -> val[0];
               n -> val[0] = n -> fils[e] -> val[1];
               Arbre *C = n -> fils[1];
               n -> fils[0] = g;
               n -> fils[1] = d;
               n -> fils[2] = C;
          }
          



					else
          {
               n -> val[1] = n -> fils[e] -> val[1];
               n -> fils[1] = g;
               n -> fils[2] = d;
          }
     }
     else
     {
          if (e == 0)
          {
               n -> val[2] = n -> val[1];
               n -> val[1] = n -> val[0];
               n -> val[0] = n -> fils[e] -> val[1];
               n -> fils[3] = n -> fils[2];
               n -> fils[2] = n -> fils[1];
               n -> fils[0] = g;
               n -> fils[1] = d;
          }
          else if (e == 1)
          {
               n -> val[2] = n -> val[1];
               n -> val[1] = n -> fils[e] -> val[1];
               n -> fils[3] = n -> fils[2];
               n -> fils[1] = g;
               n -> fils[2] = d;
          }
          else
          {
               n -> val[2] = n -> fils[e] -> val[1];
               n -> fils[2] = g;
               n -> fils[3] = d;
          }
     }
     n -> nbElt = (n -> nbElt) + 1;
}











void ajouter_tri(Arbre *a, int val)
{
     if (a -> fils[0] == NULL )
     {
          ajouter(a,val);
     }
     else
     {
          if (val < a -> val[0])
          {
               if (a -> fils[0] -> nbElt == 3) {
                    eclater(a,0);
                    ajouter_tri(a,val);
               }
               else { ajouter_tri(a -> fils[0], val);}
          }
          else if (a -> nbElt == 1 || val < a -> val[1])
          {
               if (a -> fils[1] -> nbElt == 3) {
                    eclater(a,1);
                    ajouter_tri(a,val);
               }
               else { ajouter_tri(a -> fils[1],val); }
          }
          else if (a -> nbElt == 2 || val < a -> val[2])
          {
               if (a -> fils[2] -> nbElt == 3) {
                    eclater(a,2);
                    ajouter_tri(a,val);
               }
               else { ajouter_tri(a -> fils[2],val); }
          }
          else
          {
               if (a -> fils[3] -> nbElt == 3) {
                    eclater(a,3);
                    ajouter_tri(a,val);
               }
               else { ajouter_tri(a -> fils[3],val); }
          }
     }
}







Arbre *creer234(int t[], int taille)
{
     trier(t);
     Arbre *a = creer(t[1]);
     Arbre *g = creer(t[0]);
     Arbre *d = creer(t[2]);
     a -> fils[0] = g;
     a -> fils[1] = d;
     int i; for (i = 3; i < taille; i++) {
          if (a -> nbElt == 3) { eclater_racine(a); }
          ajouter_tri(a,t[i]);
     }
     return a;
}

void trier(int t[])
{
     int a = t[0]; int b = t[1]; int c = t[2];
     t[0] = mini3(a,b,c);
     t[2] = maxi3(a,b,c);
     t[1] = a + b + c - t[0] - t[2];
}

int mini(int a, int b)
{
     if (a < b) { return a;}
     else {return b;}
}
int mini3(int a, int b, int c)
{
     return (mini(a,mini(b,c)));
}

int maxi(int a, int b)
{
     if (a < b) { return b;}
     else {return a;}
}
int maxi3(int a, int b, int c)
{
     return (maxi(a,maxi(b,c)));
}
