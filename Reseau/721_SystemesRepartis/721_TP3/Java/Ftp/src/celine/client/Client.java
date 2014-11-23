package celine.client;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;

import celine.server.GestionnaireFtp;

public class Client {

	
	public static byte[] getOnClient(String filename) throws RemoteException {
		// TODO Auto-generated method stub
		Path fileIn = Paths.get("/home/celine/Documents/ProjetsInfo/721_TP3/RepClient", filename);
		byte[] tab;
		try {
			tab = Files.readAllBytes(fileIn);
			return tab;
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			return null;
		}
	}

	public static void putOnClient(String filename, byte[] bArray) throws RemoteException {
		// TODO Auto-generated method stub
		Path fileOut = Paths.get("/home/celine/Documents/ProjetsInfo/721_TP3/RepClient", filename);
		try {
			Files.write(fileOut, bArray);
		} catch (IOException e) {
			// TODO Auto-generated catch blockDepotFTP
			e.printStackTrace();
		}
	}
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
        try {
            String name = "GestionnaireFtp";
            Registry registry = LocateRegistry.getRegistry();
            GestionnaireFtp gftp = (GestionnaireFtp) registry.lookup(name);
            String message = gftp.sayHello();
            System.out.println(message);
            
            File f = new File("/home/celine/Documents/ProjetsInfo/721_TP3/RepClient/test01.txt");
            System.out.println("Chemin absolu du fichier : " + f.getAbsolutePath());
            System.out.println("Nom du fichier : " + f.getName());
            System.out.println("Est-ce qu'il existe ? " + f.exists());
            System.out.println("Est-ce un répertoire ? " + f.isDirectory());
            System.out.println("Est-ce un fichier ? " + f.isFile());
            
    	    byte[] bArray = getOnClient("test01.txt");
    	    gftp.put("test01.txt", bArray);
    	    
        } catch (Exception e) {
            System.err.println("Client exception:");
            e.printStackTrace();
        }
	}

}
