package celine.server;

import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;
import java.io.IOException;


public class GestionnaireFtpImpl implements GestionnaireFtp {

	public GestionnaireFtpImpl()
	{
		super();
	}
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
        try {
            String name = "GestionnaireFtp";
            GestionnaireFtp engine = new GestionnaireFtpImpl();
            GestionnaireFtp stub =
                (GestionnaireFtp) UnicastRemoteObject.exportObject(engine, 0);
            Registry registry = LocateRegistry.createRegistry(1099);
            registry.rebind(name, stub);
            System.out.println("GestionnaireFtp enregistré");
        } catch (Exception e) {
            System.err.println("GestionnaireFtp erreur :");
            e.printStackTrace();
        }
	}

	@Override
	public byte[] get(String filename) throws RemoteException {
		// TODO Auto-generated method stub
		Path fileIn = Paths.get("/home/celine/Documents/ProjetsInfo/721_TP3/DepotFTP", filename);
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

	@Override
	public void put(String filename, byte[] bArray) throws RemoteException {
		// TODO Auto-generated method stub
		Path fileOut = Paths.get("/home/celine/Documents/ProjetsInfo/721_TP3/DepotFTP", filename);
		try {
			Files.write(fileOut, bArray);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	@Override
	public String sayHello() throws RemoteException {
		// TODO Auto-generated method stub
		return "Gestionnaire Ftp prêt";
	}

}
