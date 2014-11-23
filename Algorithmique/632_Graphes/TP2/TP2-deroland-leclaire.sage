# Juliana Leclaire, Céline de Roland INFO632 TP2 #

# A la fin du fichier nous proposons des commandes pour tester, il suffit de les décommenter #

# ----------------------------------------------------------- #
# Construction et affichage du graphe                         #
# ----------------------------------------------------------- #

# Lit les deux fichiers pour construire un DiGraph. #
def lireTriangulation( nom_fichier_sommets, nom_fichier_triangles ):
	# création du DiGraph #
	G = DiGraph()

	# création des sommets #
	fichier = open( nom_fichier_sommets )
	uneLigne = fichier.readline()
	nb = int( uneLigne )
	positions = dict()
	# On 'coupe' chaque ligne en 2 parties pour les 2 coordonnées, on ajoute le sommet et on enregistre ses coordonnées dans le dictionnaire positions #
	for i in range(0,nb):
		uneLigne = fichier.readline()
		G.add_vertex( i )
		(str_x, dummy, str_y ) = uneLigne.partition( " " )
		(x,y) = ( float( str_x ), float( str_y ) )
		positions[i] = (x,y)
	fichier.close()

	# création des arêtes #
	fichier = open( nom_fichier_triangles )
	uneLigne = fichier.readline()
	nb = int( uneLigne )
	# On 'coupe' chaque ligne en 3 parties pour les 3 sommets du triangles et on crée les 3 arêtes entre eux #
	for i in range(0,nb):
		uneLigne = fichier.readline()
		(str1, dummy1, partie2) = uneLigne.partition( " " )
		(str2, dummy2, str3) = partie2.partition( " " )
		(a,b,c) = (float(str1),float(str2),float(str3))
		G.add_edges([(a,b),(b,c),(c,a)]) 
	fichier.close()

	# On associe le dictionnaire des coordonnées des sommets au graphe #
	G.set_pos(positions)

	return G

# Affiche un sommet du graphe (cercle + étiquette) #
# Nous avons ajouté le paramètre color pour la section 3 #
def affVertex( G, v, scale, color=(0,0,1) ):
	(x,y) = G.get_pos()[v]
	return circle((x,y),scale,rgbcolor=color) + text(v,(x,y),rgbcolor=color)

# Affiche une arête du graphe (flèche) #
# Nous avons ajouté le paramètre color pour la section 3 #
def affArc( G, e, scale, color=(1,0,0) ):
	# On récupère la position des extrémités de l'arête #
	(xd,yd) = G.get_pos()[e[0]]
	(xf,yf) = G.get_pos()[e[1]]

	# On arrange les positions pour que la flèche ne parte pas du centre du cercle #
	if (xd < xf):
		xd = xd + scale/1.2
		xf = xf - scale/1.2
	else:
		xd = xd - scale/1.2
		xf = xf + scale/1.2
	if (yd < yf):
		yd = yd + scale/1.2
		yf = yf - scale/1.2
	else:
		yd = yd - scale/1.2
		yf = yf + scale/1.2
	dep = (xd,yd)
	fin = (xf,yf)

	# On renvoie la flèche du départ vers la fin #
	return arrow(dep,fin,rgbcolor=color,width=1)

# Crée un objet Graphics pour afficher le graphe de façon plus lisible #
def afficheDiGraph( G, scale ):
	P = Graphics()
	for v in G.vertices():
		P += affVertex(G,v,scale)
	for e in G.edges():
		P += affArc(G,e,scale)
	return P

# Poids1 : 0 sur la diagonale, 1 là où il y a des arêtes, infini pour le reste #
def poids1( G, e ):
	if (e[0] == e[1]):
		return 0
	if G.has_edge(e):
		return 1
	return infinity

# PoidsE : 0 sur la diagonale, distance entre les sommets là où il y a des arêtes, infini pour le reste #
def poidsE( G, e ):
	if (e[0] == e[1]):
		return 0
	if G.has_edge(e):
		(xd,yd) = G.get_pos()[e[0]]
		(xf,yf) = G.get_pos()[e[1]]
		return sqrt((xf-xd)**2 + (yf-yd)**2)
	return infinity



# ----------------------------------------------------------- #
# Plus court chemin entre tous les sommets                    #
# ----------------------------------------------------------- #

