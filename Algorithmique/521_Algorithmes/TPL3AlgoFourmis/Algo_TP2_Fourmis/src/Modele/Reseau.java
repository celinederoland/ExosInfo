package Modele;

public class Reseau {

	private double arretes[][];
	private Ville villes[];
	private int nbVilles = 0;
	private int meilleur[];
	private int vrai_meilleur[];
	private int cpt = 0;
	private double distance_meilleure;
	private double vraie_distance_meilleure;
	
	public void afficher(int pcpt_cycle)
	{
		if (pcpt_cycle == 2)
		{
			System.out.println("Ce réseau possède " + this.nbVilles + " villes :");
			for (int i = 0; i < this.nbVilles; i++)
			{
				this.villes[i].afficher();
			}
			System.out.println("");
			try {
				Thread.sleep(5000);
			} 
			catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		System.out.println("Etat du réseau : ");
		for (int i = 0; i < this.nbVilles; i++)
		{
			for (int j = 0; j < this.nbVilles; j++)
			{
				System.out.print(this.arretes[i][j] + "  ");
			}
			System.out.println("");
		}
		System.out.println("");
		System.out.println("Le meilleur chemin pour ce réseau est");
		for (int i = 0; i < this.nbVilles; i++)
		{
			System.out.print(this.meilleur[i] + "  ->  ");
		}
		System.out.println("");
	}
	
	public Reseau()
	{
		this.arretes = new double[50][50];
		this.villes = new Ville[50];
	}
	
	public double arrete(int i, int j)
	{
		return this.arretes[i][j];
	}
	
	public int getNbVilles()
	{
		return this.nbVilles;
	}
	
	public Ville ville(int i)
	{
		return this.villes[i];
	}
	
	public void ajoutVille(int x, int y, String nom)
	{
		this.villes[this.nbVilles++] = new Ville(x,y,nom);
		this.vrai_meilleur = new int[this.nbVilles];
		this.meilleur = new int[this.nbVilles];
		this.vraie_distance_meilleure = 3000*this.nbVilles;
		for (int i = 0; i < this.nbVilles; i++)
		{
			this.arretes[i][this.nbVilles] = 0.1;
			this.arretes[this.nbVilles][i] = 0.1;
		}
	}
	
	public double distance(int i, int j)
	{
		int X =(this.villes[i].getPosX() - this.villes[j].getPosX());
		int Y = (this.villes[i].getPosY() - this.villes[j].getPosY());
		return (double)Math.sqrt(X*X + Y*Y);
	}
	
	//Calcule uniquement le numérateur de la formule donnée en cours//
	public double probaVille(int i, int j)
	{
		return (this.arretes[i][j]/this.distance(i,j));
	}
	
	public double lg_parcours(int[] parcours)
	{
		double dist = 0;
		for (int i = 0; i < this.nbVilles - 1; i++)
		{
			dist += this.distance(parcours[i],parcours[i+1]);
		}
		dist += this.distance(parcours[0], parcours[nbVilles - 1]);
		return dist;
	}
	
	public void deposer_pheromones(int[] parcours)
	{
		for (int i = 0; i < this.nbVilles - 1; i++)
		{
			this.arretes[parcours[i]][parcours[i+1]] += 1/this.lg_parcours(parcours);
			this.arretes[parcours[i+1]][parcours[i]] = this.arretes[parcours[i]][parcours[i+1]];
	 	}
	}
	
	public void evaporer()
	{
		for (int i = 0; i < this.nbVilles; i++)
		{
			for (int j = 0; j < this.nbVilles; j++)
			{
				this.arretes[i][j] *= (double)0.7;
			}
		}
	}
	
	public int meilleur_chemin(int pverbeux)
	{
		this.meilleur = new int[this.nbVilles];
		this.meilleur[0] = 0;
		double[] nonvisitees = new double[this.nbVilles];
		for (int i = 0; i < this.nbVilles; i++)
		{
			nonvisitees[i] = this.arretes[0][i];
		}
		for (int i = 1; i < this.nbVilles; i++)
		{
			int indiceduplusgrand = 1;
			for (int j = 1; j < this.nbVilles; j++)
			{
				if (nonvisitees[j] > nonvisitees[indiceduplusgrand]) { indiceduplusgrand = j; }
			}
			this.meilleur[i] = indiceduplusgrand;
			nonvisitees[indiceduplusgrand] = 0;
			for (int j = 0; j < this.nbVilles; j++)
			{
				if (nonvisitees[j] != 0) { nonvisitees[j] = this.arretes[indiceduplusgrand][j]; }
			}
		}
		
		if (this.distance_meilleure == lg_parcours(this.meilleur)) { this.cpt++; }
		else { this.cpt = 0; }
		this.distance_meilleure = lg_parcours(this.meilleur);
		if (this.distance_meilleure < this.vraie_distance_meilleure) { this.vrai_meilleur = this.meilleur; this.vraie_distance_meilleure = this.distance_meilleure; }
		if (pverbeux > -1)
		{
			System.out.println("-----------------------");
			System.out.println("Meilleur chemin pour le cycle " + this.cpt + " : distance = " + this.distance_meilleure);
			System.out.println("Meilleur chemin enregistré : distance = " + lg_parcours(this.vrai_meilleur));
			System.out.println("-----------------------");
		}
		
		return this.cpt;
	}
	
	public Ville getMeilleur(int i)
	{
		return this.villes[this.vrai_meilleur[i]];
	}
}
