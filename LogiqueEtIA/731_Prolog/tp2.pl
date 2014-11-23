connection(a,b).
connection(a,c).
connection(b,c).
connection(c,a).
connection(c,z).


tripFixer(345,[a,b,wait(2),c,wait(1),z]).
tripFixer(346,[a, wait(3), b, c, z]).

trip(X,X,_):-
	fail.

trip(X,Y,Zs):-
	X \= Y,
	chemin(X,Y,Zs).


/*%Definition de la fonction temps

temps(A,D):-
	tripFixer(H,Ch),
	tempsCalcul(H,Ch,A,D).

tempsCalcul

temps(Rs,Ls):-
	tripFixer(Arriv,[X,Y|_]),
	connection(X,Y),
	temps([X,Arriv|Rs],[Y,Arriv]|Ls).*/


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
pasVisite(_,[]).
pasVisite(X,[Y|Ys]):-
	X\=Y,
	pasVisite(X,Ys).
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
/*
%@Param Entree
%X : Sommet de départ
%Y : Sommet d'arrivée
%@Param Sortie
%L : Liste des sommets traversée
chemin(X,Y,L):-
	cheminBoucle(X,Y,[X],L).

%Param Entree
%X : Sommet de départ
%Y : Sommet d'arrivée
%Param Sortie
%L : Liste des sommets traversée
%R : Variable de stockage (Temporaire)*/

cheminBoucle(X,Y,L,R):-
	append(L,[Y],R),
	connection(X,Y).

cheminBoucle(X,Y,M,L):-
	connection(X,Z),
	pasVisite(Z,M),
	append(M,[Z],R),
	cheminBoucle(Z,Y,R,L).



temps(A,D) :- tripFixer(H,C), tempsCalc(H,C,A,D).

tempsCalc(H,[X,Y|C],[(X,H)|A],[(X,H)|D]) :- 
	X \= wait(_), 
	Y \= wait(_), 
	H1 is H + 1, 
	tempsCalc(H1,[Y|C],A,D).

tempsCalc(H,[X,wait(N)|C],[(X,H)|A],[(X,Hc)|D]) :- 
	X \= wait(_), 
	Hc is H + N, 
	H1 is Hc + 1, 
	tempsCalc(H1,C,A,D).

tempsCalc(H,[X],[(X,H)],[]) :- X \= wait(_).


pasdecollision(tripFixer(H,C)) :- 
	findall(A0,temps(A0,_),ResA),
	findall(D0,temps(_,D0),ResD),
	tempsCalc(H,C,A,D),
	rienEnCommun(A,ResA),
	rienEnCommun(D,ResD).

rienEnCommun(A,[X|Rs]) :- 
	listesDisjointes(A,X),
	rienEnCommun(A,Rs).

rienEnCommun(_,[]).

listesDisjointes([Xa|Ra],L) :-
	not(member(Xa,L)),
	listesDisjointes(Ra,L).

listesDisjointes([],_).

