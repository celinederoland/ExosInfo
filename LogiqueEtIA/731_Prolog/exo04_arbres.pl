/* Objectif : nombre de noeuds d'un arbre */
nbnoeuds(vide,0).
nbnoeuds(arb(_,G,D),Res) :- nbnoeuds(G,TG), nbnoeuds(D,TD), Res is 1 + TG + TD.

/* objectif : membre d'un arbre ou pas */
membre(X,arb(X,_,_)).
membre(X,arb(_,G,_)) :- membre(X,G).
membre(X,arb(_,_,D)) :- membre(X,D).

/* objectif : construire un arbre binaire ordonné à partir d'une liste (les elts à gauche < racine < les elts à droite) */
const([],vide).
const([X|Xs],arb(X,G,D)) :- decomposer(Xs,X,Gs,Ds), const(Gs,G), const(Ds,D).
decomposer([],_,[],[]).
decomposer([Y|Ys],X,[Y|Gs],Ds) :- Y @=< X, decomposer(Ys,X,Gs,Ds).
decomposer([Y|Ys],X,Gs,[Y|Ds]) :- X @=< Y, decomposer(Ys,X,Gs,Ds).

/* objectif : parcours infixe */
concatenation([],L,L).
concatenation([X|Reste],L, [X|ConcatRestL] ) :- concatenation(Reste,L,ConcatRestL).
infixe(vide,[]).
infixe(arb(R,G,D),L) :- infixe(G,Gs), infixe(D,Ds), concatenation(Gs,[R|Ds],L).
