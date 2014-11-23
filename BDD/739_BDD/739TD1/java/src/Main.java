import java.sql.Connection;
import java.sql.DriverManager;
import java.util.Enumeration;
import java.util.Properties;

public class Main {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		System.out.println("Biblio708");
		try {
			Class.forName("org.postgresql.Driver");
			System.out.println("DRIVER OK ! ");
			
			String url = "jdbc:postgresql://localhost:5432/celine";
			String user = "celine";
			String passwd = "bidon";
			
			Connection conn = DriverManager.getConnection(url, user, passwd);
			System.out.println("Connection effective !");			
			
		} catch (Exception e) {
			e.printStackTrace();
		}		
		
		
		while (true)
		{
			System.out.println("1 : création d'un utilisateur");
			
		}
	}

}
