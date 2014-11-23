#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//Prototypes
void recopier(char t1[],int i1,char t2[],int i2);
void afficher_tab(char t[], int taille);
void rotation (char t1[], char t2[]);
void afficher_niveau(char t[], int n);
void afficher_parniveau(char t[], int nbniv);
void afficher_joli(char t[],int i);

//Fonctions
void recopier(char t1[],int i1,char t2[],int i2)
{
    if (t1[i1] != '0')
    {
        t2[i2] = t1[i1];
        recopier(t1,2*i1,t2,2*i2);
        recopier(t1,2*i1+1,t2,2*i2+1);
    }
}

void afficher_tab(char t[], int taille)
{
    int i; for (i = 1; i < taille; i++)
    {
        if (t[i] != '0') { putchar(t[i]); }
        else { printf("_"); }
        printf(" ");
    }
    printf("\n");
}

void rotation (char t1[], char t2[])
{
    t2[1] = t1[2];
    t2[3] = t1[1];
    recopier(t1,4,t2,2);
    recopier(t1,5,t2,6);
    recopier(t1,3,t2,7);
}

void afficher_niveau(char t[], int n)
{
    int i; for (i = pow(2,n); i <= pow(2,(n+1)) -1 ; i++)
    {
        if (t[i] != '0') { putchar(t[i]); }
        else { printf("_"); }
        printf(" ");
    }
    printf("\n");
}

void afficher_parniveau(char t[], int nbniv)
{
    int i;
    for (i = 0; i <= nbniv; i++)
    {
        afficher_niveau(t,i);
    }
}

void afficher_joli(char t[],int i)
{
    if (t[2*i+1] != '0') { afficher_joli(t, 2*i+1); }

    int niveau = 0;
    do
    {
        niveau ++;
    }
    while (pow(2,niveau) < i);

    int k; for (k = 0; k < niveau; k++)
    {
        printf("  ___ ");
    }
    putchar(t[i]); printf("\n");

    if (t[2*i] != '0') { afficher_joli(t, 2*i); }
}

int main()
{
    char t1[200] = {'0','I','G','E','Z','R','I','B','X','0','V','J','0','0','P','H','0','0','0','0','0','0','D','0','0','0','0','0','0','0','A','M'};
    int i; for (i = 32; i < 200; i++) { t1[i] = '0';}
    char t2[200];
    for (i = 0; i < 200; i++) { t2[i] = '0';}
    rotation(t1,t2);
    afficher_tab(t1,32);
    afficher_tab(t2,32);
    afficher_joli(t1,1);
    return 0;
}

