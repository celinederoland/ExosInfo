# ---------------- #
# Juliana Leclaire #
# Céline de Roland #
# TP3 INFO 632     #
# ---------------- #


# ------------ #
# Exercice 1.1 #
# ------------ #

# Initialisation du flot à zéro #
def initFlot( R ):
	"""
	Initialise un flot nul sur le réseau de transport ``R``. Retourne un
  ``dict()`` possèdant une entrée pour chaque couple ``(u,v)`` tel que
  ``(u,v)`` ou ``(v,u)`` est un arc de ``R``.
	"""
	dico = dict();
	for triplet in R.edges():
		couple = (triplet[0],triplet[1])
		dico[couple] = 0
		couple = (triplet[1],triplet[0])
		dico[couple] = 0
	return dico

# Affichage graphique du réseau avec le flot #
def afficheFlot( R, f ):
	"""
	Retourne une image (de type ``Graphics`` ) représentant le flot ``f``
  dans le réseau ``R``.
	"""
	G = copy(R)
	d = dict()

	# On modifie les labels sur toutes les arêtes lorsque le flot n'est pas nul (ajout du [f]) #
	for triplet in R.edges():
		capacite = triplet[2]
		couple = (triplet[0],triplet[1])
		flot = f[couple]
		if flot != 0:
			label = "%d [%d]" % (capacite,flot)
			G.set_edge_label(triplet[0],triplet[1],label)

	# On modifie la couleur des arêtes #
	for triplet in G.edges():
		couple = (triplet[0],triplet[1])
		flot = f[couple]
		setEdgeColor(d,triplet,flot/capacite)

	# On retourne l'image #
	return G.plot(edge_labels = true, edge_colors = d)

# ------------ #
# Exercice 1.2 #
# ------------ #

# On calcule la somme des flots sortant de la source #
def valeurFlot( R, f ):
	"""
	Retourne la valeur du flot ``f`` sur le réseau ``R``.
	"""
	S = 0
	for v in G.neighbors_out('s'):
		S = S + f[('s',v)]
	return S

# ------------ #
# Exercice 2.1 #
# ------------ #

# Graphe résiduel #
def residuel( R, f ):
	"""
	Retourne le réseau résiduel du réseau de transport ``R`` traversé par
  le flot ``f``.
	"""
	res = DiGraph()
	res.add_vertices( R.vertices() )
	res.set_pos( R.get_pos() )

	# Sur chaque arête : résiduel = capacité - flot
	for triplet in R.edges():
		capacite = triplet[2]
		couple = (triplet[0],triplet[1])
		flot = f[couple]
		if (capacite - flot) != 0 :
			label = (capacite - flot)
			res.add_edge(triplet[0],triplet[1],label)

	return res

# ------------ #
# Exercice 2.2 #
# ------------ #

# Cette fonction sert à écrire la liste
# des sommets du chemin trouvé à l'aide
# du tableau pere
def retrouverChemin(pere,u,v):
	l = list()
	if (pere[v] != None):
		x = v
		while x != u :
			l.append(x)
			x = pere[x]
		l.append(u)
	l.reverse()
	return l

# Parcours en profondeur vu en cours #
def cheminProfondeur( G, u, v ):
	"""
	étant donné un graphe orienté ``g``, effectue un parcours en
  profondeur partiel à partir du sommet ``u``. Dès que le sommet ``v``
  est rencontré, le parcours s'arrête et le chemin de ``u`` à ``v``
  induit par ce parcours est retourné sous la forme d'une liste de
  sommets : ``[ u, ..., v ]``.
	Si ``v`` n'est pas accessible à partir de ``u`` une liste vide ``[]`` est
  retournée.
	"""
	vu = dict()
	pere = dict()
	# initialisation #
	for w in G.vertices():
		vu[w]=false
		pere[w]=None
	# on lance la récursivité #
	visiter(G,u,v,vu,pere)
	return retrouverChemin(pere,u,v)

# fonction récursive de visite d'un sommet #
def visiter(G,u,v,vu,pere):
	# dès qu'on visite un sommet on place vu à true 
	#(on ne se préoccupe pas de savoir si le chemin 
	# qu'on a pris pour y arriver était optimal ) #
	vu[u]=true

	# on parcourt un voisin et un enfant de ce voisin 
	# et ainsi de suite jusqu'à ce qu'il n'y ait plus
	# de possibilité, et on revient alors à ce point
	# pour parcourir en profondeur un autre de ses enfants, 
	# quand il n'en a plus on remonte (ie on termine 
	# la fonction, ce qui ramène au niveau supérieur 
	# de la récursivité) #
	for w in G.neighbors_out(u):
		if not vu[w]:
			pere[w]=u
			# Si on a trouvé le sommet final recherché,
			# on s'arrête ici
			if w == v:
				return
			visiter(G,w,v,vu,pere)

# ------------ #
# Exercice 2.3 #
# ------------ #

