package chaineListe;

import static org.junit.Assert.*;
import junit.framework.Assert;

import org.junit.Test;

import chaineListe.Chaine;

public class TestChaineListe {

	/*
	@Test
	public void testChaine() {
		fail("Not yet implemented"); // TODO
	}*/

	@Test
	public void testConcatener() {
		//Initialisation
		char tc1[] = { 'a', 'b', 'c' };
		char tc2[] = { 'b', 'b', 'c' };
		Chaine c1 = new Chaine(tc1);
		Chaine c2 = new Chaine(tc2);
		char texpected[] = { 'a', 'b', 'c', 'b', 'b', 'c' };
		Chaine expected = new Chaine(texpected);
		
		//Evocation
		c1.concatener(c2);
		Chaine actual = c1;
		
		//Vérification
		Assert.assertTrue(expected.isEgal(actual));
	}

	
	@Test
	public void testCharAt() {
		//Initialisation
		char tc1[] = { 'a', 'b', 'c' };
		Chaine c1 = new Chaine(tc1);
		char expected = 'b';
		
		//Invocation
		char actual = c1.charAt(1);
		
		//Vérification
		Assert.assertEquals(expected, actual);
	}

	
	@Test
	public void testSouschaine() {
		//Initialisation
		char tc1[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };
		Chaine c1 = new Chaine(tc1);
		char texp[] = { 'c', 'd', 'e', 'f', 'g' };
		Chaine expected = new Chaine(texp);
		
		char tc2[] = { 'a', 'b', 'c' };
		Chaine c2 = new Chaine(tc2);
		char texp2[] = { 'b', 'c' };
		Chaine expected2 = new Chaine(texp2);
		
		char tc3[] = { 'a', 'b', 'c' };
		Chaine c3 = new Chaine(tc3);
		Chaine expected3 = new Chaine();
		
		//Invocation
		Chaine actual = c1.souschaine(2, 5);
		Chaine actual2 = c2.souschaine(1, 13);
		Chaine actual3 = c3.souschaine(1, 0);
		
		//Vérification
		Assert.assertTrue(expected.isEgal(actual));
		Assert.assertTrue(expected2.isEgal(actual2));
		Assert.assertTrue(expected3.isEgal(actual3));
	}


	@Test
	public void testIsVide() {
		//Initialisation
		char tvide[] = {};
		Chaine vide = new Chaine(tvide);
		char tc1[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };
		Chaine c1 = new Chaine(tc1);
		boolean expectedV = true;
		boolean expectedF = false;
		
		//Invocation
		boolean actualV = vide.isVide();
		boolean actualF = c1.isVide();
		
		//Vérification
		Assert.assertEquals(expectedV, actualV);
		Assert.assertEquals(expectedF, actualF);
	}

	@Test
	public void testSupprimer() {

		//Initialisation 
		char tvide[] = {};
		Chaine vide = new Chaine(tvide);
		char tc1[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };
		Chaine c1 = new Chaine(tc1);
				
		//invocation
		c1.supprimer();
				
		//verification
		Assert.assertTrue(vide.isEgal(c1));
			
	}
	
}
