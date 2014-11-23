def uniforme( a, b ) :
	return lambda : floor ( uniform( a, b ) )

def proba( p ) :
	return uniform(0,1) < p

def randomNetworkSimple( n, prob_avant = 0.7, prob_arriere = 0.3 ) :
	numCol = floor( sqrt(n) )
	minCol = numCol//2
	maxCol =(3*numCol)//2
	cap = uniforme(1, 10)
	return randomNetwork( numCol, minCol, maxCol, prob_avant, prob_arriere, cap )



def randomNetwork( numCol, minCol, maxCol, prob_avant, prob_arriere, capacity_fct ) :
	G = DiGraph()
	columns = [ uniforme(minCol, maxCol )() for i in range( numCol ) ]
	cmpt = 0
	vertex_name = dict()
	d = dict()

	# Créations des sommets centraux
	for i in range( numCol ) :
		for j in range( columns[i] ) :
			vertex_name[ (i, j) ] = cmpt
			G.add_vertex( cmpt )
			cmpt += 1

	# 's' est relié aux sommets centraux de la 1ere colonne
	for i in range( columns[0] ) :
		u = vertex_name[ (0, i) ] 
		alea = capacity_fct()
		d[('s',u)] = alea
		G.add_edge( ('s', u) )
		label = "%d" % (alea)
		G.set_edge_label('s',u,label)


	# création de liens aléatoirement entre les sommets centraux de colonne
	# adjacentes
	for i in range( numCol - 1 ) :
		for u in range( columns[i] ) :
			for v in range( columns[i+1] ) :
				U = vertex_name[ (i,u) ]
				V = vertex_name[ (i+1, v) ]
				if proba ( prob_avant ) :
					alea = capacity_fct()
					d[(U,V)] = alea
					G.add_edge( U, V )
					label = "%d" % (alea)
					G.set_edge_label(U,V,label)
				if proba ( prob_arriere ) :
					alea = capacity_fct()
					d[(V,U)] = alea
					G.add_edge( V, U )
					label = "%d" % (alea)
					G.set_edge_label(V,U,label)

	# les sommets de la dernières colonne sont reliés à 'p'
	for i in range( columns[-1] ) :
		u = vertex_name[ ( numCol - 1, i ) ] 
		alea = capacity_fct()
		d[(u,'p')] = alea
		G.add_edge( (u, 'p' ) )
		label = "%d" % (alea)
		G.set_edge_label(u,'p',label)
	if G.distance('s','p') == oo :
		return randomNetwork ( numCol, minCol, maxCol, prob_avant, prob_arriere, capacity_fct ) 
	return (G,d)

