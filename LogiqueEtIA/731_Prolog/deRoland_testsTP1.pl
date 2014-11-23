/* 1) Ecrire un programme pour satisfaire le prédicat element(E, Xs ,Ys) permettant de sélectionner l’élément E d’une
liste Xs et de rendre la liste Ys égale à Xs moins E.*/
element(3,[2,3,1],Y).
element(3,[2,3,1,3,3,5],Y).
element(3,[2,2],Y).

/*2) Ecrire le prédicat coderUn permettant de remplacer les répétitions consécutives d'un élément E par un seul E.*/
coderUn([a,a,a,b,c,c,d],Y).
coderUn([],Y).
coderUn([a,a,a],Y).
coderUn([a,a,a,b],Y).


/*3) Ecrire le prédicat coderDeux permettant de remplacer les répétitions consécutives d'un élément par une liste [N,E]
où N est le nombre de répétitions et E est l'élément.*/
coderDeux([a,a,a,b,c,c,d],Y).
coderDeux([],Y).
coderDeux([a,a,a],Y).
coderDeux([a,a,a,b],Y).

/*4) Ecrire le prédicat selectSuivant permettant de sélectionner l'élément qui suit dans une liste un élément déterminé.*/
selectSuivant([a,b,c,a,d,e,f,a,g,h,i,k,a],a,E).
selectSuivant([a,b,c,a,a,a,f,a,g,h,i,k,a],a,E).
selectSuivant([a],a,E).
selectSuivant([d],a,E).
selectSuivant([],a,E).

/*5) Ecrire un nouveau prédicat selectSuivants pour obtenir toutes les réponses du problème précédant sous la forme
d'une liste.*/ 
selectSuivants([a,b,c,a,d,e,f,a,g,h,i,k,a],a,E).
selectSuivants([a,b,c,a,a,a,f,a,g,h,i,k,a],a,E).
selectSuivants([a],a,E).
selectSuivants([d],a,E).
selectSuivants([],a,E).

/*6) Ecrire le prédicat selectNbr permettant de sélectionner (ou vérifier) K éléments d'une liste. La sélection se fait dans
l'ordre mais pas nécessairement consécutive.*/ 
selectNbr(3,[a,b,c,d,e],Xs).
selectNbr(5,[a,b,c,d,e],Xs).
selectNbr(6,[a,b,c,d,e],Xs).
selectNbr(0,[a,b,c,d,e],Xs).

/*7) Compléter le programme du problème le singe et la banane. Ce programme était capable de répondre seulement
« true ». Apporter des modifications nécessaires pour qu’il rend, en paramètre de sortie, une liste avec les actions
effectuées pour atteindre l'état final.*/
mange(L).

/*8) Ecrire un programme sudoku(Xs) pour construire une liste avec les chiffres 1 à 9 sans répétition.*/
sudoku(Xs).
sudoku([A,B,C,D,E,F,G,H,I]).
sudoku([A,6,C,D,E,4,G,H,I]).

/*9) Un graphe orienté est défini par un ensemble de sommets et un ensemble d'arcs reliant ces sommets.*/ 
chemin(a,f,C).
chemin(a,g,C).
chemin(g,b,C).
chemin(a,a,C).

/*10) Une matrice est un objet composés d’éléments organisés en lignes et colonnes. */
	/*a) Ecrire la définition du prédicat ij(M, I, J, E) qui calcule dans E l’élément de rang J dans la ligne I de la matrice M.*/
ij([ [3, 7, 5], [4, 1, 2]],2,1,E).

	/*b) La matrice transposée d’une matrice est obtenue en changeant les lignes en colonnes.*/ 
transposee([ [3, 7, 5], [4, 1, 2]],T).
