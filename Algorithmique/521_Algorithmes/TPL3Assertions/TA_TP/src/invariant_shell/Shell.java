package invariant_shell;


public class Shell {

	/**
	 * @param args
	 */
	
	public static void afficher(int t[])
	{
		for (int i = 1; i < t.length; i++)
		{
			System.out.print(t[i]);
			System.out.print(" ");
		}
		System.out.println();
	}
	
	public static void main(String[] args) {
		int i, j, k, x, l, m;
		boolean test_reussi, tout_reussi;
		tout_reussi = true;
		int incr = 18, n = 18;
		int t[] = {0,9,3,4,6,2,5,10,7,9,1,4,8,12,3,6,4,7,5};
		afficher(t);
		do
		{
			
			//ASSERTION : toutes les séries d'incrément incr sont triées
			// (au premier passage, cela constitue n séries d'un seul élément)
			test_reussi = true;
			for (l = 1 ; l <= n - incr; l++)
			{
				if (t[l] > t[l + incr]) {test_reussi = false; }
			}
			if (test_reussi) { System.out.print("."); }
			else { System.out.print("F"); tout_reussi = false;}
			//FIN ASSERTION
			
			incr = incr/2;
			
			for (k = 1; k <= incr; k++)
			{
				
				// todo a adapter fin boucle
				//ASSERTION : les k - 1 premières séries d'incrément incr sont triées
				//(au premier passage, cela constitue 0 séries (aucune série))
				test_reussi = true;
				for (l = 1; l <= k - 1; l++)
				{
					for (m = l; m + incr <= n; m = m + incr)
					{
						if (t[m] > t[m + incr]) { test_reussi = false; }
					}
				}
				if (test_reussi) { System.out.print("."); }
				else { System.out.print("F");  tout_reussi = false;}
				//FIN ASSERTION
				
				for (i = incr + k; i <= n; i = i + incr)
				{
					//ASSERTION : la kieme série d'incrément incr est triée jusqu'au rang i - incr
					//(Au premier passage : seul le premier élément de la série est trié)
					test_reussi = true;
					for (m = k; m + incr <= i - incr; m = m + incr)
					{
						if (t[m] > t[m + incr]) { test_reussi = false; }
					}
					if (test_reussi) { System.out.print("."); }
					else { System.out.print("F");  tout_reussi = false;}
					//FIN ASSERTION
					
					x = t[i];
					j = i - incr;
					
					while (j >= 1 && t[j] > x)
					{
						//ASSERTION :
						// Jusqu'à j - incr les éléments de la série sont triés
						// A partir de j + incr les éléments de la série sont triés ET supérieurs à x
						test_reussi = true;
						for (m = k; m + incr <= j - incr; m = m + incr)
						{
							if (t[m] > t[m + incr]) { test_reussi = false; }
						}
						for (m = j + incr; m + incr <= i; m = m + incr)
						{
							if (t[m] > t[m + incr] || t[m] <= x) { test_reussi = false; }
						}
						if (test_reussi) { System.out.print("."); }
						else { System.out.print("F");  tout_reussi = false;}
						//FIN ASSERTION
						
						t[j + incr] = t[j];
						
						//ASSERTION : TODO déplacé vers le bas
						// Jusqu'à j - 2incr les éléments de la série sont triés
						// A partir de j les éléments de la série sont triés ET supérieurs à x
						test_reussi = true;
						for (m = k; m + incr <= j - 2*incr; m = m + incr)
						{
							if (t[m] > t[m + incr]) { test_reussi = false; }
						}
						for (m = j; m + incr <= i; m = m + incr)
						{
							if (t[m] > t[m + incr] || t[m] <= x) { test_reussi = false; }
						}
						if (test_reussi) { System.out.print("."); }
						else { System.out.print("F");  tout_reussi = false;}
						//FIN ASSERTION
						
						j = j - incr;
					}
					
					//ASSERTION : 
					// Jusqu'à j - incr les éléments de la série sont triés ET inférieurs à x
					// A partir de j + 2incr les éléments de la série sont triés ET supérieurs à x
					// (donc en plaçant x à l'indice j + incr la série est triée dans son ensemble. )
					test_reussi = true;
					for (m = k; m + incr <= j; m = m + incr)
					{
						if (t[m] > t[m + incr] || t[m] > x) { test_reussi = false; }
					}
					for (m = j + 2*incr; m + incr <= i; m = m + incr)
					{
						if (t[m] > t[m + incr] || t[m] <= x) { test_reussi = false; }
					}
					if (test_reussi) { System.out.print("."); }
					else { System.out.print("F");  tout_reussi = false;}
					//FIN ASSERTION
					
					t[j + incr] = x;
					
					//ASSERTION : la kieme série est triée
					test_reussi = true;
					for (m = k; m + incr <= i; m = m + incr)
					{
						if (t[m] > t[m + incr]) { test_reussi = false; }
					}
					if (test_reussi) { System.out.print("."); }
					else { System.out.print("F");  tout_reussi = false;}
					System.out.println("");
					//FIN ASSERTION
				}
				// toutes les séries de 1..k sont triées
			}
			
			//ASSERTION : toutes les séries d'incrément incr sont triées (mais incr a été divisé par deux depuis l'assertion d'entrée de boucle).  A simplifié
			test_reussi = true;
			for (l = 1; l <= k; l++)
			{
				for (m = l; m + incr <= n; m = m + incr)
				{
					if (t[m] > t[m + incr]) { test_reussi = false; }
				}
			}
			if (test_reussi) { System.out.print("."); }
			else { System.out.print("F");  tout_reussi = false;}
			//FIN ASSERTION
			
		}
		while (incr != 1);
		
		//ASSERTION : la série d'incrément 1 est triée, c'est à dire le tableau est trié
		test_reussi = true;
		for (l = 1; l < t.length - 1; l++)
		{
			if (t[l] > t[l+1]) { test_reussi = false; }
		}
		if (test_reussi) { System.out.print("."); }
		else { System.out.print("F");  tout_reussi = false;}
		//FIN ASSERTION
		
		
		System.out.println("");
		afficher(t);
		if (tout_reussi) { System.out.println("Tous les tests ont réussi"); }
		else { System.out.println("Des tests ont échoué!!!"); }
	}
	
}
