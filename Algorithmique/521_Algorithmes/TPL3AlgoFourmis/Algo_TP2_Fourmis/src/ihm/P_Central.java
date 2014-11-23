package ihm;

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;

import javax.swing.JPanel;

import Modele.Fourmis;
import Modele.Reseau;
import Modele.Ville;

/**
 * Classe P_Central : affichage du graphe des villes
 * 
 * @author Céline de Roland
 * @author Mickaël Koza
 * @version 1
 * 
 *
 */
public class P_Central extends JPanel {
	
	private static final long serialVersionUID = -5807043172375174594L;

	private Reseau reseau = new Reseau();
	private Fourmis fourmis[];
	private int nbFourmis;
	private int cpt_cycle = 0;
	private int verbeux = -1;
	
	/**
	 * 
	 * Affiche toutes les villes et les connections entre elles
	 * 
	 * @param g : Graphics
	 */
	public void paintComponent(Graphics g)
	{
		g.setColor(Color.WHITE);
		g.fillRect(0, 0, this.getWidth(), this.getHeight());
	    g.setColor(Color.BLACK);
	    for (int i = 0; i < this.reseau.getNbVilles(); i++)
	    {
	    	g.setColor(Color.RED);
	    	BasicStroke stroke = new BasicStroke(4);
	    	Graphics2D g2d = (Graphics2D)(g);
			g2d.setStroke(stroke);
	    	g2d.drawString(this.reseau.ville(i).getNom(),this.reseau.ville(i).getPosX()-20, this.reseau.ville(i).getPosY() + 20);
	    	g2d.fillOval(this.reseau.ville(i).getPosX()-5, this.reseau.ville(i).getPosY()-5, 10, 10);
	    	g.setColor(Color.BLACK);
	    	for (int j = i + 1; j < this.reseau.getNbVilles(); j++)
	    	{
	    		if (cpt_cycle == 0) { stroke = new BasicStroke((float)(this.reseau.arrete(i,j))); }
	    		else { stroke = new BasicStroke((float)(1000*this.reseau.arrete(i,j))); }
	    		g2d = (Graphics2D)(g);
	    		g2d.setStroke(stroke);
	    		g2d.drawLine(this.reseau.ville(i).getPosX(), this.reseau.ville(i).getPosY(), this.reseau.ville(j).getPosX(), this.reseau.ville(j).getPosY());
	    	}    	
	    }
	    if (cpt_cycle != 0)
	    {
	    	
		    if (this.cycle() < 10 && this.cpt_cycle < 2000) { 
		    	try {
		    		Thread.sleep(50);
		    	} catch (InterruptedException e) {
		    		e.printStackTrace();
		    	} 
		    	this.repaint(); 
		    }
		    else
		    {
		    	g.setColor(Color.GREEN);
		    	BasicStroke stroke = new BasicStroke(2);
		    	Graphics2D g2d = (Graphics2D)(g);
				g2d.setStroke(stroke);
			    for (int i = 0; i < this.reseau.getNbVilles() - 1; i++)
			    {
			    	Ville depart = this.reseau.getMeilleur(i);
			    	Ville arrivee = this.reseau.getMeilleur(i+1);
			    	g2d.drawLine(depart.getPosX(), depart.getPosY(), arrivee.getPosX(), arrivee.getPosY());
			    }
			    if (this.reseau.getNbVilles() >= 3)
			    {
			    	Ville depart = this.reseau.getMeilleur(this.reseau.getNbVilles() - 1);
			    	Ville arrivee = this.reseau.getMeilleur(0);
			    	g2d.drawLine(depart.getPosX(), depart.getPosY(), arrivee.getPosX(), arrivee.getPosY());
			    }
		    }
	    }
	}   
	
	/**
	 * 
	 * Enregistre une nouvelle ville à afficher
	 * 
	 * @param x : abscisse de la ville
	 * @param y : ordonnée de la ville
	 * @param nom : nom de la ville
	 * 
	 */
	public void setCoord(int x,int y, String nom)
	{
		this.reseau.ajoutVille(x,y,nom);
		//this.reseau.meilleur_chemin();
		this.repaint();
	}
	
	public void ajoutFourmis(int nbFourmis)
	{
		this.nbFourmis = nbFourmis;
		fourmis = new Fourmis[nbFourmis];
		for (int i = 0; i < nbFourmis; i++)
		{
			fourmis[i] = new Fourmis(this.reseau);
		}
	}
	
	public int cycle()
	{
		for (int i = 0; i < nbFourmis; i++)
		{
			boolean pas_fini = true;
			//Une fourmis fait son cycle : elle visite des villes jusqu'à avoir visité toutes les villes.
			while (pas_fini) { 
				pas_fini = fourmis[i].choisir_ville(this.reseau);
			}
		}
		if (this.verbeux == 2) { fourmis[0].afficher(); }
		this.reseau.evaporer();
		if (this.verbeux > 0) { this.reseau.afficher(this.cpt_cycle); }
		cpt_cycle++;
		if (this.verbeux > -1) { System.out.println("fin du cycle numéro " + cpt_cycle); }
		return this.reseau.meilleur_chemin(this.verbeux);
	}
	
	public void demarrer(int pverbeux)
	{
		this.verbeux = pverbeux;
		this.cpt_cycle = 1;
		this.repaint();
	}
}
