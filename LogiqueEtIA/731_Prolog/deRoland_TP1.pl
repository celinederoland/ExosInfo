/* 1) Ecrire un programme pour satisfaire le prédicat element(E, Xs ,Ys) permettant de sélectionner l’élément E d’une
liste Xs et de rendre la liste Ys égale à Xs moins E.*/
element(E,[E|Rs],Rs).
element(E,[X|Rs],[X|Ys]) :- element(E,Rs,Ys).

/*2) Ecrire le prédicat coderUn permettant de remplacer les répétitions consécutives d'un élément E par un seul E.*/
coderUn([],X,[X]).
coderUn([X|Rs],X,Res) :- coderUn(Rs,X,Res).
coderUn([Y|Rs],X,[X|Res]) :- \=(X,Y), coderUn(Rs,Y,Res).

coderUn([],[]).
coderUn([X|Xs],Ys) :- coderUn(Xs,X,Ys).

/*3) Ecrire le prédicat coderDeux permettant de remplacer les répétitions consécutives d'un élément par une liste [N,E]
où N est le nombre de répétitions et E est l'élément.*/
coderDeux([],1,X,[[1,X]]).
coderDeux([X|Rs],N,X,Res) :- N2 is N + 1, coderDeux(Rs,N2,X,Res).
coderDeux([Y|Rs],N,X,[[N,X]|Res]) :- \=(X,Y), coderDeux(Rs,1,Y,Res).

coderDeux([],[]).
coderDeux([X|Xs],Ys) :- coderDeux(Xs,1,X,Ys).

/*4) Ecrire le prédicat selectSuivant permettant de sélectionner l'élément qui suit dans une liste un élément déterminé.*/
selectSuivant([E,F|_],E,F).
selectSuivant([_|Rs],E,F) :- selectSuivant(Rs,E,F).

/*5) Ecrire un nouveau prédicat selectSuivants pour obtenir toutes les réponses du problème précédant sous la forme
d'une liste.*/ 
	/* On a besoin des deux initialisations pour traiter tous les cas */
selectSuivants([],_,[]).
selectSuivants([A],A,[]).
selectSuivants([E,F|Rs],E,[F|Res]) :- selectSuivants([F|Rs],E,Res).
selectSuivants([X|Rs],E,Res) :- \=(X,E), selectSuivants(Rs,E,Res).

/*6) Ecrire le prédicat selectNbr permettant de sélectionner (ou vérifier) K éléments d'une liste. La sélection se fait dans
l'ordre mais pas nécessairement consécutive.*/ 
selectNbr(0,_,[]).
selectNbr(N,[X|L1],[Y|L2]) :- selectNbr(N,L1,[Y|L2]), \=(X,Y).
selectNbr(N,[X|L1],[X|L2]) :- Np is N - 1, selectNbr(Np,L1,L2).


/*7) Compléter le programme du problème le singe et la banane. Ce programme était capable de répondre seulement
« true ». Apporter des modifications nécessaires pour qu’il rend, en paramètre de sortie, une liste avec les actions
effectuées pour atteindre l'état final.*/
etatInitial(etat(porte,fenetre,sol,pasbanane)).
etatFinal(etat(_,_,_,banane)).

mange(L) :- etatInitial(E), chercher(E,L).

chercher(E,[]) :- etatFinal(E).
chercher(E1,[X|L0]) :- action(E1,X,E2), chercher(E2,L0).
chercher(etatInitial(_),[]).

action(
	etat(milieu,milieu,dessus,pasbanane),
	prendre,
	etat(milieu,milieu,dessus,banane)
).
action(
	etat(X,X,sol,Y),
	monter,
	etat(X,X,dessus,Y)
).
action(
	etat(A,A,sol,Y),
	pousser(A,B),
	etat(B,B,sol,Y)
).
action(
	etat(A,X,sol,Y),
	marcher(A,B),
	etat(B,X,sol,Y)
).


/*8) Ecrire un programme sudoku(Xs) pour construire une liste avec les chiffres 1 à 9 sans répétition.*/
sudoku(X) :- permutations(X,[1,2,3,4,5,6,7,8,9]).
permutations([],[]).
permutations([X|Rs],L) :- element(X,L,Lr), permutations(Rs,Lr).

/*9) Un graphe orienté est défini par un ensemble de sommets et un ensemble d'arcs reliant ces sommets.*/ 
sommet(a). sommet(b). sommet(c).
sommet(d). sommet(e). sommet(f).
sommet(g).
arc(a, b). arc(a, c). arc(a, e).
arc(b, a). arc(b, d). arc(b, f).
arc(c, b). arc(c, d).
arc(e, a). arc(f, d).

pasVisite(_,[]).
pasVisite(X,[Y|Ys]) :- X\=Y, pasVisite(X,Ys).

chemin(X,Y,R) :- chemin(X,Y,[X],R).
chemin(X,Y,L,R) :- append(L,[Y],R), arc(X,Y).
chemin(X,Y,L,R) :- arc(X,I), pasVisite(I,L), append(L,[I],L1), chemin(I,Y,L1,R).
/* On ne vérifie pas qu'il y a une différence entre le sommet de départ et le sommet d'arrivée. Ainsi, on pourra trouver des chemins de a vers a. */


/*10) Une matrice est un objet composé d’éléments organisés en lignes et colonnes. */

	/*a) Ecrire la définition du prédicat ij(M, I, J, E) qui calcule dans E l’élément de rang J dans la ligne I de la matrice M.*/
			/* J'écris d'abord le prédicat getElem(L,Numero,E) qui calcule dans E l'élément numéro Numero de la liste L */
getElem([X|_],1,X).
getElem([_|Xs],J,E) :- K is J - 1, getElem(Xs,K,E).
			/* Puis j'utilise ce prédicat pour écrire le prédicat ij(M,I,J,E)*/
ij(M,I,J,E) :- getElem(M,I,L), getElem(L,J,E).

	/*b) La matrice transposée d’une matrice est obtenue en changeant les lignes en colonnes.*/ 
			/* J'écris le prédicat colonne(M,J,L) qui calcule dans C la liste des éléments de la colonne J de M. */
colonne([L1],J,[E]) :- getElem(L1,J,E).
colonne([L1|Ls],J,[E|Es]) :- getElem(L1,J,E), colonne(Ls,J,Es).
			/* J'écris le prédicat taille d'une liste */
taille([],0).
taille([_|Rs],J) :- taille(Rs,K), J is K + 1.
nbcol([L|_],N) :- taille(L,N).
			/* J'écris la transposée comme liste des colonnes de M */
transp(M,[L],N) :- colonne(M,N,L), nbcol(M,N).
transp(M,[L|Ts],J) :- colonne(M,J,L), K is J + 1, transp(M,Ts,K).
			/* J'écris le prédicat transposée */
transposee(M,T) :- transp(M,T,1).
