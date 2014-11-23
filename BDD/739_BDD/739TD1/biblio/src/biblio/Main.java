/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package biblio;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.Scanner;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.sql.PreparedStatement;
import java.sql.ResultSetMetaData;

/**
 *
 * @author celine
 */
public class Main {

    private static Scanner sc = new Scanner(System.in);
    private static PreparedStatement pst = null;
    private static Connection con = null;
    private static Statement st = null;
    private static ResultSet rs = null;

    public static void initcon()
    {
        String url = "jdbc:postgresql://localhost/biblio";
        String user = "celine";
        String password = "bidon";

        try {
            con = DriverManager.getConnection(url, user, password);
            st = con.createStatement();
            rs = st.executeQuery("SELECT VERSION()");

            if (rs.next()) {
                System.out.println(rs.getString(1));
            }

        } catch (SQLException ex) {
            Logger lgr = Logger.getLogger(Main.class.getName());
            lgr.log(Level.SEVERE, ex.getMessage(), ex);

        }

        System.out.println("Début du programme");
    }
    
    
    public static void afficheMenu() {
        System.out.println("0 : quitter");
        System.out.println("1 : ajouter des livres");
        System.out.println("2 : voir les livres");
        System.out.println("3 : ajouter des lecteurs");
        System.out.println("4 : voir les lecteurs");
        System.out.println("5 : emprunter un livre");

    }
    
    public static void ajoutLivre() throws SQLException {
        System.out.println("Entrez l'id");
        int id = sc.nextInt();
        sc.nextLine();
        System.out.println("Entrez le titre");
        String titre = sc.nextLine();
        System.out.println("Entrez l'auteur");
        String auteur = sc.nextLine();

        String requete = "INSERT INTO \"Ouvrage\"(id,titre, auteur) VALUES(?,?, ?)";
        pst = con.prepareStatement(requete);
        pst.setInt(1, id);
        pst.setString(2, titre);
        pst.setString(3, auteur);
        pst.executeUpdate();
    }
    
    public static void ajoutLecteur() throws SQLException {
        System.out.println("Entrez l'id");
        int id = sc.nextInt();
        sc.nextLine();
        System.out.println("Entrez le nom");
        String nom = sc.nextLine();
        System.out.println("Entrez l'âge");
        int age = sc.nextInt();
        sc.nextLine();

        String requete = "INSERT INTO \"Lecteur\"(id_lecteur,nom, age) VALUES(?,?, ?)";
        pst = con.prepareStatement(requete);
        pst.setInt(1, id);
        pst.setString(2, nom);
        pst.setInt(3, age);
        pst.executeUpdate();
    }
        
    public static void voirLivres() throws SQLException {
        //L'objet ResultSet contient le résultat de la requête SQL
        try (Statement state = con.createStatement()) {
            //L'objet ResultSet contient le résultat de la requête SQL
            ResultSet result = state.executeQuery("SELECT * FROM \"Ouvrage\"");
            //On récupère les MetaData
            ResultSetMetaData resultMeta = result.getMetaData();

            System.out.println("\n**********************************");
            //On affiche le nom des colonnes
            for (int i = 1; i <= resultMeta.getColumnCount(); i++) {
                System.out.print("\t" + resultMeta.getColumnName(i).toUpperCase() + "\t *");
            }

            System.out.println("\n**********************************");

            while (result.next()) {
                for (int i = 1; i <= resultMeta.getColumnCount(); i++) {
                    if (result.getObject(i) != null) System.out.print("\t" + result.getObject(i).toString() + "\t |");
                }

                System.out.println("\n---------------------------------");

            }

            result.close();
        }
    }
    
        public static void voirLecteurs() throws SQLException {
        //L'objet ResultSet contient le résultat de la requête SQL
        try (Statement state = con.createStatement()) {
            //L'objet ResultSet contient le résultat de la requête SQL
            ResultSet result = state.executeQuery("SELECT * FROM \"Lecteur\"");
            //On récupère les MetaData
            ResultSetMetaData resultMeta = result.getMetaData();

            System.out.println("\n**********************************");
            //On affiche le nom des colonnes
            for (int i = 1; i <= resultMeta.getColumnCount(); i++) {
                System.out.print("\t" + resultMeta.getColumnName(i).toUpperCase() + "\t *");
            }

            System.out.println("\n**********************************");

            while (result.next()) {
                for (int i = 1; i <= resultMeta.getColumnCount(); i++) {
                    if (result.getObject(i) != null) System.out.print("\t" + result.getObject(i).toString() + "\t |");
                }

                System.out.println("\n---------------------------------");

            }

            result.close();
        }
    }
        
    public static void emprunter() throws SQLException {
        System.out.println("nom de l'emprunteur");
        String nom = sc.nextLine();
        System.out.println("titre du livre");
        String titre = sc.nextLine();
        
        
        String requeteLec = "SELECT id_lecteur FROM \"Lecteur\" WHERE nom = ?";
        pst = con.prepareStatement(requeteLec);
        pst.setString(1, nom);
        ResultSet result =  pst.executeQuery();
        int idLec = 0;
        while (result.next()) { idLec = result.getInt("id_lecteur");}
        System.out.println("id emprunteur : " + idLec);
        
        String requeteOuv = "UPDATE \"Ouvrage\" SET id_emprunteur = ? WHERE titre = ?";
        pst = con.prepareStatement(requeteOuv);
        pst.setInt(1, idLec);
        pst.setString(2, titre);
        pst.executeUpdate();

    }
    
    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) throws SQLException {

        initcon();
        
        afficheMenu();

        int choix = sc.nextInt(); sc.nextLine();
        while (choix != 0) {
            if (choix == 1) {
                ajoutLivre();
            } else if (choix == 2) {
                voirLivres();
            } else if (choix == 3) {
                ajoutLecteur();
            } else if (choix == 4) {
                voirLecteurs();
            } else if (choix == 5) {
                emprunter();
            }

            afficheMenu();


            choix = sc.nextInt();
            sc.nextLine();
        }

        try {
            if (rs != null) {
                rs.close();
            }
            if (st != null) {
                st.close();
            }
            if (con != null) {
                con.close();
            }

        } catch (SQLException ex) {
            Logger lgr = Logger.getLogger(Main.class.getName());
            lgr.log(Level.WARNING, ex.getMessage(), ex);
        }
    }

}
