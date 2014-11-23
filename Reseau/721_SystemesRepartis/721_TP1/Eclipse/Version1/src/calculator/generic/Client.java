package calculator.generic;
/*import java.net.Socket;
import java.net.InetAddress;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.UnknownHostException;
import java.util.Scanner;*/


public class Client {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		
		Calculatrice ihm = new Calculatrice(null);
	
		//Ce code réalise le calcul en mode console. 
		//Vous pouvez le décommenter pour taper les expressions en console et obtenir les résultats en console
		/*try {
			
			//On crée un socket
	        Socket socket = new Socket(InetAddress.getLocalHost(),2000);
	        
	        Scanner sc = new Scanner(System.in); //to be removed (pour essai en console)

	        //Un buffer pour la lecture
	        BufferedReader plec = new BufferedReader(
	                               new InputStreamReader(socket.getInputStream())
	                               );

	        //Un buffer pour l'écriture
	        PrintWriter pred = new PrintWriter(
	                                new OutputStreamWriter(socket.getOutputStream()),
	                             true);

	        //On attend les expressions de l'utilisateurs (en console pour le moment)
	        String exp = "";
	        while(exp != "end")
	        {
	        	
	        	//Il faut récupérer l'expression dans l'IHM
	        	//TODO
	        	exp = sc.nextLine(); //to be removed
	        	
	        	
	        	//On écrit l'expression dans le buffer d'écriture
	        	pred.println(exp);
	        	
	        	//On lit la réponse du serveur dans le buffer de lecture
	        	String str = plec.readLine();
	        	
	        	
	        	//Il faut afficher la réponse dans l'IHM
	        	//TODO
	        	System.out.println(str); //to be removed
	        	
	        }
	        
	        //On ferme les buffers et le socket
	        plec.close();
	        pred.close();
	        socket.close();
	        
		}
		catch (UnknownHostException e) {	
			e.printStackTrace();
		}
		catch (IOException e) {
			e.printStackTrace();
		}*/
		

	}

}
