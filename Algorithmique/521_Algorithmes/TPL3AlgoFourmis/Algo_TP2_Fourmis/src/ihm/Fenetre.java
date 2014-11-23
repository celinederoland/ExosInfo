package ihm;

import javax.swing.JFrame;

public class Fenetre extends JFrame
{
	
	public Fenetre(Panneau pan)
	{
		this.setTitle("Algorithme des fourmis");
		this.setSize(500, 500);
		this.setLocationRelativeTo(null);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);  
		
	    this.setContentPane(pan);            
	    
		this.setVisible(true);
	}
}
