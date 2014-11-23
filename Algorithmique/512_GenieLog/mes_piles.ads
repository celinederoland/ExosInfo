-- TP2 info529
-- Morgan Durand
-- Céline de Roland

generic

type Element is private;
with procedure put(x:element);
with procedure debug(x:string);

package Mes_Piles is

type Cellule is private;

type Pile is access Cellule;

PileVide: constant Pile;

------------------- PILE_VIDE ---------------------
-- Renvoie true si la pile est vide, false sinon --
-- param_pile : IN Pile -> la pile à vérifier    --
---------------------------------------------------
function pile_vide(param_pile: IN Pile) return boolean;

------------------- EMPILER   ------------------------
-- Ajoute un élément au sommet de la pile           --
-- param_pile : IN OUT Pile -> la pile à modifier   --
-- param_valeur : IN integer -> la valeur à empiler --
------------------------------------------------------
procedure empiler(param_pile: IN OUT Pile; param_valeur: IN Element);

------------------- DEPILER   ------------------------
-- Enlève l'élément au sommet de la pile et					-- 
-- enregistre sa valeur.								            --
-- param_pile : IN OUT Pile -> la pile à modifier   --
-- param_resultat : OUT Element -> résultat					--
------------------------------------------------------
procedure depiler(param_pile: IN OUT Pile; param_resultat : OUT Element);

------------------- VISUALISER   ---------------------
-- Affiche tous les éléments de la pile							-- 
-- param_pile : IN OUT Pile -> la pile à modifier   --
------------------------------------------------------
procedure visualiser(param_pile: IN Pile);


private


------------------- TYPE CELLULE ------------------
-- valeur : integer -> valeur au sommet 	    --
-- ptr_precedent : Pile -> pointeur vers la pile
-- se trouvant en dessous				    --
---------------------------------------------------
type Cellule is
record
	valeur : Element;
	ptr_precedent : Pile;
end record;


PileVide: constant Pile := NULL;

end Mes_Piles;


