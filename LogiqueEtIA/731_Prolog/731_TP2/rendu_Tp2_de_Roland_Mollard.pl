/*
	info 730 Prolog
	TP2
	Rémi Mollard
	Céline de Roland
*/

/* Connections existantes dans le réseau de portes */
connection(a,b).
connection(a,c).
connection(b,c).
connection(c,a).
connection(c,z).

/* Voyages déjà programmés */
tripFixe(345,[a,b,wait(2),c,wait(1),z]).
tripFixe(346,[a, wait(3), b, c, z]).

/**************/
/* Question 1 */
/**************/

/* On cherche un chemin possible entre deux portes */

	/* On ne fait pas de voyage d'un point X vers le même point X */
	trip(X,X,_):-
		fail.

	/* On utilise la fonction chemin du TP précédent */
	trip(X,Y,Zs):-
		X \= Y,
		chemin(X,Y,Zs).

	/* Fonction chemin : */

		/* Utilitaire : fonction pour inverser une liste */
		retourne([],[]).
		retourne([X|R],L1) :- retourne(R,R1),append(R1,[X],L1).

		/* Utilitaire : fonction pour voir si un sommet a déjà été visité */
		pasVisite(_,[]).
		pasVisite(X,[Y|Ys]):-
			X\=Y,
			pasVisite(X,Ys).

		/* Appel à une autre fonction chemin prenant un paramètre supplémentaire */
		chemin(X,Y,L):-
			cheminBoucle(X,Y,[X],L).

		cheminBoucle(X,Y,L,R):-
			append(L,[Y],R),
			connection(X,Y).

		cheminBoucle(X,Y,M,L):-
			connection(X,Z),
			pasVisite(Z,M),
			append(M,[Z],R),
			cheminBoucle(Z,Y,R,L).


/**************/
/* Question 2 */
/**************/

/* Recherche des heures d'arrivée et de départ à chaque porte pour tous les voyages programmés */

	/* Pour chaque voyage caractérisé par son heure d'arrivée et son chemin, on calcule les heures d'arrivée et de départ */
	temps(A,D) :- tripFixe(H,C), tempsCalc(H,C,A,D).

	/* Pour deux sommets qui se suivent :
		 l'heure d'arrivée et de départ sont les même pour le premier sommet 
		 l'horaire d'arrivée au prochain sommet est une heure plus tard
	*/
	tempsCalc(H,[X,Y|C],[(X,H)|A],[(X,H)|D]) :- 
		X \= wait(_), 
		Y \= wait(_), 
		H1 is H + 1, 
		tempsCalc(H1,[Y|C],A,D).

	/* Pour un sommet suivi d'un wait(N) :
		 on arrive à l'heure H sur le sommet
		 on part du sommet à H + N
		 l'horaire d'arrivée au prochain sommet sera donc H + N + 1
	*/
	tempsCalc(H,[X,wait(N)|C],[(X,H)|A],[(X,Hc)|D]) :- 
		X \= wait(_), 
		Hc is H + N, 
		H1 is Hc + 1, 
		tempsCalc(H1,C,A,D).

	/* Cas initial : il ne reste plus qu'un sommet, on arrive à l'heure H sur ce sommet et on en repart pas (c'est la fin du voyage) */
	tempsCalc(H,[X],[(X,H)],[]) :- X \= wait(_).
 

/**************/
/* Question 3 */
/**************/

/* Voyages sans collision :
	 On calcule toutes les heures d'arrivée et toutes les heures de départ de tous les voyages.
	 On enlève l'arrivée au premier sommet de chaque voyage car on arrive pas par téléportation à la première porte du voyage
		(exemple : dans l'énoncé, le voyage [a,b,c,z] est accepté en partant à 346, ors on a (a,346) dans les arrivées déjà programmées)
	 On enlève également l'heure d'arrivée au premier sommet du voyage testé.
	 Pour chaque voyage déjà programmé, on vérifie qu'il n'est pas en collision avec le voyage testé
	*/
pasdecollision(tripFixe(H,C)) :- 
	findall(A0,temps(A0,_),ResA),
	findall(D0,temps(_,D0),ResD),
	tempsCalc(H,C,[_|A],D),
	enleverLesPremiers(ResA,NewResA),
	rienEnCommun(A,NewResA),
	rienEnCommun(D,ResD).

