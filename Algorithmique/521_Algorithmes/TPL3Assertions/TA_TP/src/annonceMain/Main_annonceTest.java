package annonceMain;

import static org.junit.Assert.*;
import junit.framework.Assert;

import org.junit.Test;

import chaineTab.Chaine;

public class Main_annonceTest {

	@Test
	public void testAnnonce() {
		//Initialisation
		char tc1[] = { 'v','a','l','e','u','r' };
		char tc2[] = { 'e','u','r','o','p','e' };
		char tc3[] = { 'c','a','t','i','o','n' };
		char tc4[] = { 'i','o','n','i','s','a','t','i','o','n' };
		char tc5[] = { 's','a','l','o','n' };
		char tc6[] = { 'n','o','u','v','e','a','u' };
		
		Chaine c1 = new Chaine(tc1);
		Chaine c2 = new Chaine(tc2);
		Chaine c3 = new Chaine(tc3);
		Chaine c4 = new Chaine(tc4);
		Chaine c5 = new Chaine(tc5);
		Chaine c6 = new Chaine(tc6);
		
		char te1[] = {'e','u','r'};
		Chaine expected1 = new Chaine(te1);
		char te2[] = {'i','o','n'};
		Chaine expected2 = new Chaine(te2);
		char te3[] = {'n'};
		Chaine expected3 = new Chaine(te3);
		char te4[] = new char[0];
		Chaine expected4 = new Chaine(te4);
		char te5[] = new char[0];
		Chaine expected5 = new Chaine(te5);
		
		
		//Invocation et vérification
		Chaine actual1 = Main_annonce.Annonce(c1,c2);
		Chaine actual2 = Main_annonce.Annonce(c3,c4);
		Chaine actual3 = Main_annonce.Annonce(c5,c6);
		Chaine actual4 = Main_annonce.Annonce(c1,c6);
		Chaine actual5 = Main_annonce.Annonce(c2,c3);
		
		Assert.assertTrue(actual1.isEgal(expected1));
		Assert.assertTrue(actual2.isEgal(expected2));
		Assert.assertTrue(actual3.isEgal(expected3));
		Assert.assertTrue(actual4.isEgal(expected4));
		Assert.assertTrue(actual5.isEgal(expected5));
	}

}
