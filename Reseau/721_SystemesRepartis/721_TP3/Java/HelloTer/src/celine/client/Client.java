package celine.client;

import java.rmi.Remote;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import celine.server.Hello;

public class Client {

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
            Registry registry = LocateRegistry.getRegistry();
            Hello r = (Hello) registry.lookup(name);
            String message = r.sayHello();
            System.out.println(message);
            System.out.println(r);
        } catch (Exception e) {
            System.err.println("Client exception:");
            e.printStackTrace();
        }
	}

}
