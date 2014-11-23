package calculator.generic;
import java.net.ServerSocket;
import java.net.Socket;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;



public class Serveur {

	/**
	 * @param args
	 * @throws IOException 
	 * @throws ExpressionInvalide 
	 */
	public static void main(String[] args) throws IOException, ExpressionInvalide {
		ServerSocket SS = new ServerSocket(2000);
		Socket S = SS.accept();

		while(true)
		{
			//Récupération de l'expression du client
			InputStreamReader ISR = new InputStreamReader (S.getInputStream());
			BufferedReader BR = new BufferedReader(ISR);
			String expression_client = BR.readLine();
			System.out.println("expression reçue");
			System.out.println(expression_client);

			//Calcul de l'expression reçue par le client
			MoteurCalculSimple MCS = new MoteurCalculSimple();
			double Resultat = MCS.calculer(expression_client);

			//Renvoi du résultat
			PrintWriter PW = new PrintWriter(S.getOutputStream());
			PW.println(Resultat);
			PW.flush();
		}

	}

}
