package producteur;

public class TitreBoursier {

	private String nom;
	private double cours;
	private double variation;
	
	public TitreBoursier(String pnom)
	{
		this.nom = pnom;
		this.cours = 50*Math.random();
	}
	
	public void update() {
		this.variation = 10*(Math.random() - 0.5);
		this.cours = this.cours + this.variation;
	}
	
	public void setNom(String pnom) {
		this.nom = pnom;
	}
	
	public String getNom() {
		return this.nom;
	}
	
	public double getCours() {
		return this.cours;
	}
	
	public double getVariation() {
		return this.variation;
	}
 }