# Matrice Poids #
def matricePoids( G, fct_poids ):
	# On compte les sommets du graphe #
	n = 0
	for v in G.vertices():
		n = n + 1

	# On crée une matrice carrée de taille nombre de sommets du graphe #
	M = matrix( n,n, ring=RealDoubleField() )

	# On remplit la matrice avec la fonction poids passée en paramètre #
	for i in range(0,n):
		for j in range(0,n):
			M[i,j] = round(fct_poids(G,(i,j)),3)

	return M


# FloydWarshall #
def floydWarshall( M ):
	# On compte le nombre de sommets du graphe #
	n = M.nrows()

	# On crée les matrices D et I #
	D = copy(M) # distances : égale à M au départ #
	I = matrix(n,n) # sommets intermédiaires #

	# On remplit I avec la valeur -1 (pas de sommet intermédiaire) #
	for i in range(0,n):
		for j in range(0,n):
			I[i,j] = -1

	# Pour chaque sommet k #
	for k in range(0,n):
		# Pour chaque couple (i,j) de sommets #
		for i in range(0,n):
			for j in range(0,n):
				# Si c'est plus court de passer par k pour aller de i à j on remplace D[i,j] et on indique dans I qu'on passe par k #
				if (D[i,k]+D[k,j] < D[i,j]):
					D[i,j] = D[i,k] + D[k,j]
					I[i,j] = k
	return (D,I)

# Calcul du chemin le plus court entre i et j à l'aide de D et I #
def chemin( I, i, j ):
	# On trouve le sommet par lequel passer, puis on réitère le procédé jusqu'à obtenir -1 (ie chemin direct) #
	k = I[i,j]
	L = list()
	if k != -1 :
		L += chemin( I, i, k )
		L.append(k)
		L += chemin( I, k, j )
	return L

# Crée un graphique pour tracer le chemin entre deux sommets du graphe #
def afficheChemin( G, I, depart, arrivee, scale ):
	P = Graphics()
	L = chemin(I,depart,arrivee)
	# On affiche les flèches une par une le long de la liste L #
	for i in L:
		(xd,yd) = G.get_pos()[depart]
		(xf,yf) = G.get_pos()[i]
		# On arrange les positions des extrémités des flèches #
		if (xd < xf):
			xd = xd + scale/1.2
			xf = xf - scale/1.2
		else:
			xd = xd - scale/1.2
			xf = xf + scale/1.2
		if (yd < yf):
			yd = yd + scale/1.2
			yf = yf - scale/1.2
		else:
			yd = yd - scale/1.2
			yf = yf + scale/1.2
		dep = (xd,yd)
		fin = (xf,yf)
		# ajout de la flèche au graphique #
		P += arrow(dep,fin,rgbcolor=(0,0,0),width=3)
		# on passe au sommet suivant #
		depart = i

	# On relie le dernier sommet à l'arrivée #
	(xd,yd) = G.get_pos()[depart]
	(xf,yf) = G.get_pos()[arrivee]
	if (xd < xf):
		xd = xd + scale/1.2
		xf = xf - scale/1.2
	else:
		xd = xd - scale/1.2
		xf = xf + scale/1.2
	if (yd < yf):
		yd = yd + scale/1.2
		yf = yf - scale/1.2
	else:
		yd = yd - scale/1.2
		yf = yf + scale/1.2
	dep = (xd,yd)
	fin = (xf,yf)
	P += arrow(dep,fin,rgbcolor=(0,0,0),width=3)

	return P




# ----------------------------------------------------------- #
# Distance et plus court chemin à un sommet                   #
# ----------------------------------------------------------- #


# Trouve l'indice du plus petit élément de clé tel traite soit faux #
# Il faut s'assurer avant d'appeler cette fonction qu'il existe bien des éléments non infinis et non traités #
def minParmisLesNonTraite(cle,traite):
	min = infinity
	# On crée une liste de couples (indice,valeur) du tableau clé #
	L = [(key,value) for key, value in cle.iteritems()]
	# On parcourt cette liste à la recherche du minimum #
	for paire in L:
		if (paire[1] < min and not traite[paire[0]]):
			min = paire[1]
			clemin = paire[0]
	return clemin

