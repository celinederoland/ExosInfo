def generer(n,proportion):
	G = Graph()
	G.add_vertices(range(n))
	d = dict()
	for i in range(n):
		for j in range(n-1-i):
			k = ZZ.random_element(0,proportion)
			l = ZZ.random_element(1,15)
			if (k == 0):
				G.add_edge( (i,i+j+1) )
				label = "%d" % (l)
				G.set_edge_label(i,i+j+1,label)
				d[(i,i+j+1)] = l
				d[(i+j+1,i)] = l
	return (G,d)

def genererOriente(n,proportion):
	G = DiGraph()
	G.add_vertices(range(n))
	d = dict()
	for i in range(n):
		for j in range(n):
			k = ZZ.random_element(0,proportion)
			l = ZZ.random_element(1,15)
			if (k == 0 and not G.has_edge(i,j)):
				G.add_edge( (i,j) )
				label = "%d" % (l)
				G.set_edge_label(i,j,label)
				d[(i,j)] = l
	return (G,d)

def ajouterSommet(G,d,proportion):
	g = G.copy()
	D = d.copy()
	n = len(G.vertices())
	g.add_vertex(n)
	for i in range(n):
		k = ZZ.random_element(0,proportion)
		k2 = ZZ.random_element(0,proportion)
		l = ZZ.random_element(1,15)
		l2 = ZZ.random_element(1,15)
		if (k == 0 and not g.has_edge(i,n)):
			g.add_edge( (i,n) )
			label = "%d" % (l)
			g.set_edge_label(i,n,label)
			D[(i,n)] = l
		if (k2 == 0 and not g.has_edge(n,i)):
			g.add_edge( (n,i) )
			label = "%d" % (l2)
			g.set_edge_label(n,i,label)
			D[(n,i)] = l2
	return (g,D)

def graphToMatrice(G,d):
	n = len(G.vertices())
	M = matrix(RR,n,n)
	for i in range(n):
		for j in range(n):
			if G.has_edge(i,j):
				M[i,j] = d[(i,j)]
	return M

def matriceToGraph(M):
	n = M.nrows()
	G = DiGraph()
	d = dict()
	G.add_vertices(range(n))
	for i in range(n):
		for j in range(n):
			if M[i,j] != 0:
				G.add_edge( (i,j) )
				label = "%d" % (M[i,j])
				G.set_edge_label(i,j,label)
				d[(i,j)] = M[i,j]
	return (G,d)
