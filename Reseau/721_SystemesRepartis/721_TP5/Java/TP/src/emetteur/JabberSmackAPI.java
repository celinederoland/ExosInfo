package emetteur;

import java.util.*;
import java.io.*;

import org.jivesoftware.smack.Chat;
import org.jivesoftware.smack.ConnectionConfiguration;
import org.jivesoftware.smack.MessageListener;
import org.jivesoftware.smack.Roster;
import org.jivesoftware.smack.RosterEntry;
import org.jivesoftware.smack.RosterListener;
import org.jivesoftware.smack.XMPPConnection;
import org.jivesoftware.smack.XMPPException;
import org.jivesoftware.smack.packet.Message;
import org.jivesoftware.smack.packet.Presence;
import org.jivesoftware.smack.packet.Presence.Mode;

public class JabberSmackAPI implements MessageListener {

	static XMPPConnection connection;

	public void login(String userName, String password) throws XMPPException {
		int port = 5222;
		String host = "talk.google.com";
		ConnectionConfiguration config = new ConnectionConfiguration(host,
				port, "work");
		connection = new XMPPConnection(config);

		connection.connect();
		connection.login("deroland.celine.info@gmail.com", "bidonbidon");
		

	}

	public void sendMessage(String message, String to) throws XMPPException {
		Chat chat = connection.getChatManager().createChat(to, this);
		chat.sendMessage(message);
	}

	public void displayBuddyList() {
		Roster roster = connection.getRoster();

		roster.addRosterListener(new RosterListener() {
            // Ignored events public void entriesAdded(Collection<String> addresses) {}
            public void entriesDeleted(Collection<String> addresses) {}
            public void entriesUpdated(Collection<String> addresses) {}
            public void presenceChanged(Presence presence) {
                System.out.println("Presence changed: " + presence.getFrom() + " " + presence);
            }
            @Override
            public void entriesAdded(Collection<String> arg0) {
                // TODO Auto-generated method stub

            }
        });

		Presence p = roster.getPresence("deroland.celine.info@gmail.com");
		p.setPriority(42);
		p.setMode(Mode.dnd);
		connection.sendPacket(p);
		
		Collection<RosterEntry> entries = roster.getEntries();
		/*for (RosterEntry entry : entries) {
            System.out.println(entry);
        }*/

		System.out.println("\n\n" + entries.size() + " buddy(ies):");
		for (RosterEntry r : entries) {
			System.out.println(r.getName() + " : " + r.getUser() + " " + roster.getPresence(r.getUser()));
		}
	}

	public void disconnect() {
		connection.disconnect();
	}

	public void processMessage(Chat chat, Message message) {
		if (message.getType() == Message.Type.chat)
			System.out.println(chat.getParticipant() + " says: "
					+ message.getBody());
	}

	public static void main(String args[]) throws XMPPException, IOException, InterruptedException {
		// declare variables
		JabberSmackAPI c = new JabberSmackAPI();
		BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
		String msg;

		// turn on the enhanced debugger
		XMPPConnection.DEBUG_ENABLED = true;

		// Enter your login information here
		c.login("userName", "password");

		Thread.sleep(1000);

		Roster roster = connection.getRoster();

		roster.addRosterListener(new RosterListener() {
            // Ignored events public void entriesAdded(Collection<String> addresses) {}
            public void entriesDeleted(Collection<String> addresses) {}
            public void entriesUpdated(Collection<String> addresses) {}
            public void presenceChanged(Presence presence) {
                System.out.println("Presence changed: " + presence.getFrom() + " " + presence);
            }
            @Override
            public void entriesAdded(Collection<String> arg0) {
                // TODO Auto-generated method stub

            }
        });

		c.displayBuddyList();

		System.out.println("-----");

		System.out
		.println("Avec qui souhaitez-vous échanger ? - Tapez l'adresse de contact");
		String talkTo = br.readLine();

		System.out.println("Entrez votre message:");
		System.out.println("-----\n");

		while (!(msg = br.readLine()).equals("bye")) {
			c.sendMessage(msg, talkTo);
		}

		c.disconnect();
		System.exit(0);
	}

}