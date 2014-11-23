package annonceMain;

import in.keyboard.Keyboard;
import chaineTab.*;



public class Main_annonce {

	private static int i;
	
	public static Chaine Annonce(Chaine c1, Chaine c2) // testé avec Junit
	{
		int i = 0, j =0;
		Chaine c3 = new Chaine ();
		char el1;
		char el2 = c2.charAt(0);
		int long2 = c2.longueur();
		
		// 
		if (c1.isEgal(c2))
		{
			return c1;
		}
		else
		{
			for (i = 0; i < c1.longueur(); i++)
			{
				// c3 contient les j caractères communs entre la fin de (la souschaine de c1 du caractère 0 jusqu'au caractère i) et le début de c2.
				el1 = c1.charAt(i);
				
				if (el1 == el2)
				{
					
					long2--;
					if (long2 == 0 && i != c1.longueur())
					{
						c3.supprimer();
						j=0;
						el2 = c2.charAt(j);
						long2 = c2.longueur();
					}
					else 
					{
						c3.concatener(el1);
						el2 = c2.charAt(++j);
					}
				}
				else 
				{
					c3.supprimer();
					j=0;
					el2 = c2.charAt(j);
				}
				// c3 contient les j caractères communs entre la fin de (la souschaine de c1 du caractère 0 jusqu'au caractère i) et le début de c2.
			}
			// c3 contient les j caractères communs entre la fin de (la souschaine de c1 du caractère 0 jusqu'au caractère c1.longueur - 1) et le début de c2.
			// c3 contient les j caractères communs entre la fin de (					n												  ) et le début de c2.
		
			return c3;
		}
	}

	// variable du main
	public static boolean drapeau;
	public static int nb_lettre, j;
	
	public static void main(String[] args) {
		
		Chaine c1 = new Chaine();
		Chaine c2 = new Chaine();
		
		// On demande à l'utilisateur de rentrer des caracteres pour former 
		// des chaines (Listes ou tableaux) et on les affichent
		drapeau = true;
		nb_lettre = 10;
		j = 0;
		
		System.out.println ("Premiere chaine");
		while ( (j < nb_lettre) && (drapeau == true) )
		{
			System.out.println ("Rentrez un caractère et rentrez 1 pour finir");
			String caractere = Keyboard.getString();
			if (caractere.charAt(0) != '1' )
			{
				c1.concatener(caractere.charAt(0));
				j++;
			}
			else
			{
				drapeau = false;
			}
		}
		
		drapeau = true;
		nb_lettre = 10;
		j = 0;
		
		System.out.println ("Deuxieme chaine");
		while ( (j < nb_lettre) && (drapeau == true) )
		{
			System.out.println ("Rentrez un caractère et rentrez 1 pour finir");
			String caractere = Keyboard.getString();
			if (caractere.charAt(0) != '1' )
			{
				c2.concatener(caractere.charAt(0));
				j++;
			}
			else
			{
				drapeau = false;
			}
		}
		
		System.out.println ("Chaine c1 :");
		System.out.print ("c1 vaut : ");
		c1.afficher();
		System.out.println();
				
		System.out.println ("Chaine c2 :");
		System.out.print ("c2 vaut : ");
		c2.afficher();
		System.out.println();
		
	
	// Différentes méthodes que propose le package
	
	System.out.println ("caractere 1 de c1 :");
	System.out.println (c1.charAt(1));
	System.out.println();
	
	System.out.println ("Les chaînes sont-elles égales? ");
	System.out.println(c1.isEgal(c2));
	System.out.println();
				
	System.out.println ("c1 annonce c2 : Si oui renvoie ce qu'il annonce sinon rien. ");
	System.out.println ("L'annonce vaut donc : ");
	Annonce(c1, c2).afficher();
	System.out.println();
	
	System.out.println("La chaine c1 concatenée à c2 vaut : ");
	c1.concatener(c2);
	c1.afficher();
	System.out.println();
	
	System.out.println("La sous chaine de c1 vaut : ");
	c1.souschaine(1, 3).afficher();
	System.out.println();
	
	System.out.println("On supprime c1 ");
	c1.supprimer();
	System.out.print ("c1 vaut : ");
	c1.afficher();
	System.out.println();
	
	}
	
}
