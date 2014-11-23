package ihm;

import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;

import Modele.Ville;

/**
 * Classe P_Inferieur : boutons et champs de contrôle de l'application
 * 
 * @author Céline de Roland
 * @author Mickaël Koza
 * @version 1
 * 
 *
 */
public class P_Inferieur extends JPanel {
	

	private static final long serialVersionUID = -2191583589584322901L;
	private JTextField jtf_nomVille = new JTextField("Nom ville");
	private JLabel lbl_nomVille = new JLabel("Nom de la ville");
	private JTextField jtf_x = new JTextField("100");
	private JTextField jtf_y = new JTextField("100");	
	private JLabel lbl_coordonnees = new JLabel("Coordonnées");
	private JButton bt_ajout = new JButton("Ajouter");
	private JLabel lbl_fourmis = new JLabel("Nombre de fourmis");
	private JTextField jtf_fourmis = new JTextField("20");
	private JButton bt_fourmis = new JButton("Démarrer");
	private JLabel lbl_verbeux = new JLabel("détail des commentaires (-1 à 2)");
	private JTextField jtf_verbeux = new JTextField("-1");
	private JButton bt_lancer_tout = new JButton("Tous les cycles");
	private JPanel pan_ajout = new JPanel();
	private JPanel pan_animer = new JPanel();
	private JPanel pan_fourmis = new JPanel();
	
	public P_Inferieur()
	{
		this.setLayout(new BoxLayout(this,BoxLayout.PAGE_AXIS));
		this.pan_ajout.add(lbl_nomVille);
		this.pan_ajout.add(jtf_nomVille);
		this.pan_ajout.add(lbl_coordonnees);
		this.pan_ajout.add(jtf_x);
		this.pan_ajout.add(jtf_y);
		this.pan_ajout.add(bt_ajout);
		this.pan_fourmis.add(lbl_fourmis);
		this.pan_fourmis.add(jtf_fourmis);
		this.pan_fourmis.add(bt_fourmis);
		this.pan_animer.add(lbl_verbeux);
		this.pan_animer.add(jtf_verbeux);
		this.pan_animer.add(bt_lancer_tout);
		this.add(pan_ajout);
		this.add(pan_fourmis);
		this.add(pan_animer);
	}
	 
	/**
	 * 
	 * Renvoie le bouton d'ajout d'une ville au contrôleur p_principal pour qu'il l'écoute
	 * 
	 * @return JButton
	 */
	public JButton getBt_ajout()
	{
		return this.bt_ajout;
	}
	
	/**
	 * 
	 * Renvoie le bouton d'ajout de fourmis au contrôleur p_principal pour qu'il l'écoute
	 * 
	 * @return JButton
	 */
	public JButton getBt_fourmis()
	{
		return this.bt_fourmis;
	}
	
	/**
	 * 
	 * Renvoie le bouton de lancement de tous les cycles au contrôleur p_principal pour qu'il l'écoute
	 * 
	 * @return JButton
	 */
	public JButton getBt_tous_cycle()
	{
		return this.bt_lancer_tout;
	}
	
	/**
	 * 
	 * Renvoie une ville au contrôleur pour qu'il le repasse à la figure et au modèle
	 * 
	 * @return Ville
	 */
	public Ville getVille()
	{
		int x = Integer.parseInt(jtf_x.getText());
		int y = Integer.parseInt(jtf_y.getText());
		String nom = jtf_nomVille.getText();
		return new Ville(x,y,nom);
	}
	
	public int getFourmis()
	{
		return Integer.parseInt(jtf_fourmis.getText());
	}
	
	public int getVerbeux()
	{
		return Integer.parseInt(jtf_verbeux.getText());
	}
}

