/**
 * INFO724 Fichier : Decoupe_Dynamique.java Auteur : Xavier Provençal
 *
 *
 * Résoud le problème de découpe des barres par programmation dynamique.
 *
 * Implémentation de la méthode décrite dans Cormen, Leiserson, Rivest et Stein,
 * Chapitre 15.1
 */
package td_decoupebarresprof;

public class Decoupe_Dynamique {

    /**
     * Les prix sont fixés via le tableau ``prix``
     * 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10| 11
     * --|---|---|---|---|---|---|---|---|---|---|--- 
     * 0 | 1 | 5 | 7 | 11| 13| 15| 17| 19| 22| 26| 27
     */
    public static int[] prix = {0, 1, 5, 7, 11, 13, 15, 17, 19, 22, 26, 27};

    public static void usage() {
        System.out.println("Usage : java Decoupe_Dynamique lng");
        System.out.println("  où ``lng`` est la longueur de la tige à découper");
    }

    public static int decoupe_dyn(int n) {
        int[] solution = new int[n + 1];
        int prix_opt = 0;
        solution[0] = 0;
        for (int i = 1; i <= n; ++i) {
            solution[i] = 0;
            for (int j = 1; j <= Math.min(prix.length - 1, i); ++j) {
                solution[i] = Math.max(solution[i], solution[i - j] + prix[j]);
            }
        }
        return solution[n];
    }

    public static void main(String[] argv) {
        if (argv.length == 0) {
            usage();
            System.exit(1);
        }
        int n = Integer.parseInt(argv[0]);
        System.out.println("Découpe dynamique d'une barre de longueur : " + n);
        int prix_opt = decoupe_dyn(n);
        System.out.println("Prix optimal : " + prix_opt);
        System.exit(0);
    }
}