# Algorithme de Dijkstra #
def dijkstra( G, u, fct_poids ):
	cle = dict() # poids du chemin le plus court entre chaque sommet et le sommet de départ u #
	traite = dict() # un sommet est traité lorsqu'on est sur que le tableau clé contient la distance minimale #
	pere = dict() # père du sommet sur le plus court chemin entre le sommet et le sommet de départ u #
	nbSommetsNonTraites = 0; # On compte le nombre de sommets non traités pour savoir quand s'arrêter #

	# Initialisation #
	for v in G.vertices():
		cle[v] = infinity
		traite[v] = false
		nbSommetsNonTraites += 1;
	# Le sommet de départ est à une distance nulle de lui même et n'a pas de père (c'est la racine de l'arbre) #
	cle[u] = 0
	pere[u] = None

	# On continue tant qu'il y a des sommets non traités #
	while (nbSommetsNonTraites > 0):
		# On prend le sommet non traité le plus proche du sommet de départ #
		v = minParmisLesNonTraite(cle,traite)
		traite[v]=true
		nbSommetsNonTraites -= 1;
		# On parcourt ses voisins pour voir si on peut y accéder plus rapidement en passant par ce sommet #
		for w in G.neighbors(v):
			if not traite[w] and cle[v]+fct_poids(G,(v,w))<cle[w]:
				cle[w]=cle[v]+fct_poids(G,(v,w))
				pere[w] = v

	return (cle,pere)

# Fonction de l'énoncé pour changer la couleur en fonction de la distance #
def couleurDistance( d, dmax ):
	cmap = colormaps['rainbow_r']
	if ( d < 0.0 ):
		d = 0.0
	elif ( d > dmax ):
		d = dmax
	c = cmap( float(d)/float(dmax) )
	return c[0:3]

# Cherche le maximum du tableau clé #
# Nous l'avons recodé car la fonction max donnait le max des indices du dictionnaire #
def maxval(cle):
	max = 0
	L = [(key,value) for key, value in cle.iteritems()]
	for paire in L:
		if (paire[1] > max):
			max = paire[1]
	return max

# Affiche le graphe avec les couleurs correspondant à la distance #
# Même fonctionnement que afficheDiGraph mais on passe des couleurs en paramètre à affVertex et affArc #
def afficheDiGraphDistance( G, u, fct_poids, scale ):
	P = Graphics()
	(cle,pere) = dijkstra(G,u,fct_poids)
	dmax = maxval(cle)
	for v in G.vertices():
		P += affVertex(G,v,scale,couleurDistance(cle[v],dmax))
	for e in G.edges():
		P += affArc(G,e,scale,couleurDistance(cle[e[0]],dmax))
	return P



# ----------------------------------------------------------- #
# Exercice optionnel                                          #
# ----------------------------------------------------------- #

# Ces variables globales permettent de ne pas repasser les dictionnaires en paramètre à chaque appel récursif #
gpere=dict()
gvu=dict()
gcle=dict()

# Parcours en profondeur #
def parcoursProfondeur( G, u, fct_poids ):
	# initialisation #
	for v in G.vertices():
		gvu[v]=false
		gpere[v]=None
		gcle[v]=infinity
	# le sommet de départ est à une distance nulle de lui même #
	gcle[u]=0
	# on lance la récursivité #
	visiter(G,u,fct_poids)

# fonction récursive de visite d'un sommet #
def visiter(G,u,fct_poids):
	# dès qu'on visite un sommet on place vu à true (on ne se préoccupe pas de savoir si le chemin qu'on a pris pour y arriver était optimal ) #
	gvu[u]=true
	# on parcourt un voisin et un enfant de ce voisin et ainsi de suite jusqu'à ce qu'il n'y ait plus de possibilité, et on revient alors à ce point pour parcourir en profondeur un autre de ses enfants, quand il n'en a plus on remonte (ie on termine la fonction, ce qui ramène au niveau supérieur de la récursivité) #
	for v in G.neighbors_out(u):
		if not gvu[v]:
			gpere[v]=u
			gcle[v]=gcle[u]+fct_poids(G,(u,v))
			visiter(G,v,fct_poids)

# Affiche le graphe en couleur avec le parcours en profondeur #
def afficheDiGraphDistanceProfondeur( G, u, fct_poids, scale ):
	P = Graphics()
	parcoursProfondeur(G,u,fct_poids)
	dmax = maxval(gcle)
	for v in G.vertices():
		P += affVertex(G,v,scale,couleurDistance(gcle[v],dmax))
	for e in G.edges():
		P += affArc(G,e,scale,couleurDistance(gcle[e[0]],dmax))
	return P


# Explication : on obtient une figure approchant un 'camenbert' car les chemins obtenus par le parcours en profondeur peut être long et sinueux. Par conséquent, deux points géographiquement proches du départ peuvent être à des distances calculées très différentes selon les chemins empruntés #


# ----------------------------------------------------------- #
# Tests                                                       #
# ----------------------------------------------------------- #

# Graphe créé en lisant les fichiers de points et de triangles #
G10 = lireTriangulation( "./pts-box-10.txt", "./tri-box-10.txt" )
G10.show()

