package calculator;
import org.apache.xmlrpc.webserver.WebServer;
import java.net.InetAddress;
import org.apache.xmlrpc.common.TypeConverterFactoryImpl;
import org.apache.xmlrpc.server.PropertyHandlerMapping;
import org.apache.xmlrpc.server.XmlRpcServer;
import org.apache.xmlrpc.server.XmlRpcServerConfigImpl;

public class Serveur {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		try {
			// instancie le serveur
			WebServer server = new WebServer(2009);
			XmlRpcServer xmlRpcServer = server.getXmlRpcServer();
			
			// enregistre la classe du gestionnaire
			PropertyHandlerMapping phm = new PropertyHandlerMapping();
			phm.addHandler("Moteur", MoteurCalculSimple.class);
			xmlRpcServer.setHandlerMapping(phm);
			
			// démarre le serveur
			server.start();
			System.out.println("Serveur en attente...");
		} catch (Exception ex) {
			ex.printStackTrace();
		}
	}

}
