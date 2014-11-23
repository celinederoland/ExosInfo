/**
 * INFO724
 * Fichier : Decoupe_Recursive.java
 * Auteur : Xavier Provençal
 *
 *
 * Résoud le problème de découpe des barres par récursivité.
 */
package td_decoupebarresprof;


public class Decoupe_Recursive
{
	/**
	 * Les prix sont fixés via le tableau ``prix``
	 *  0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10| 11
	 * ---|---|---|---|---|---|---|---|---|---|---|---
	 *  0 | 1 | 5 | 7 | 11| 13| 15| 17| 19| 22| 26| 27
	 */
	public static int[] prix = { 0, 1, 5, 7, 11, 13, 15, 17, 19, 22, 26, 27 };

	public static void usage()
	{
		System.out.println( "Usage : java Decoupe_Recursive lng" );
		System.out.println( "  où ``lng`` est la longueur de la tige à découper" );
	}

	public static int decoupe_rec( int n )
	{
		//System.out.println( "decoupe_rec(" + n + ")" );
		if ( n == 0 ) {
			return 0;
		}
		int prix_opt = 0;
		for ( int i=1; i <= Math.min( n, prix.length-1 ); ++i ) {
			prix_opt = Math.max( prix_opt, prix[i] + decoupe_rec( n-i ) );
		}
		return prix_opt;
	}

	public static void main( String[] argv )
	{
		if ( argv.length == 0 ) {
			usage();
			System.exit( 1 );
		}
		int n = Integer.parseInt( argv[0] );
		System.out.println( "Découpe naïve d'une barre de longueur " + n );
		int prix_opt = decoupe_rec( n );
		System.out.println( "Prix optimal : " + prix_opt );
		System.exit( 0 );
	}
}

