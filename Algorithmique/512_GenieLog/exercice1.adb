-- TP2 info529
-- Morgan Durand
-- Céline de Roland

with Ada.Text_IO;
use Ada.Text_IO;
with Ada.Integer_Text_IO;
use Ada.Integer_Text_IO;
with Mes_Piles;

PROCEDURE exercice1 IS

	PROCEDURE Putlineentier(param_int: IN Integer) IS
	BEGIN
		Put(param_int);
		new_line;
	END Putlineentier;

	package Mes_Piles_Entiers is new Mes_Piles(Element => integer, put => putlineentier, debug => put_line);
	use Mes_Piles_Entiers;

  package boolio is new enumeration_io(boolean);
  use boolio;

	pile_test : Pile := PileVide;
BEGIN	

	

	Put_line("Création d'une pile vide");
	Put_line("Attendu : true");
	Put(pile_vide(pile_test));
	new_line;

	Put_line("Ajout de quelques valeurs dans la pile");

	empiler(pile_test,12);
	Put_line("Attendu : false");
	Put(pile_vide(pile_test));
	new_line;

	empiler(pile_test,28);
	Put_line("Attendu : false");
	Put(pile_vide(pile_test));
	new_line;

	empiler(pile_test,5);
	Put_line("Attendu : false");
	Put(pile_vide(pile_test));
	new_line;

	new_line;
	Put_line("Affichage de toute la pile");
	Put_line("--------------------------");
	visualiser(pile_test);
	new_line;

END exercice1;
