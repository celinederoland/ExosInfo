package Modele;

import java.util.Arrays;

public class Fourmis {

	private double[] villes_visitees = new double[50];
	private int nbVilles_visitees;
	private int nbVilles;
	private int ville_actuelle;
	private int ville_initiale;
	private int[] parcours;
	
	public void afficher()
	{
		System.out.println("La fourmis possède " + nbVilles + " villes");
		for (int i = 0; i < nbVilles; i++)
		{
			System.out.println("villes_visitees[" + i + "] = " + this.villes_visitees[i]);
		}
		System.out.println("ville actuelle = " + this.ville_actuelle);
		System.out.println("");
	}
	
	public Fourmis(Reseau res)
	{
		this.ville_actuelle = (int)(Math.random()*res.getNbVilles());
		this.ville_initiale = this.ville_actuelle;
		this.initialiser(res);
	}
	
	public void initialiser(Reseau res)
	{
		for (int i = 0; i < 50; i++) { this.villes_visitees[i] = 1; }
		this.villes_visitees[ville_actuelle] = 0;
		this.nbVilles = res.getNbVilles();
		this.parcours = new int[this.nbVilles];
		this.parcours[0] = ville_actuelle;
		this.nbVilles_visitees = 1;
		this.calculer_proba_ville(res);
	}
	
	public void calculer_proba_ville(Reseau res)
	{
		//Calcul du dénominateur//
		double total = 0;
		for (int i = 0; i < this.nbVilles; i++)
		{
			if (this.villes_visitees[i] != 0)
			{
				total += res.probaVille(this.ville_actuelle, i); 
			}
		}
		
		//
		double cumul = 0;
		for (int i = 0; i < this.nbVilles; i++)
		{
			if (this.villes_visitees[i] != 0)
			{
				double proba = res.probaVille(this.ville_actuelle, i)/total; 
				if (proba < 0.001) { proba = (double)0.001; } //magouille pour éviter que (float)proba donne 0 si proba est trop petit
				cumul += res.probaVille(this.ville_actuelle, i)/total;
				this.villes_visitees[i] = cumul;
			}
		}
	}
	
	public boolean choisir_ville(Reseau res)
	{
		if (this.nbVilles_visitees < this.nbVilles) 
		{
			double nombre_alea = (double)Math.random();
			int i = 0;
			while (nombre_alea > this.villes_visitees[i] ) //|| this.villes_visitees[i] == 0
			{
				i++;
			}
			
			this.ville_actuelle = i;
			this.villes_visitees[this.ville_actuelle] = 0;
			this.parcours[this.nbVilles_visitees++] = this.ville_actuelle;
			this.calculer_proba_ville(res);
			return true;
		}
		else
		{
			res.deposer_pheromones(this.parcours);
			this.ville_actuelle = ville_initiale;
			this.initialiser(res);
			return false;
		}
	}
	
	public int getVilleActuelle()
	{
		return this.ville_actuelle;
	}
}
