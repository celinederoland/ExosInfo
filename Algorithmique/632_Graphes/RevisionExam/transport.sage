def initFlot( d ):
	dico = dict();
	for arete in d:
		dico[arete] = 0
		dico[(arete[1],arete[0])] = 0
	return dico

def afficheFlot( R, d, f ):
	G = copy(R)
	for arete in R.edges():
		couple = (arete[0],arete[1])
		capacite = d[couple]
		flot = f[couple]
		if flot != 0:
			label = "%d [%d]" % (capacite,flot)
			G.set_edge_label(arete[0],arete[1],label)
	return G

def valeurFlot( R, f ):
	S = 0
	for v in G.neighbors_out('s'):
		S = S + f[('s',v)]
	return S

def residuel( R, d, f ):
	res = DiGraph()
	d2 = dict()
	res.add_vertices( R.vertices() )
	for arete in d:
		couple = (arete[0],arete[1])
		capacite = d[couple]
		flot = f[couple]
		if (capacite - flot) > 0 :
			label = "%d" % (capacite - flot)
			res.add_edge(couple)
			res.set_edge_label(arete[0],arete[1],label)
			d2[couple] = capacite - flot
	return (res,d2)

def iterationFordFulkerson(R,d,f):
	S = cheminLargeur(R,'s','p')
	if S != []:
		A = list() 
		capacite = list()
		l = len(S)
		for i in range(l-1):
			A.append((S[i],S[i+1]))
			capacite.append(d[(S[i],S[i+1])])
		ajout = min(capacite)
		for arc in A:
			f[arc] = f[arc] + ajout
			f[(arc[1],arc[0])] = - f[arc]
		return True
	return False

def FordFulkerson(R,d):
	f = initFlot(d)
	res = copy(R)
	d2 = copy(d)
	while iterationFordFulkerson(res,d2,f):
		(res,d2) = residuel(R,d2,f)
	return f
