package ihm;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import javax.swing.JPanel;

/**
 * Classe P_Superieur : simple affichage du titre de l'application : algorithme des fourmis
 * 
 * @author Céline de Roland
 * @author Mickaël Koza
 * @version 1
 * 
 *
 */
public class P_Superieur extends JPanel {
	
	private static final long serialVersionUID = -9007501945342620581L;

	public P_Superieur()
	{
		this.setPreferredSize(new Dimension(500, 30));;
	}
	
	public void paintComponent(Graphics g)
	{
	    g.setColor(Color.BLACK);
	    g.drawString("Algorithme des fourmis",10,20);

	}   
}