# Une itération de FordFulkerson correspond à ajouter autant de flot que
# possible sur un chemin existant entre la source et le puit
def iterationFordFulkerson(R,f,fct_chemin):
	"""
	Étant donné un réseau de transport ``R`` et un flot ``f``, effectue une
  itération de l'algorithme de Ford-Fulkerson en utilisant la
  fonction ``fct_chemin`` afin de trouver un chemin améliorant de ``s`` à
  ``p``.
     
	Retourne ``true`` si le flot a été augmenté et ``false`` si le flot est
  déjà optimal.
     
	Notes:
		* Dans le cas où un chemin améliorant est trouvé, l'objets ``f``
    passé en paramètre est modifié
     
		* Voir la documentation de la fonction ``FordFulkerson`` pour plus
    d'information sur la fonction ``fct_chemin``
	"""
	S = fct_chemin(R,'s','p')
	# Si S est vide, il n'existe plus de chemin de s à p, donc le flot est
	# déja optimal
	# Si S est non vide, elle représente un chemin de s à p. 
	# on va pouvoir ajouter du flot sur ce chemin
	if S != []:
		A = list() # liste des arêtes du chemin de s à p
		capacite = list() # liste des capacités des arêtes de A
		l = len(S)
		# On transforme le chemin (s,u1,...,p) en 2 listes A et capacite
		for i in range(l-1):
			A.append((S[i],S[i+1]))
			capacite.append(R.edge_label(S[i],S[i+1]))
		# Le maximum de flot qu'on pourra ajouter est égal à la plus petite
		# capacité rencontrée sur le chemin de s à p
		ajout = min(capacite)
		# On ajoute ce flot sur toutes les arêtes du chemin de s à p
		for arc in A:
			f[arc] = f[arc] + ajout
			f[(arc[1],arc[0])] = - f[arc]
		# On retourne true car on a pu ajouter du flot
		return True
	# On retourne false car on a pas pu ajouter de flot
	return False

# Algorithme de calcul du flot maximal :
def FordFulkerson(R,fct_chemin):
	"""
	Étant donné un réseau de transport ``R``, calcule un flot maximal de
  ``s`` à ``p``. Les chemins améliorant sont calculés à l'aide de la
  fonction ``fct_chemin`` qui prend trois paramètres :
      
    * un graphe orienté,
      
    * le sommet de départ,
      
    * le sommet d'arrivé.

  ``fct_chemin`` retourne le chemin sélectionné sous la forme d'une liste
  de sommets de la forme : ``['s', ..., 'p']``

  Retourne un flot maximal sur ``R``.
	"""
	f = initFlot(R)
	res = copy(R)

	# tant qu'il existe un chemin de s à p 
	# (iterationFordFulkerson renvoie false s'il n'en 
	# existe plus)
	while iterationFordFulkerson(res,f,fct_chemin):
		# iterationFordFulkerson a rajouté du flot
		# on calcule donc le résiduel et on recommence
		res = residuel(R,f)

	return f

# ------------ #
# Exercice 3   #
# ------------ #

# parcours en largeur vu en cours 
# voir plus bas la comparaison avec le parcours en profondeur
def cheminLargeur(G,u,v):
	vu = dict()
	pere = dict()
	F = list()

	# Initialisation
	for s in G.vertices():
		vu[s] = False
		pere[u] = None
	# On a visité la racine
	vu[u] = True
	F.append(s)

	while len(F) != 0 :
		s = F.pop(0)
		# pour chaque sommet traité, on traite tous ses fils
		# s'ils n'ont pas déjà été traités
		# et ainsi de suite jusqu'à ce qu'il n'y ait plus de sommet
		for w in G.neighbors_out(s):
			if (vu[w] == False):
				vu[w] = True
				pere[w] = s
				# Si on a trouvé le sommet final recherché,
				# on s'arrête ici
				if (w == v):
					return retrouverChemin(pere,u,v)
				# on place dans la file le sommet qu'on vient de traiter
				# afin d'indiquer qu'il faudra tester ses fils
				F.append(w)
	return list()

# Comparaison des deux parcours : 
# executez la fonction comparer() pour voir nos résultats graphiquement #

def comparer():
	L = list_plot([(10,1.14), (100,54.4), (500,2800), (700,7740), (1000,8980)],plotjoined=True,color='red')
	P = list_plot([(10,1.16), (100,54.8), (500,2600), (700,7250),(1000,8790)],plotjoined=True,color='green')
	TL = text("temps parcours largeur",(200,6000),color='red')
	TP = text("temps parcours profondeur",(200,5000),color='green')
	(TL+TP+L+P).show()

# Parcours en profondeur semble légèrement plus rapide
# sans que la différence entre les deux soit vraiment 
# significative

# sage: G = randomNetworkSimple( 10 )
# sage: timeit("f = FordFulkerson(G,cheminLargeur)")
# 625 loops, best of 3: 1.14 ms per loop
# sage: timeit("f = FordFulkerson(G,cheminProfondeur)")
# 625 loops, best of 3: 1.16 ms per loop

# sage: G = randomNetworkSimple( 100 )
# sage: timeit("f = FordFulkerson(G,cheminLargeur)")
# 5 loops, best of 3: 54.4 ms per loop
# sage: timeit("f = FordFulkerson(G,cheminProfondeur)")
# 5 loops, best of 3: 54.8 ms per loop

# sage: G = randomNetworkSimple( 500 )
# sage: timeit("f = FordFulkerson(G,cheminLargeur)")
# 5 loops, best of 3: 2.8 s per loop
# sage: timeit("f = FordFulkerson(G,cheminProfondeur)")
# 5 loops, best of 3: 2.6 s per loop

# sage: G = randomNetworkSimple( 700 )
# sage: timeit("f = FordFulkerson(G,cheminLargeur)")
# 5 loops, best of 3: 7.74 s per loop
# sage: timeit("f = FordFulkerson(G,cheminProfondeur)")
# 5 loops, best of 3: 7.25 s per loop

# sage: G = randomNetworkSimple( 1000 )
# sage: timeit("f = FordFulkerson(G,cheminLargeur)")
# 5 loops, best of 3: 8.98 s per loop
# sage: timeit("f = FordFulkerson(G,cheminProfondeur)")
# 5 loops, best of 3: 8.79 s per loop

