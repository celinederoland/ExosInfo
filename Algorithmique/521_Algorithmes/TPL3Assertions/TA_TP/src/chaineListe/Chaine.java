package chaineListe;

import java.io.*;

// test identique que pour chaineTab
// tous testés avec JUnit

public class Chaine {

	// attributs
	
	Element premier;
		
	// constructeur
	
	public Chaine ()
	{
		this.premier = null;
	}
	
	public Chaine (char tab[])
	{
		this.premier = null;
		for (int i = 0; i < tab.length; i++)
		{
			this.concatener(tab[i]);
		}
	}

	// methodes
	
	/*premier element d'une liste */
	private Element getPremier()
	{
		return this.premier;
	}
		
	/* Savoir si Liste Vide */
	public boolean isVide()
	{
		return this.premier == null;
	}
	
	
	/* Insertion d'un élément à la fin */
	 public void concatener(char c)
	 {
		    Element e = new Element(c);
		    Element ptr = getPremier();
		    if (this.isVide())
		    {
		        this.premier = e;
		    }
		    else
		    {
		    	while(ptr.getSuivant() != null)
		    	{
		    		ptr = ptr.getSuivant();
		    	}
		        ptr.setSuivant(e);
		    }
	 }
	
	
	 
	public void concatener(Chaine c2)
	{
		if (this.isVide())
		{
			this.premier = c2.getPremier();
		}
		else
		{
			Element ptr = this.getPremier();
			while (ptr.getSuivant() != null)
			{
				ptr = ptr.getSuivant();
			}
			ptr.setSuivant(c2.getPremier());
		}
	}
	
	public char charAt(int num)
	{
		Element ptr = this.getPremier();
		while (num > 0)
		{
			if (this.isVide())
			{
				System.out.println("l'element n'existe pas !!!");
				return '0';
			}
			else
			{
				ptr = ptr.getSuivant();
				num--;	
			}
		}
				
		return ptr.getElement();
	}
	
	public Chaine souschaine(int start, int longueurf)
	{
		Element ptr = this.getPremier();
		Chaine ctr = new Chaine();
		
		if (start<0)
		{
			start =0;
		}
		if (longueurf <0)
		{
			longueurf =0;
		}
		if (start > this.longueur()-1)
		{
			this.supprimer();
			return this;
		}
		if (start + longueurf > this.longueur())
		{
			longueurf = this.longueur()-start;
		}
		for (int i = 0; i < start; i++)
		{
			ptr = ptr.getSuivant();
		}
		for (int i = 0; i < longueurf; i++)
		{
			ctr.concatener(ptr.getElement()) ;
			ptr = ptr.getSuivant();	
		}
		return ctr;
	}
		
	
	public int longueur()
	{
		int taille =0;
		Element ptr = getPremier();
		while (ptr != null)
		{
			taille++;
			ptr = ptr.getSuivant();
		}
		return taille;
	}
	
	public void afficher()
	{
		Element ptr = this.getPremier();
		while (ptr != null)
		{
			System.out.print(ptr.getElement());
			ptr = ptr.getSuivant();
		}
		System.out.println ();
	}
	
	public boolean isEgal(Chaine c2)
	{
		boolean ok_value = true;
		if (this.longueur() == c2.longueur())
		{
			for (int i = 0; i < this.longueur(); i++)
			{
				if (this.charAt(i) != c2.charAt(i)) { ok_value = false; }
			}
		}
		else
		{ 
			ok_value = false;
		}
		return (ok_value);
	}
	
	public Chaine supprimer()
	{
		this.premier=null;
		return this;
	}
	
	
	/*public Chaine Annonce(Chaine c2)
	{
		int i=0;
		Chaine c3 = new Chaine();
		Element ptr1 = this.getPremier();
		Element ptr2 = c2.getPremier();
		while (ptr1 != null)
		{
			if (ptr1.getElement() == ptr2.getElement())
			{
				c3.concatener(this.CharAt(i)) ;
				ptr2 = ptr2.getSuivant();
			}
			else
			{
				c3.Supprimer();
				ptr2 = c2.getPremier();
			}
			ptr1 = ptr1.getSuivant();
			i++;
		}
		
		return c3;
	}*/
}
