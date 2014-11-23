package consommateur;

import java.util.ArrayList;
import java.util.List;

import javax.naming.Context;
import javax.naming.InitialContext;
import javax.naming.NamingException;
import javax.jms.ConnectionFactory;
import javax.jms.Connection;
import javax.jms.Topic;
import javax.jms.TopicSubscriber;
import javax.jms.Session;
import javax.jms.Message;
import javax.jms.TextMessage;
import javax.jms.JMSException;

public class Consommateur {


	public static void main(String[] args) {
		// TODO Auto-generated method stub
        Context context = null;
        ConnectionFactory factory = null;
        Connection connection = null;
        String factoryName = "ConnectionFactory";
        String[] topicNames;
        List<Topic> topics = new ArrayList<Topic> ();
        Session session = null;
        List<TopicSubscriber> subscribers = new ArrayList<TopicSubscriber> ();
        String subscriptionName = "nomccc";

        if (args.length < 1) {
            System.out.println("usage: DurableSubscriber <topic> <topic> ...");
            System.exit(1);
        }

        topicNames = args;

        try {
            // create the JNDI initial context.
            context = new InitialContext();

            // look up the ConnectionFactory
            factory = (ConnectionFactory) context.lookup(factoryName);
            
            // create the connection
            connection = factory.createConnection();

            // create the session
            session = connection.createSession(
                false, Session.AUTO_ACKNOWLEDGE);

            // look up the topic
            for (String topicName : topicNames) {
            	Topic topic = (Topic) context.lookup(topicName);
                topics.add(topic);
                // create the durable subscriber
                subscribers.add(session.createDurableSubscriber(
                    topic, subscriptionName + topicName));
            }



            // start the connection, to enable message receipt
            connection.start();

            while (true) {
            	for (TopicSubscriber subscriber : subscribers) {
	                Message message = subscriber.receive();
	                if (message instanceof TextMessage) {
	                    TextMessage text = (TextMessage) message;
	                    System.out.println("Received: " + text.getText());
	                } else if (message != null) {
	                    System.out.println("Received non text message");
	                }
            	}
            }
        } catch (JMSException exception) {
            exception.printStackTrace();
        } catch (NamingException exception) {
            exception.printStackTrace();
        } finally {
            // close the context
            if (context != null) {
                try {
                    context.close();
                } catch (NamingException exception) {
                    exception.printStackTrace();
                }
            }

            // close the connection
            if (connection != null) {
                try {
                    connection.close();
                } catch (JMSException exception) {
                    exception.printStackTrace();
                }
            }
        }
	}
}
