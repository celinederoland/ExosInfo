package ihm;

/**
 * Classe ihm
 * Méthode main, point d'entrée du programme
 * Crée une fenêtre, contenant un panneau contenant lui même 3 panneaux
 * Le panneau p_principal contrôle l'ensemble de l'application par ses implémentations d'ActionListener
 * 
 * @author Céline de Roland
 * @author Mickaël Koza
 * @version 1
 * @param args : arguments d'éxécution, inutile
 * 
 */
public class ihm {

	
	public static void main(String[] args) {
	    Panneau p_principal = new Panneau();
		Fenetre ma_fenetre = new Fenetre(p_principal);
	}

}
