package calculator;
// pour le client
import java.net.MalformedURLException;
import java.net.URL;
import org.apache.xmlrpc.XmlRpcException;
import org.apache.xmlrpc.client.XmlRpcClient;
import org.apache.xmlrpc.client.XmlRpcClientConfigImpl;
import org.apache.xmlrpc.client.XmlRpcCommonsTransportFactory;
import org.apache.xmlrpc.client.util.ClientFactory;

// pour l'interface graphique
import java.awt.BorderLayout;
import java.awt.GridLayout;
import java.awt.TextField;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.*;


/**
 * La classe <code>Calculatrice</code> est l'interface graphique d'une
 * calculatrice simple. Le calcul des résultats sera effectué par un moteur de
 * calcul séparé spécifié par l'interface <code>IMoteurCalcul</code>.
 * 
 */

// fenetre qui gere elle meme ses evenements
public class Calculatrice extends JFrame implements ActionListener {
	private String ip = "192.168.136.200";//"localhost";
	private String port = "8080";//"2009";
	private String nom_methode = "MoteurCalculSimple";//"Moteur";
	
	private XmlRpcClient client = new XmlRpcClient();
	
	// tableau de boutons
	private JButton[] buttons;

	// ecran de la calculatrice
	private TextField ecran;

	// tableau des legendes des boutons
	private static String[] labels = { "(", ")", "EFF", "OFF", "7", "8", "9",
			"*", "4", "5", "6", "/", "1", "2", "3", "-", "0", ".", "=", "+" };

	// pour savoir s'il faut effacer l'ecran ou non
	boolean estCalcule = false;

	// constructeur
	public Calculatrice() {
		// la fenetre a pour titre "Calculatrice"
		super("Calculatrice");
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		XmlRpcClientConfigImpl config = new XmlRpcClientConfigImpl();
		try {
			config.setServerURL(new URL("http://"+ip+":"+port+"/xmlrpc"));
		} catch (MalformedURLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		client.setConfig(config);
		
		setLayout(new BorderLayout());
		// l'ecran occupe le haut de la fenetre
		ecran = new TextField("");
		add(BorderLayout.NORTH, ecran);

		// creation du clavier
		JPanel clavier = new JPanel();
		clavier.setLayout(new GridLayout(5, 4));

		// on cree chaque bouton
		buttons = new JButton[labels.length];
		for (int i = 0; i < labels.length; i++) {
			buttons[i] = new JButton(labels[i]);
			clavier.add(buttons[i]);
			buttons[i].addActionListener(this);
		}

		// ajout du clavier au centre de la fenetre
		add(BorderLayout.CENTER, clavier);
		pack();
		setVisible(true);
	}

	// gestion des clics sur bouton
	public void actionPerformed(ActionEvent e) {
		// recuperation de la commande associee
		// (i.e. la legende du bouton)
		String s = e.getActionCommand();
		// chaque commande est identifiable par son premier caractere
		char c = s.charAt(0);
		switch (c) {
		// effacement de l'ecran
		case 'E':
			ecran.setText("");
			break;
		// on quitte
		case 'O':
			System.exit(0);
		// calcul et affichage de l'expression
		case '=':
			try {
				Object[] params = new Object[]{new String(ecran.getText())};

				Double resultat = 0.0;
				try {
					resultat = (Double) client.execute(nom_methode + ".calculer",params);
				} catch (XmlRpcException xe) {
					// TODO Auto-generated catch block
					xe.printStackTrace();
				}
				
				//Double resultat = moteurCalcul.calculer(ecran.getText());
				// affichage du resultat
				ecran.setText(resultat.toString());
				// il faudra effacer l'ecran
				// lors de la prochaine frappe
				estCalcule = true;
			}
			// plantage du programme
			catch (Exception ex) {
				JOptionPane.showMessageDialog(this,
						"<html>Erreur interne :<br>" + ex.getMessage()
								+ "<html>");
				ecran.selectAll();
			}
			break;
		// toutes les autres touches : ajout du caractere correspondant
		default:
			if (estCalcule)
				ecran.setText("");
			StringBuffer temp = new StringBuffer(ecran.getText());
			temp.append(c);
			ecran.setText(new String(temp));
			estCalcule = false;
			break;
		}
	}
}