/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package celine.bdd.biblio;

import com.mongodb.BasicDBObject;
import com.mongodb.DB;
import com.mongodb.DBCollection;
import com.mongodb.DBCursor;
import com.mongodb.DBObject;
import com.mongodb.MongoClient;
import java.util.List;
import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.Scanner;

/**
 *
 * @author celine
 */
public class Main {
    
    private static MongoClient client;
    private static DB db;
    private static DBCollection usrs;
    private static DBCollection bks;
    private static Scanner sc = new Scanner(System.in);
    
    public static void initcon() throws UnknownHostException {
        System.out.println("Gestion bibliothèque");
        
        client = new MongoClient("localhost");
        db = client.getDB("biblio");
        usrs = db.getCollection("users");
        bks = db.getCollection("books");
        
        
    }
    
    public static void afficheMenu() {
        System.out.println("0 : quitter");
        System.out.println("1 : ajouter des livres");
        System.out.println("2 : voir les livres");
        System.out.println("3 : ajouter des lecteurs");
        System.out.println("4 : voir les lecteurs");
        System.out.println("5 : emprunter un livre");
        System.out.println("6 : rendre un livre");
        System.out.println("7 : supprimer un lecteur");
        System.out.println("8 : supprimer un livre");
    }
    
    public static void ajoutLivre() {
        System.out.println("Entrez le titre");
        String titre = sc.nextLine();
        System.out.println("Entrez l'auteur");
        String auteur = sc.nextLine();
        
        BasicDBObject usr = new BasicDBObject();
        usr.append("titre",titre);
        usr.append("auteur",auteur);  
        bks.insert(usr);
        
    }
    
    public static void ajoutLecteur() {
        
        System.out.println("Entrez le nom");
        String nom = sc.nextLine();
        System.out.println("Entrez le prénom");
        String prenom = sc.nextLine();
        
        BasicDBObject usr = new BasicDBObject();
        usr.append("nom",nom);
        usr.append("prenom",prenom);  
        usrs.insert(usr);
   
    }
    
    public static void supprimerLecteur() {
        System.out.println("nom de l'emprunteur");
        String nom = sc.nextLine();
        
        BasicDBObject usr = new BasicDBObject("nom", nom);
        DBCursor usr_cursor = usrs.find(usr);
        usr = (BasicDBObject) usr_cursor.next();
        
        usrs.remove(usr);
        
    }
    
    public static void supprimerLivre() {
        System.out.println("titre du livre");
        String titre = sc.nextLine();
        
        BasicDBObject bk = new BasicDBObject("titre", titre);
        DBCursor bk_cursor = bks.find(bk);
        bk = (BasicDBObject) bk_cursor.next();
        
        bks.remove(bk);
    }
    
    public static void voirLivres() {
        System.out.println("");
        DBCursor cursor = bks.find();
        while(cursor.hasNext()) {
            DBObject res = cursor.next();
            String titre = (String) res.get("titre");
            String auteur = (String) res.get("auteur");
            Object emprunteur = res.get("emprunteur");
            System.out.print("titre : " + titre + " ; auteur : " + auteur + " --- ");
            if(emprunteur != null) { 
                DBObject emp = (DBObject) emprunteur;
                System.out.println("emprunté par : " 
                        + emp.get("prenom") 
                        + " " 
                        + emp.get("nom"));
            }
            else {
                System.out.println("libre");
            }
        }
        System.out.println("");       
    }
    
    public static void voirLecteurs() {
        System.out.println("");
        DBCursor cursor = usrs.find();
        while(cursor.hasNext()) {
            DBObject res = cursor.next();
            String nom = (String) res.get("nom");
            String prenom = (String) res.get("prenom");
            Object bks = res.get("books");
            //System.out.println(res);
            System.out.print("prenom : " + prenom + " ; nom : " + nom + " --- ");
            if (bks != null) {
                ArrayList<DBObject> l = (ArrayList<DBObject>) bks;
                for (DBObject bk : l) {
                    System.out.print(bk.get("titre") + " ; ");
                }
                System.out.println();
            }
            else {
                System.out.println("pas de livres empruntés");
            }
        }
        System.out.println("");
    }
    
    public static void emprunter() {
        System.out.println("nom de l'emprunteur");
        String nom = sc.nextLine();
        System.out.println("titre du livre");
        String titre = sc.nextLine();
        
        BasicDBObject usr = new BasicDBObject("nom", nom);
        DBCursor usr_cursor = usrs.find(usr);
        usr = (BasicDBObject) usr_cursor.next();
        
        
        BasicDBObject bk = new BasicDBObject("titre", titre);
        DBCursor bk_cursor = bks.find(bk);
        bk = (BasicDBObject) bk_cursor.next();
        
        BasicDBObject usr_avec_emprunt = (BasicDBObject) usr.copy();
        ArrayList<DBObject> l;
        Object b = usr_avec_emprunt.get("books");
        if (b == null) { l = new ArrayList<DBObject>(); }
        else { l = (ArrayList<DBObject>) b; }
        l.add(bk);
        usr_avec_emprunt.append("books",l);
        
        BasicDBObject bk_avec_emprunt = (BasicDBObject) bk.copy();
        bk_avec_emprunt.append("emprunteur",usr);
        
        usrs.update(usr, usr_avec_emprunt);
        bks.update(bk, bk_avec_emprunt);

    }
    
    public static void rendre() {
        System.out.println("nom de l'emprunteur");
        String nom = sc.nextLine();
        System.out.println("titre du livre");
        String titre = sc.nextLine();
        
        BasicDBObject usr = new BasicDBObject("nom", nom);
        DBCursor usr_cursor = usrs.find(usr);
        usr = (BasicDBObject) usr_cursor.next();
        
        
        BasicDBObject bk = new BasicDBObject("titre", titre);
        DBCursor bk_cursor = bks.find(bk);
        bk = (BasicDBObject) bk_cursor.next();
        
        //System.out.println("utilisateur trouvé :" + usr);
        BasicDBObject usr_sans_emprunt = (BasicDBObject) usr.copy();
        DBObject sup = null;
        ArrayList<DBObject> l = (ArrayList<DBObject>) usr_sans_emprunt.get("books");
        for (DBObject b : l) {
            String actual = (String) b.get("titre");
            String expected = (String) bk.get("titre");
            if (actual.equals(expected)) {
                sup = b;
            }
        }
        l.remove(sup);
        if (l.isEmpty()) { usr_sans_emprunt.removeField("books"); }
        else { usr_sans_emprunt.append("books",l); }
        
        BasicDBObject bk_sans_emprunt = (BasicDBObject) bk.copy();
        bk_sans_emprunt.removeField("emprunteur");
        
        usrs.update(usr, usr_sans_emprunt);
        bks.update(bk, bk_sans_emprunt);

    }
    
    public static void main(String args[]) throws UnknownHostException {
             
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
            } else if (choix == 6) {
                rendre();
            } else if (choix == 7) {
                supprimerLecteur();
            } else if (choix == 8) {
                supprimerLivre();
            }

            afficheMenu();


            choix = sc.nextInt();
            sc.nextLine();
        }

    }
    
    
}
