package ihm;
import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.JPanel;
 
/**
 * Classe Panneau : contrôleur de l'application
 * 
 * @author Céline de Roland
 * @author Mickaël Koza
 * @version 1
 * 
 *
 */
public class Panneau extends JPanel {      
  
	private static final long serialVersionUID = -4241895702570813689L;

	private P_Superieur p_titre = new P_Superieur();
	private P_Central p_figure = new P_Central();
	private P_Inferieur p_controle = new P_Inferieur();
	
	/**
	 * 
	 * Le panneau principal contient 3 panneaux :
	 * p_titre : affiche un titre
	 * p_figure : affiche le graphe des villes
	 * p_controle : affiche les boutons pour ajouter une ville ou lancer l'algorithme des fourmis
	 * 
	 */
	public Panneau()
	{ 
		this.setLayout(new BorderLayout());
		this.add(p_titre,BorderLayout.NORTH);
		this.add(p_figure,BorderLayout.CENTER);
		this.add(p_controle,BorderLayout.SOUTH);
		this.p_controle.getBt_ajout().addActionListener(new AjoutVilleListener());
		this.p_controle.getBt_fourmis().addActionListener(new FourmisListener());
		this.p_controle.getBt_tous_cycle().addActionListener(new LancerToutListener());
		
		/*p_figure.setCoord(100,100,"A");
		p_figure.setCoord(100,300,"B");
		p_figure.setCoord(300,100,"C");
		p_figure.setCoord(300,300,"D");
		p_figure.setCoord(10,400,"E");
		p_figure.setCoord(129,170,"F");
		p_figure.setCoord(250,500,"G");
		p_figure.setCoord(200,30,"H");
		p_figure.setCoord(200,200,"I");
		p_figure.setCoord(150,160,"J");
		p_figure.setCoord(20,20,"K");
		p_figure.setCoord(600,300,"L");
		p_figure.setCoord(138,212,"M");
		p_figure.setCoord(100,260,"N");
		p_figure.setCoord(600,10,"O");*/
		
		p_figure.setCoord(312,150,"Rumilly");
		p_figure.setCoord(327,138,"Sales");
		p_figure.setCoord(312,62,"Vallières");
		p_figure.setCoord(290,138,"Moye");
		p_figure.setCoord(350,138,"Marcellaz");
		p_figure.setCoord(340,175,"Boussy");
		p_figure.setCoord(335,62,"Hauteville/Fier");
		p_figure.setCoord(340,225,"Marigny");
		p_figure.setCoord(350,255,"Alby");
		p_figure.setCoord(290,225,"Massingy");
		p_figure.setCoord(550,175,"Vieugy");
		p_figure.setCoord(328,280,"St-Felix");
		p_figure.setCoord(150,280,"Cessens");
		p_figure.setCoord(285,35,"Lornay");
		p_figure.setCoord(75,138,"Serrière");
		p_figure.setCoord(525,35,"Poisy");
		p_figure.setCoord(625,300,"Semnoz");
		
		p_figure.setCoord(312,70,"Paris");
		p_figure.setCoord(400,185,"Lyon");
		p_figure.setCoord(410,180,"Grenoble");
		p_figure.setCoord(200,225,"Bordeaux");
		p_figure.setCoord(300,250,"Toulouse");
		p_figure.setCoord(325,250,"Montpellier");
		p_figure.setCoord(405,255,"Marseille");
		p_figure.setCoord(500,300,"Ajaccio");
		p_figure.setCoord(300,175,"Limoges");
		p_figure.setCoord(400,115,"Dijon");
		p_figure.setCoord(185,110,"Nantes");
		p_figure.setCoord(90,75,"Brest");
		p_figure.setCoord(335,10,"Lille");
		p_figure.setCoord(250,48,"LeHavre");
		p_figure.setCoord(360,55,"Reims");
		p_figure.setCoord(490,70,"Strasbourg");
	}
	
	/**
	 * 
	 * Envoie une nouvelle ville au panneau p_figure pour affichage
	 *
	 */
	class AjoutVilleListener implements ActionListener 
	{
	    public void actionPerformed(ActionEvent arg0) 
	    {
			p_figure.setCoord(p_controle.getVille().getPosX(),p_controle.getVille().getPosY(),p_controle.getVille().getNom());        
	    }
	}
	
	/**
	 * 
	 * Appelle la fonction cycle (qui n'est pas encore faite)
	 *
	 */
	class FourmisListener implements ActionListener
	{
		public void actionPerformed(ActionEvent arg0) 
		{  
			p_figure.ajoutFourmis(p_controle.getFourmis()); 
		}
	}      
	
	/**
	 * 
	 * Appelle la fonction trouver_chemin (qui n'est pas encore faite)
	 *
	 */
	class LancerToutListener implements ActionListener
	{
		public void actionPerformed(ActionEvent arg0) 
		{
			p_figure.demarrer(p_controle.getVerbeux());
		}
	}    
}