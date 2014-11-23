package producteur;

import java.util.ArrayList;
import java.util.Hashtable;
import java.util.List;

import javax.naming.Context;
import javax.naming.InitialContext;
import javax.naming.NamingException;

import javax.jms.JMSException;
import javax.jms.Destination;
import javax.jms.Connection;
import javax.jms.ConnectionFactory;
import javax.jms.MessageProducer;
import javax.jms.Session;
import javax.jms.TextMessage;

public class Producteur {

	public static void main(String[] args) {
		List<TitreBoursier> listeDesCours = new ArrayList<TitreBoursier> ();
		listeDesCours.add(new TitreBoursier("GoogleInc"));
		listeDesCours.add(new TitreBoursier("EDF"));
		listeDesCours.add(new TitreBoursier("Apple"));
		listeDesCours.add(new TitreBoursier("IBM"));
		listeDesCours.add(new TitreBoursier("Kiabi"));

		
		Context context = null;
		ConnectionFactory factory = null;
		Connection connection = null;
		String factoryName = "ConnectionFactory";
		Session session = null;
		MessageProducer sender = null;

		try {
			// create the JNDI initial context.
			context = new InitialContext();

			// look up the ConnectionFactory
			factory = (ConnectionFactory) context.lookup(factoryName);


			// create the connection
			connection = factory.createConnection();

			// create the session
			session = connection.createSession(
					false, Session.AUTO_ACKNOWLEDGE);

			// create the sender and lookup the destination
			Hashtable<TitreBoursier,MessageProducer> senderTab = new Hashtable<TitreBoursier,MessageProducer>();
			Hashtable<TitreBoursier,Destination> destTab = new Hashtable<TitreBoursier,Destination>();
			for (TitreBoursier titre : listeDesCours) {
				destTab.put(titre, (Destination) context.lookup(titre.getNom()));
				senderTab.put(titre, session.createProducer(destTab.get(titre)));
			}

			// start the connection, to enable message sends
			connection.start();

			while (true) {
				for (TitreBoursier titre : listeDesCours) {
					titre.update();
					TextMessage message = session.createTextMessage();
					String mess = "titre : " + titre.getNom() + " ; cours : " + titre.getCours() + " ; variation : " + titre.getVariation();
					message.setText(mess);
					
					sender = senderTab.get(titre);
					sender.send(message);
					System.out.println("Sent: " + message.getText());
				}
				try {
					Thread.sleep(2000);
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		} catch (JMSException exception) {
			exception.printStackTrace();
		} catch (NamingException exception) {
			exception.printStackTrace();
		} finally {
			// close the context
			if (context != null) {
				try {
					context.close();
				} catch (NamingException exception) {
					exception.printStackTrace();
				}
			}

			// close the connection
			if (connection != null) {
				try {
					connection.close();
				} catch (JMSException exception) {
					exception.printStackTrace();
				}
			}
		}
	}

}

