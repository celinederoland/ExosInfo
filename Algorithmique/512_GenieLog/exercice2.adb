-- TP2 info529
-- Morgan Durand
-- Céline de Roland

with Ada.Strings.Unbounded;
use Ada.Strings.Unbounded;
with Ada.Strings.Unbounded.Text_IO;
use Ada.Strings.Unbounded.Text_IO;

WITH Ada.Text_IO;
USE Ada.Text_IO;
WITH Ada.Integer_Text_IO;
USE Ada.Integer_Text_IO;

with Mes_Piles;


procedure exercice2 is

   -- INITIALISATION DE LA PILE GENERIQUE



   -- il faut creer la fonction generique put

	 -- pulineentier --
	 -- affiche un entier et retourne à la ligne
	 -- @param1 : param_int : integer (entier à afficher)

   PROCEDURE Putlineentier(param_int : IN Integer) IS
   BEGIN
      Put(param_int);
      new_line;
   END Putlineentier;


   package Mes_Piles_Entiers is new Mes_Piles(Element => integer, put => putlineentier, debug => put_line);
   use Mes_Piles_Entiers;



   -- SOUS PROCEDURES / SOUS FONCTIONS



	 -- obtenir --
	 -- récupère les valeurs et les opérandes dans la ligne entrée par l'utilisateur 
   -- renvoie au fur à mesure le contenu du buffer pour alimenter le programme
   -- @return unbounded_string 

	 function obtenir return unbounded_string is

			-- tmp_char : character utilisé pour enregistrer le caractère suivant du buffer
	  	tmp_char: Character;
			-- Element : unbounded_string : élément à renvoyer à la fin.
			Element: unbounded_string;

	 begin

			Element := to_unbounded_string("");
			get(tmp_char);
			while (tmp_char = ' ' and tmp_char /= '=') loop
				get(tmp_char);
			end loop;
			if (tmp_char = '=') then
				Element := Element & tmp_char;
			else
				while (tmp_char /= ' ' and tmp_char /= '=') loop
					Element := Element & tmp_char;
					get(tmp_char);
				end loop;
			end if;		
			return Element;

	 end obtenir;


   -- is_operande --
   -- permet de déterminer si la chaine passé en caractère est un caractère d'opérande (+,-,*,/)
   -- @param1 : operande : Unbounded_String

   function is_operande (o : Unbounded_String) return boolean is

   begin

      case To_String(o)(1) is
      	when '+' => return true;
      	when '-' => return true;
      	when '*' => return true;
      	when '/' => return true;
      	when others => return false;
      end case;

   end is_operande;


   -- faire_operation --
   -- permet d'effectuer une opération (grâce aux élément dans la pile) avec l'opérande passé en paramètre
   -- @param1 : La pile in_out
   -- @param2 : operande : Unbounded_String

   procedure faire_operation( p : in out Pile ; o : Unbounded_String) is

			-- resultat : integer enregistre le résultat de l'opération pour l'empiler
      resultat                          : Integer;
			-- operateur1, operateur2 : integer enregistrent les 2 derniers éléments dépilés de la pile pour effectuer l'opération
      operateur1, operateur2            : integer;

   begin

      case To_String(o)(1) is
      	when '+' =>
         depiler(p, operateur2);
         depiler(p, operateur1);
         resultat := operateur1 + operateur2;
         empiler(p,resultat);

      	when '-' =>
         depiler(p, operateur2);
         depiler(p, operateur1);
         resultat := operateur1 - operateur2;
         empiler(p,resultat);

      	when '*' =>
         depiler(p, operateur2);
         depiler(p, operateur1);
         resultat := operateur1 * operateur2;
         empiler(p,resultat);

      	when '/' =>
         depiler(p, operateur2);
         depiler(p, operateur1);
         resultat := operateur1 / operateur2;
         empiler(p,resultat);

      	when others => null;

      end case;

   end faire_operation;



	 -- DECLARATIONS DE VARIABLES

	 -- expression : Pile contient les opérandes et opérateurs à traiter
   expression                        	     : Pile := PileVide;
	 -- elem : unbounded_string contient le dernier élément à empiler
   elem                              : Unbounded_String;
	 -- result : integer Résultat de toute l'opération
   result                                 : Integer;



	 -- DEBUT DE LA PROCEDURE PRINCIPALE

	 begin

	 put_line("Entrez sur une ligne l'expression à calculer en notation polonaise");

   -- structure algorithmique du tant que
   elem := obtenir;
   while to_string(elem)(1) /= '=' loop

      -- si le caractère est une opérande, on effectue l'opération
      if (is_operande(elem)) then
         faire_operation(expression, elem);
      -- sinon on empile le nombre
      else
         empiler(expression,Integer'Value(To_String(elem)));
      end if;

      elem := obtenir;
   end loop;

   depiler(expression, result);

   -- si la pile est vide après le dépilage, c'est que l'expression était correctement formée
   if (pile_vide(expression)) then
      put_line("Resultat de l'operation : ");
      Putlineentier(result);
   else
      put_line("l'expression n'est pas bien formee");
   end if;

end exercice2;
