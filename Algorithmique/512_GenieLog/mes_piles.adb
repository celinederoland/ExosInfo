-- TP2 info529
-- Morgan Durand
-- Céline de Roland

package body Mes_Piles is

------------------- PILE_VIDE ---------------------
-- Renvoie true si la pile est vide, false sinon --
-- param_pile : IN Pile -> la pile à vérifier    --
---------------------------------------------------
function pile_vide(param_pile: IN Pile) return boolean is
begin
	return (param_pile = NULL);
end pile_vide;

------------------- EMPILER   ------------------------
-- Ajoute un élément au sommet de la pile           --
-- param_pile : IN OUT Pile -> la pile à modifier   --
-- param_valeur : IN integer -> la valeur à empiler --
------------------------------------------------------
procedure empiler(param_pile: IN OUT Pile; param_valeur: IN Element) is
begin
	param_pile := new Cellule'(param_valeur, param_pile);
end empiler;

------------------- DEPILER   ------------------------
-- Enlève l'élément au sommet de la pile            --
-- param_pile : IN OUT Pile -> la pile à modifier   --
------------------------------------------------------
procedure depiler(param_pile: IN OUT Pile; param_resultat: OUT Element) is
begin
	if param_pile /= NULL then
		param_resultat := param_pile.valeur;
		param_pile := param_pile.ptr_precedent;
	end if;
end depiler;

------------------- VISUALISER   ---------------------
-- Affiche tous les éléments de la pile							-- 
-- param_pile : IN OUT Pile -> la pile à modifier   --
------------------------------------------------------
procedure visualiser(param_pile: IN Pile) is
	tmp_pile: Pile;
	tmp_resultat: Element;
begin
	tmp_pile := param_pile;
	while tmp_pile /= NULL loop
		depiler(tmp_pile,tmp_resultat);
		put(tmp_resultat);
	end loop;
end visualiser;

end Mes_Piles;
