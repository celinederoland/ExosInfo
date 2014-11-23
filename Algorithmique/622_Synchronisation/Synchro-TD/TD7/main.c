#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

typedef struct
{
	int *t;
	int debut;
	int fin;
	int z;
	int (*m)(int);
	int (*f)(int,int);
	int res;
} data;


int id(int x) { return x; }
int estPair(int x) { return (1 - (x%2)); }
int sum(int x, int y) { return x+y; }
int max2(int x, int y) { return (x>y)?x:y; }
int deuxieme(int x, int y) { return y; }

int mapAndFold(int *t, int n, int z, int (*m)(int), int (*f)(int, int))
{
	int i; int res=z;
	for (i = 0; i < n; ++i) { res = f( res,m( t[i] ) ); }
	return res;
}

int mapAndFoldRec(int *t, int n, int z, int (*m)(int), int (*f)(int, int))
{
	data d; 
	d.t = t;
	d.debut = 0;
	d.fin = n;
	d.z = z;
	d.m = m;
	d.f = f;
	mapAndFoldRec2((void*)&d);
	return d.res;
}

void mapAndFoldRec2(void* d)
{
	d = (data*)d;
	if (d -> fin - d -> debut == 0) { d -> res = d -> z; }
	else if (d -> fin - d -> debut == 1) { d -> res = d -> f(d -> z,d -> m(d -> t[d -> debut])); }
	else 
	{
		data gauche,droite;
		pthread_t th;

		gauche = *d;
		gauche.fin = (d -> debut + d -> fin)/2;
		gauche.z = d -> res;
		pthread_create(&th,NULL,(void *(*)(void *))mapAndFoldRec2,(void*)&gauche);

		droite = *d;
		droite.debut = (d -> debut + d -> fin)/2;
		mapAndFoldRec2(&gauche);
		pthread_join(th, NULL);
		d -> res = d -> f(gauche.res,droite.res);
	}
}

int somme(int *t, int n) { return mapAndFold(t,n,0,id,sum); }
int max(int *t, int n) { return mapAndFold(t,n,0,id,max2); }
int dernier(int *t, int n) { return mapAndFold(t,n,0,id,deuxieme); }
int pairs(int *t, int n) { return mapAndFold(t,n,0,estPair,sum); }

int sommeRec(int *t, int n) { return mapAndFoldRec(t,n,0,id,sum); }
int maxRec(int *t, int n) { return mapAndFoldRec(t,n,0,id,max2); }
int dernierRec(int *t, int n) { return mapAndFoldRec(t,n,0,id,deuxieme); }
int pairsRec(int *t, int n) { return mapAndFoldRec(t,n,0,estPair,sum); }


int main()
{
	int t[4];
	t[0] = 12; t[1] = 3; t[2] = -5; t[3] = 10;
	printf("résultat de la somme : %d\n",somme(t,4));
	printf("résultat du max : %d\n",max(t,4));
	printf("résultat du dernier : %d\n",dernier(t,4));
	printf("résultat nombre de nombres pairs : %d\n",pairs(t,4));
	return 0;
}