rienEnCommun(A,[X|Rs]) :- 
	listesDisjointes(A,X),
	rienEnCommun(A,Rs).

rienEnCommun(_,[]).

enleverLesPremiers([[_|Rx]|Rs],[[Rx]|R]) :- enleverLesPremiers(Rs,R).
enleverLesPremiers([],_).

listesDisjointes([Xa|Ra],L) :-
	not(member(Xa,L)),
	listesDisjointes(Ra,L).

listesDisjointes([],_).

/*
  Génération de tous les voyages
  Avec test de collision sur les voyages deja programmés
  Le raisonnement est le suivant :
	1 ) On génére tous les voyages possibles. (chemin puis tous_trip)
	2 ) On "supprime|filtre" dans les voyages générés ceux qui sont en conflits avec les voyages déja programmés
*/
trip(Hdeb,Harr,Pdeb,Pfin,STrip):-
	chemin(Pdeb,Pfin,Ch),
	tous_trip(pasAttente,Hdeb,Harr,Pdeb,Pfin,STrip,Ch),
	pasdecollision(tripFixe(Hdeb,STrip)). 

/*La condition d'arrêt est très simple 
  On arrete lorsque nous somme à la porte d'arrivée souhaité*/
tous_trip(_,_,_,Pdeb,Pdeb,[Pdeb],_).

/*On fait ici l'aternance Wait|NoWait
  En verifiant à chaque fois si on ne depasse pas l'heure d'arrivée maximale
*/

tous_trip(attente,Init,Harr,Pdeb,Pfin,[wait(ValWait)|R],Ch):-
	N is Harr - Init,
	boucle(ValWait,1,N),
	NewInit is Init + ValWait,
	NewInit =< Harr,
	tous_trip(pasattente,NewInit,Harr,Pdeb,Pfin,R,Ch).
	
tous_trip(_,Init,Harr,Pdeb,Pfin,[Pdeb|R],[Pdeb,Y|Rs]):-
	NewInit is Init + 1,
	NewInit =< Harr,
	tous_trip(attente,NewInit,Harr,Y,Pfin,R,[Y|Rs]).

/*Boucle For à trois arguments.
  Cette boucle permet d'incrémenter une valeur.
  Cas d'utilisation : (Fonction tous_trip)
  Permet d'incrémenter la valeur de Wait de +1
  Ceci Permet de générer les Wait(K) dans les chemins 
*/
boucle(I,I,J):-
	I =< J.
boucle(K,I,J):-
	I < J,
	I1 is I +1,
	boucle(K,I1,J).

/**************/
/* Question 4 */
/**************/

	/* Le chemin optimal est le plus court parmis la liste de tous les chemins possibles (fournis par trip) */
	trip_opt(Hdeb,Harr,Pdeb,Parr,STrip) :-
		findall(X,trip(Hdeb,Harr,Pdeb,Parr,X),Res),
		tmin(STrip,Res).

	/* Calcul du chemin le plus court parmis une liste de chemins. */

		/* On passe par une fonction avec un argument supplémentaire : le temps du chemin le plus court trouvé jusqu'à maintenant */
		tmin(STrip,Res) :-
			tmin(STrip,Res,_).

		/* Cas initial : il n'y a qu'un seul chemin dans la liste, c'est lui le plus court */
		tmin(C,[C],T) :- tempsAbs(C,T).
	
		/* Deux cas possibles : 
			le plus court chemin n'est pas le premier de la liste 
			le plus court chemin est le premier de la liste
		*/
		tmin(CheminCourt,[C|Res], TCourt) :- tmin(CheminCourt,Res, TCourt), tempsAbs(C,TLong), TCourt =< TLong.
		tmin(CheminCourt,[CheminCourt|Res], TCourt) :- tmin(_,Res, TLong), tempsAbs(CheminCourt, TCourt), TCourt < TLong.

		/* Calcul de la durée d'un voyage (c'est l'heure d'arrivée au dernier sommet en partant à 0) */
		tempsAbs(C,T) :- tempsCalc(0,C,A,_), dernier(A,T).
		dernier(A,T) :- retourne(A,[(_,T)|_]).

