package celine.server;

import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;

public class HelloImpl implements Hello {

	public HelloImpl() { super(); }
	
	public String sayHello() {
		return "Hello the World";
	}
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
	    /*if (System.getSecurityManager() == null) {
            System.setSecurityManager(new SecurityManager());
        }*/
        try {
            String name = "Hello";
            Hello engine = new HelloImpl();
            Hello stub =
                (Hello) UnicastRemoteObject.exportObject(engine, 0);
            Registry registry = LocateRegistry.createRegistry(1099);
            registry.rebind(name, stub);
            System.out.println("HelloImpl enregistré");
        } catch (Exception e) {
            System.err.println("HelloImpl erreur :");
            e.printStackTrace();
        }
	}

}