# Graphe à 10 sommets affichés plus lisiblement #
P = afficheDiGraph( G10, 0.02 )
P.show( axes = False )
# Graphe à 100 sommets affichés plus lisiblement #
G100 = lireTriangulation( "./pts-box-100.txt", "./tri-box-100.txt" )
P = afficheDiGraph( G10, 0.02 )
P.show( axes = False )

# Matrice poids pour le graphe à 10 sommets #
M10poids1 = matricePoids( G10, poids1 )
M10poids1
M10poidsE = matricePoids( G10, poidsE )
M10poidsE
# Matrice poids pour le graphe à 100 sommets #
M100poids1 = matricePoids( G100, poids1 )
M100poidsE = matricePoids( G100, poidsE )

# FloydWarshall pour le graphe à 10 sommets #
(D10poids1, I10poids1) = floydWarshall( M10poids1 )
(D10poids1, I10poids1)
(D10poidsE, I10poidsE) = floydWarshall( M10poidsE )
(D10poidsE, I10poidsE)
# FloydWarshall pour le graphe à 100 sommets #
(D100poids1, I100poids1) = floydWarshall( M100poids1 )
(D100poidsE, I100poidsE) = floydWarshall( M100poidsE )

# Plus court chemin avec FloydWarshall pour le graphe à 100 sommets #
chemin(I100poidsE, 5, 17)
chemin(I100poidsE, 50, 44)
V = afficheDiGraph( G100, 0.02 )
V1 = afficheChemin( G100, I100poidsE, 5, 17, 0.02 ) 
(V+V1).show(axes=false)   
V1 = afficheChemin( G100, I100poidsE, 50, 44, 0.02 )
(V+V1).show(axes=false)          

# Dijkstra #
(cle, pere) = dijkstra( G10, 0, poids1 )
(cle, pere)

# Affichage des distances sur le graphe à 10 sommets #
P = afficheDiGraphDistance( G10, 0, poids1, 0.02 )
P.show(axes=false) 
P = afficheDiGraphDistance( G10, 0, poidsE, 0.02 )
P.show(axes=false) 
# Affichage des distances sur le graphe à 100 sommets #
P = afficheDiGraphDistance( G100, 0, poids1, 0.02 )
P.show(axes=false) 
P = afficheDiGraphDistance( G100, 0, poidsE, 0.02 )
P.show(axes=false) 
# Affichage des distances sur le graphe à 1000 sommets #
G1000 = lireTriangulation( "./pts-box-1000.txt", "./tri-box-1000.txt" )
P = afficheDiGraphDistance( G1000, 0, poids1, 0.02 )
P.show(axes=false) 
P = afficheDiGraphDistance( G1000, 0, poidsE, 0.02 )
P.show(axes=false) 
# Affichage des distances sur le graphe à 10000 sommets #
G10000 = lireTriangulation( "./pts-box-10000.txt", "./tri-box-10000.txt" )
# P = afficheDiGraphDistance( G10000, 0, poids1, 0.02 )
# P.show(axes=false) 
# P = afficheDiGraphDistance( G10000, 0, poidsE, 0.02 )
# P.show(axes=false) 

sys.setrecursionlimit(10000)

# Affichage des distances du parcours en profondeur sur le graphe à 10 sommets #
P = afficheDiGraphDistanceProfondeur( G10, 0, poids1, 0.02 )
P.show(axes=false) 
P = afficheDiGraphDistanceProfondeur( G10, 0, poidsE, 0.02 )
P.show(axes=false) 
# Affichage des distances du parcours en profondeur sur le graphe à 100 sommets #
P = afficheDiGraphDistanceProfondeur( G100, 0, poids1, 0.02 )
P.show(axes=false) 
P = afficheDiGraphDistanceProfondeur( G100, 0, poidsE, 0.02 )
P.show(axes=false) 
# Affichage des distances du parcours en profondeur sur le graphe à 1000 sommets #
P = afficheDiGraphDistanceProfondeur( G1000, 0, poids1, 0.02 )
P.show(axes=false) 
P = afficheDiGraphDistanceProfondeur( G1000, 0, poidsE, 0.02 )
P.show(axes=false) 
# Affichage des distances du parcours en profondeur sur le graphe à 10000 sommets #
# P = afficheDiGraphDistanceProfondeur( G10000, 5421, poids1, 0.02 )
# P.show(axes=false) 
P = afficheDiGraphDistanceProfondeur( G10000, 5421, poidsE, 0.02 )
P.show(axes=false) 
