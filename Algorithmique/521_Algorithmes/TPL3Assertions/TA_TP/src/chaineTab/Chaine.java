package chaineTab;

import java.util.Arrays;

public class Chaine {

	public char value[];
	int taille;
	int offset;
	

	// constructeur
	public Chaine ()
	{
		this.offset = 0;
		this.value = new char[0];
		this.taille = 0;
	}
	
	public Chaine(char pvalue[]) //testée indirectement JUnit
	{
		this.offset = 0;
		this.value = pvalue;
		this.taille = pvalue.length;
	}
	
	//methodes
	
	public void concatener(Chaine c2) //testée JUnit
	{
		char tmp[];
		int taille = this.taille + c2.longueur();
		tmp = Arrays.copyOf(this.value, taille);
		for (int i = c2.getOffset(); i < c2.longueur(); i++)
		{
			tmp[this.taille + i] = c2.charAt(i);
		}
		this.taille = taille;
		this.value = tmp;
	}
	
	public void concatener(char c2) //testée JUnit
	{
		char pvalue[] = new char[1];
		pvalue[0] = c2;
		Chaine chaine2 = new Chaine(pvalue) ;
		this.concatener(chaine2);
	}
	
	public char charAt(int num) //testée JUnit
	{
		return this.value[this.offset + num];
	}
	
	public Chaine souschaine(int start, int longueurf) //testée JUnit
	{
				
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
		this.offset += start;
		this.taille = this.offset + longueurf;
		return this;
	}
	
	public int longueur() //testée indirectement JUnit
	{
		return (this.taille - this.getOffset());
	}
	
	public void afficher() //testée visuellement
	{
		for (int i = this.offset; i < this.taille; i++)
		{
			System.out.print(this.value[i]);
		}
		System.out.println();
	}
	
	public boolean isEgal(Chaine c2) //testée indirectement JUnit
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

	public boolean isVide() //testée JUnit
	{
		return (this.longueur() == 0);
	}
	
	private int getOffset() { return this.offset; } //testée indirectement JUnit
	
	
	public void supprimer() // testée JUnit
	{
		this.offset = 0;
		char pvalue[] =new char[0];
		this.value = pvalue;
		this.taille = pvalue.length;
	}
}
