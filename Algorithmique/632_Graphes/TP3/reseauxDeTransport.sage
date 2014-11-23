def uniforme( a, b ) :
	"""
	Retourne un générateur aléatoire uniforme de nombre entiers 
	dans {a, a+1, ..., b-1}.
	"""
	return lambda : floor ( uniform( a, b ) )

def proba( p ) :
	"""
	Retourne ``true`` avec une probabilité ``p``, ``false`` sinon.
	"""
	return uniform(0,1) < p

def randomNetworkSimple( n, prob_avant = 0.7, prob_arriere = 0.3 ) :
	"""
	Génère un réseau de transport aléatoire avec environ ``n`` sommets. 
	Voir la documentation de ``randomNetwork`` pour la signification des
	probabilités ``prob_avant`` et ``prob_arriere``.

	Exemple::

	sage: G = randomNetworkSimple( 20 )
	sage: G
	Digraph on 17 vertices
	sage: G.plot( edge_labels=true )
	"""
	numCol = floor( sqrt(n) )
	minCol = numCol//2
	maxCol =(3*numCol)//2
	cap = uniforme(1, 10)
	return randomNetwork( numCol, minCol, maxCol, prob_avant, prob_arriere, cap )



def randomNetwork( numCol, minCol, maxCol, prob_avant, prob_arriere, capacity_fct ) :
	"""
	Construit un réseau régulier aléatoire dont les sommets sont disposés sur
	``numCol`` colonnes. Chaque colonne contient un nombre de sommet aléatoire
	tiré uniformément entre ``minCol`` et ``maxCol``. Deux sommets dans des
	colonnes adjacentes ont une probabilité déterminée par ``prob_avant`` d'être
	reliés par un arc si cet arc est orienté vers le puits et par
	``prob_arriere`` si cet arc est orienté vers la source. La capacité d'un arc
	est attribué par un appel à ``capacity_fct``.  

	Une méthode de rejet est utilisée afin de s'assurer que le réseau est
	connexe, c'est-à-dire qu'on peut rejoindre ``p`` à partir de ``s``. Ansi, si
	les probabilités fournies sont insuffisantes, la méthode peut boucler
	arbitrairement longtemps.

	Exemple::

	sage: G = randomNetwork( 30, 50, 100, 0.76, 0.6, uniforme(1, 100) )
	sage: G.num_verts()
	2274
	sage: G.num_edges()
	15311
	"""
	G = DiGraph(weighted=true)
	columns = [ uniforme(minCol, maxCol )() for i in range( numCol ) ]
	cmpt = 0
	vertex_name = dict()
	pos = dict()

	pos[ 's' ] = ( -1, 0 )
	pos[ 'p' ] = ( numCol, 0 )

	# Créations des sommets centraux
	for i in range( numCol ) :
		for j in range( columns[i] ) :
			vertex_name[ (i, j) ] = cmpt
			pos[ cmpt ] = (i, j - (columns[i]-1)/2)
			G.add_vertex( cmpt )
			cmpt += 1

	# 's' est relié aux sommets centraux de la 1ere colonne
	for i in range( columns[0] ) :
		u = vertex_name[ (0, i) ] 
		G.add_edge( ('s', u, capacity_fct() ) )


	# création de liens aléatoirement entre les sommets centraux de colonne
	# adjacentes
	for i in range( numCol - 1 ) :
		for u in range( columns[i] ) :
			for v in range( columns[i+1] ) :
				U = vertex_name[ (i,u) ]
				V = vertex_name[ (i+1, v) ]
				if proba ( (prob_avant)^(1+abs(pos[U][1]-pos[V][1])) ) :
					G.add_edge( U, V, capacity_fct() )
				if proba ( (prob_arriere)^(1+abs(pos[U][1]-pos[V][1])) ) :
					G.add_edge( V, U, capacity_fct() )

	# les sommets de la dernières colonne sont reliés à 'p'
	for i in range( columns[-1] ) :
		u = vertex_name[ ( numCol - 1, i ) ] 
		G.add_edge( (u, 'p', capacity_fct() ) )
	G.set_pos(pos)
	if G.distance('s','p') == oo :
		return randomNetwork ( numCol, minCol, maxCol, prob_avant, prob_arriere, capacity_fct ) 
	return G




def couleurDistance( d, dmax ):
     cmap = colormaps['rainbow_r']
     if ( d < 0.0 ):
         d = 0.0
     elif ( d > dmax ):
         d = dmax
     c = cmap( float(d)/float(dmax) )
     return c[0:3]




def setEdgeColor( d, e, val ) :
	"""
	Ajoute l'information nécessaire au dictionnaire ``d`` pour que la couleur de
	l'arc ``e`` soit fixée en fonction de ``val``, un nombre entre 0 et 1. 

	Plus ``val`` approche 1, plus l'arc prend une teinte rouge.
	"""
	couleur = couleurDistance ( 1-val, 1 )
	if d.has_key(couleur) :
		d[couleur].append(e)
	else :
		d[couleur] = [e]


def initReseauBobo ( n ) :
	"""
	Retourne un réseau bien particulier.
	"""
	R = DiGraph()
	R.add_edges ( [ ('s', 0, n), ('s', 1, n), (0,1,1), (0, 'p', n), (1, 'p', n) ] )
	pos = { 's' : (0,0), 'p' : (2,0), 0 : (1,1), 1 : (1,-1) }
	R.set_pos( pos )
	return R












