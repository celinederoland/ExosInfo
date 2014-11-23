/* objectif : concaténation de 2 listes */

concatenation([],L,L).
concatenation([X|Reste],L, [X|ConcatRestL] ) :- concatenation(Reste,L,ConcatRestL).

/* objectif : enlever la tête à une liste */

enlever([_|R],R).

/* objectif : obtenir le dernier élément d'une liste */
dernier([X],X).
dernier([_|R],X) :- dernier(R,X).

/* objectif : y a t'il des répétitions dans la liste ? */
repetition([X,X]).
repetition([_|R]) :- repetition(R).
repetition([X|R]) :- membre(X,R).

/* objectif : ajouter un élément en tête et en queue de liste */
ajoutEnTete(X,L1,[X|L1]). 
ajoutEnQueue(X,[],[X]).
ajoutEnQueue(X,[El|Reste],[El|AjoutEnQueueRest]) :- ajoutEnQueue(X,Reste,AjoutEnQueueRest).

/* objectif : trouver deux éléments consécutifs */
consec([A,B|_],A,B).
consec([_|Reste],A,B) :- consec(Reste,A,B).

/* objectif : faire la même chose en utilisant la concaténation */
consecBis(L,A,B) :- concatenation(_,[A,B|_],L).

/* objectif : trouver l'avant dernier elt d'une liste*/
avDernier([X,_],X).
avDernier([_|R],X) :- avDernier(R,X). 

/* objectif : trouver si une liste est préfixe d'une autre (rq : vide est préfixe de toute liste) */
prefixe(L,M) :- concatenation(L,_,M).

/* objectif : trouver si une liste est suffixe d'une autre (rq : vide est préfixe de toute liste) */
suffixe(L,M) :- concatenation(_,L,M).

/* objectif : trouver si une liste est une sous liste d'une autre */
sousliste(S,L) :- prefixe(S,L).
sousliste(S,[_|R]) :- sousliste(S,R).

/* objectif : doubler ou tripler une liste */
double(L,Res) :- concatenation(L,L,Res).
triple(L,Res) :- double(L,Y), concatenation(Y,L,Res).

/* objectif : inverser une liste */
inverse([],[]).
inverse([X|R],I) :- inverse(R,Rev), concatenation(Rev,[X],I).

/* objectif : isérer un élément n'importe où dans une liste */
inserer(X,L,Res) :- concatenation(Pref,Suf,L), concatenation(Pref,[X|Suf],Res).
insBis(E,L,[E|L]).
insBis(E,[_|R],[_|Res]) :- insBis(E,R,Res).

/* objectif : longueur d'une liste */
long([],0).
long([_|R],Res) :- long(R,L), Res is L + 1.

/* objectif : additionner les éléments d'une liste */
add([],0).
add([X|R],Res) :- add(R,T), Res is T + X.

/* objectif : maximum d'une liste */
max([X],X).
max([X|R],X) :- max(R,Res), X @>= Res.
max([X|R],Res) :- max(R,Res), X @=< Res.
