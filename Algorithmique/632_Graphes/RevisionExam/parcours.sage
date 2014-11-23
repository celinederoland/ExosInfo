def estTransitif(G):
	for u in G:
		for v in G.neighbors(u):
			for w in G.neighbors(v):
				if (u != w) and (not G.has_edge(u,w)):
					return false
	return true

def rendreTransitif(G):
	g = G.copy()
	termine = False
	while (not termine):
		termine = True
		for u in g:
			for v in g.neighbors(u):
				for w in g.neighbors(v):
					if (u != w) and (not g.has_edge(u,w)):
						g.add_edge( (u,w) )
						termine = False
	return g

def arbreCouvrantLargeur(G,s):
	vu = list()
	pere = list()
	g = Graph()
	F = File()
	for i in G:
		vu.append(False)
		pere.append(None)
	vu[s] = 0
	F.enfile(s)
	while (not F.estVide()):
		u = F.defile()
		for v in G.neighbors(u):
			if (not vu[v]):
				vu[v] = True
				pere[v] = u
				g.add_vertex(v)
				g.add_edge(u,v)
				F.enfile(v)
	return g

def cheminLargeur(G,u,v):
	vu = dict()
	pere = dict()
	F = list()
	for s in G.vertices():
		vu[s] = False
	pere[u] = None
	vu[u] = True
	F.append(u)
	while len(F) != 0 :
		s = F.pop(0)
		for w in G.neighbors_out(s):
			if (vu[w] == False):
				vu[w] = True
				pere[w] = s
				if (w == v):
					return retrouverChemin(pere,u,v)
				F.append(w)
	return list()

def cheminLargeurNO(G,u,v):
	vu = dict()
	pere = dict()
	F = list()
	for s in G.vertices():
		vu[s] = False
	pere[u] = None
	vu[u] = True
	F.append(u)
	while len(F) != 0 :
		s = F.pop(0)
		for w in G.neighbors(s):
			if (vu[w] == False):
				vu[w] = True
				pere[w] = s
				if (w == v):
					return retrouverChemin(pere,u,v)
				F.append(w)
	return list()

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

def arbreCouvrantProfondeur(G,s):
	vu = list()
	pere = list()
	g = Graph()
	for i in G:
		vu.append(False)
		pere.append(None)
	visiterPP(G,s,vu,pere,g)
	return g

def visiterPP(G,s,vu,pere,g):
	vu[s] = True
	for v in G.neighbors(s):
		if (not vu[v]):
			pere[v] = s
			g.add_vertex(v)
			g.add_edge( (s,v) )
			visiterPP(G,v,vu,pere,g)

def arbreCouvrantProfondeurSansRec(G,s):
	vu = list()
	pere = list()
	g = Graph()
	P = Pile()
	for i in G:
		vu.append(False)
		pere.append(None)
	vu[s] = True
	pere[s] = None
	voisins = G.neighbors(s)
	voisins.reverse()
	P.empile( (s,voisins) )
	while (not P.estVide()):
		L = P.depile()
		u = L[0]
		vu[u] = True
		g.add_vertex(u)
		if (pere[u] != None):
			g.add_edge( (pere[u],u) )
		f = L[1]
		fini = False
		while (not fini and len(f) != 0):
			v = f.pop()
			if (not vu[v]):
				pere[v] = u
				P.empile(L)
				voisins = G.neighbors(v)
				voisins.reverse()
				P.empile( (v,voisins) )
				fini = True
	return g

def estBiparti(G):
	coul = list()
	F = File()
	for i in G:
		coul.append(None)
	for s in G:
		if (coul[s] == None):
			coul[s] = 0
			F.enfile(s)
			while (not F.estVide()):
				u = F.defile()
				for v in G.neighbors(u):
					if (coul[v] != None):
						if (coul[v] == coul[u]):
							return False
					else:
						coul[v] = 1 - coul[u]
						F.enfile(v)
	return True

def acyclique(G):
	vu = list()
	pere = list()
	F = File()
	for i in G:
		vu.append(False)
		pere.append(None)
	for s in G:
		if (not vu[s]):
			F.enfile(s)
			vu[s] = True
			while (not F.estVide()):
				u = F.defile()
				for v in G.neighbors(u):
					if (vu[v] and pere[u] != v):
						return False
					if (not vu[v]):
						pere[v] = u
						vu[v] = True
						F.enfile(v)
	return True

def couper(G,u,v):
	g = copy(G)
	S = 0
	L = cheminLargeurNO(g,u,v)
	while (L != []):
		S += 1
		g.delete_edge((L[0],L[1]))
		L = cheminLargeurNO(g,u,v)
	return (g,S)

def choisirVoisin(G,u,vu):
	for v in G.neighbors(u):
		if not vu[(u,v)]:
			return v
	return None

def initVu(G,vu):
	for arete in G.edges():
		vu[(arete[0],arete[1])] = false
		vu[(arete[1],arete[0])] = false

def balade(G,s,vu):
	u = s
	L = [u]
	v = choisirVoisin(G,u,vu)
	while (v != s and v != None):
		L.append(v)
		vu[(u,v)] = True
		vu[(v,u)] = True
		u = v
		v = choisirVoisin(G,u,vu)
	if (v == s):
		L.append(s)
		return L
	return []

def choisirNonVu(vu):
	for arete in vu:
		if (not vu[arete]):
			return arete[0]
	return None

def cycleEulerien(G):
	vu = dict()
	initVu(G,vu)
	s = choisirNonVu(vu)
	if (s == None):
		return False
	l = balade(G,s,vu)
	L = l
	while (l != []):
		l = balade(G,s,vu)
		L.extend(l)
	if (choisirNonVu(vu) == None):
		return L
	else:
		return []
