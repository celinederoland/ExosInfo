def Prim(G,d,s):
	pere = list()
	vu = list()
	cle = list()
	g = Graph()
	for i in G:
		cle.append(Infinity)
		vu.append(False)
		pere.append(None)
	cle[s] = 0
	pere[s] = None
	g.add_vertex(s)
	while (resteNonTraites(vu)):
		u = minParmisNonTraites(cle,vu)
		vu[u] = True
		g.add_vertex(u)
		g.add_edge((pere[u],u))
		for v in G.neighbors(u):
			if (not vu[v]) and (d[(u,v)] < cle[v]):
				cle[v] = d[(u,v)]
				pere[v] = u
	return g

def Kruskal(G,d,s):
	U = UnionFind()
	g = Graph()
	for v in G:
		U.creerEnsemble(v)
		g.add_vertex(v)
	l = trierAretes(d)
	for arete in l:
		if (U.trouverEnsemble(arete[0]) != U.trouverEnsemble(arete[1])):
			g.add_edge(arete)
			U.unionEnsemble(arete[0],arete[1])
	return (U._g,g)

def Dijkstra(G,d,s):
	pere = list()
	vu = list()
	cle = list()
	g = DiGraph()
	for i in G:
		cle.append(Infinity)
		vu.append(False)
		pere.append(None)
	cle[s] = 0
	pere[s] = None
	u = minParmisNonTraites(cle,vu)
	while (u != None):
		print u
		vu[u] = True
		g.add_vertex(u)
		if (pere[u] != None):
			g.add_edge((pere[u],u))
		for v in G.neighbors_out(u):
			if (not vu[v]) and (cle[u] + d[(u,v)] < cle[v]):
				cle[v] = cle[u] + d[(u,v)]
				pere[v] = u
		u = minParmisNonTraites(cle,vu)
	return g

def FloydWarshall(M):
	n = M.nrows()
	D = copy(M)
	I = matrix(n,n)
	for i in range(0,n):
		for j in range(0,n):
			I[i,j] = -1
			if (D[i,j] == 0):
				D[i,j] = Infinity
	for k in range(0,n):
		for i in range(0,n):
			for j in range(0,n):
				if (D[i,k]+D[k,j] < D[i,j]):
					D[i,j] = D[i,k] + D[k,j]
					I[i,j] = k
	return (D,I)

def chemin(D,I,u,v):
	if D[u,v] == Infinity:
		return None
	if I[u,v] == -1:
		return [u,v]
	else:
		l1 = chemin(D,I,u,I[u,v])
		l2 = chemin(D,I,I[u,v],v)
		l = joindre(l1,l2)
		return l

def joindre(l1,l2):
	l = l1
	l.pop()
	l.extend(l2)
	return l

def arbreMin(D,I,s):
	n = I.nrows()
	g = DiGraph()
	pere = []
	vu = []
	for i in range(n):
		g.add_vertex(i)
		pere.append(None)
		vu.append(False)
	vu[s] = True
	for u in range(n):
		if u != s:
			l = chemin(D,I,s,u)
			print l
			if (l != None):
				v = u
				l.pop()
				while (not vu[v]):
					print v
					vu[v] = True
					w = l.pop()
					pere[v] = w
					g.add_edge(w,v)
					v = w
	print pere
	return g

def estFortementConnexe(D):
	n = D.nrows()
	for i in range(n):
		if D[0,i] == Infinity or D[i,0] == Infinity:
			return False
	return True

def cheminEtendu(M2,D,I,u):
	M = copy(M2)
	n = M.nrows() - 1
	for i in range(n+1):
		for j in range(n+1):
			if (M[i,j] == 0):
				M[i,j] = Infinity
	taille = M[u,n]
	actual = -1
	for i in range(n):
		if D[u,i] + M[i,n] < taille:
			actual = i
			taille = D[u,i] + M[i,n]
	if taille == Infinity:
		return None
	if (actual == -1):
		return [u,n]
	else:
		l = chemin(D,I,u,actual)
		l.append(n)
		return l

def trierAretes(d):
	l = list()
	dico = d.copy()
	for arete in d:
		l.append(mindico(dico))
	return l

def mindico(d):
	minimum = Infinity
	for arete in d.keys():
		if d[arete] < minimum:
			minimum = d[arete]
			indicemin = arete
	d[indicemin] = Infinity
	return indicemin

def resteNonTraites(T):
	for t in T:
		if (not t):
			return True
	return False

def minParmisNonTraites(T,V):
	minimum = Infinity
	indice = 0
	indicemin = None
	for t in T:
		if (not V[indice]) and (t < minimum):
			minimum = t
			indicemin = indice
		indice += 1
	return indicemin 
