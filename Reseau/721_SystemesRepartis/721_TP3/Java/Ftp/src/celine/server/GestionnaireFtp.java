package celine.server;

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface GestionnaireFtp extends Remote {

	public String sayHello() throws RemoteException;
	public byte[] get(String filename) throws RemoteException;
	public void put(String filename, byte[] bArray) throws RemoteException;
}
