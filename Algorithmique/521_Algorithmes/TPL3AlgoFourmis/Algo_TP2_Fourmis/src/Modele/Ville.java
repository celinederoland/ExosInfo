package Modele;

/**
 * Classe Ville : type de données ville, contenant une abscisse et une ordonnée de type int, et un nom de type String
 * 
 * @author Céline de Roland
 * @author Mickaël Koza
 * @version 1
 * 
 *
 */

public class Ville {

	private int posX;
	private int posY;
	private String nom;
	
	public void afficher()
	{
		System.out.println(this.nom + " ( " + this.posX + " ; " + this.posY + " )");
	}
	
	public Ville(int x,int y,String pnom)
	{
		this.posX = x;
		this.posY = y;
		this.nom = pnom;
	}
	
	public int getPosX()
	{
		return this.posX;
	}
	
	public int getPosY()
	{
		return this.posY;
	}
	
	public String getNom()
	{
		return this.nom;
	}
}
