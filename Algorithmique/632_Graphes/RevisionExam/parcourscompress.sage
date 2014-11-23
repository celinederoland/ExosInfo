def estTransitif(G):
	for u in G: for v in G.neighbors(u): for w in G.neighbors(v):
		if (u != w) and (not G.has_edge(u,w)): return false
	return true

def rendreTransitif(G):
	while (not termine):
		termine = True
		for u in g: for v in g.neighbors(u): for w in g.neighbors(v):
			if (u != w) and (not g.has_edge(u,w)): g.add_edge( (u,w) ) ; termine = False
	return g

def arbreCouvrantLargeur(G,s):
	F.enfile(s)
	while (not F.estVide()): u = F.defile()
		for v in G.neighbors(u): if (not vu[v]):
			vu[v] = True ; pere[v] = u ; F.enfile(v)

def arbreCouvrantProfondeur(G,s):
	visiterPP(G,s,vu,pere)

def visiterPP(G,s,vu,pere):
	vu[s] = True
	for v in G.neighbors(s): if (not vu[v]):
		pere[v] = s ; visiterPP(G,v,vu,pere)

def estBiparti(G):
	coul = list() ; F = File() ; for i in G: coul.append(None)
	for s in G: if (coul[s] == None):
		coul[s] = 0 ; F.enfile(s)
		while (not F.estVide()): u = F.defile()
			for v in G.neighbors(u):
				if (coul[v] != None): if (coul[v] == coul[u]): return False
				else: coul[v] = 1 - coul[u] ; F.enfile(v)
	return True

def acyclique(G):
	for s in G: if (not vu[s]): F.enfile(s) ; vu[s] = True
		while (not F.estVide()): u = F.defile()
			for v in G.neighbors(u):
				if (vu[v] and pere[u] != v): return False
				if (not vu[v]): pere[v] = u ; vu[v] = True ; F.enfile(v)
	return True
