class Cellule :
	"""
	Classe dont les objets constituent les cellules d'une liste chainÃ©e.
	"""

	def __init__(self, val, pred, succ) :
		"""
		Le constructeur prend en paramÃ¨tre :
		'val' la valeur Ã  affecter dans la cellule,
		'pred' l'Ã©lÃ©ment prÃ©cÃ©dent dans la liste,
		'succ' l'Ã©lÃ©ment suivant dans la liste.
		ComplexitÃ© en temps : O(1)
		"""
		self._valeur = val
		self._pred = pred
		self._succ = succ

	def valeur ( self ) :
		"""
		Retourne la valeur contenue dans la cellule.
		ComplexitÃ© en temps : O(1)
		"""
		return self._valeur

	def succ ( self ) :
		"""
		Retourne l'Ã©lÃ©ment suivant de la liste.
		ComplexitÃ© en temps : O(1)
		"""
		return self._succ

	def pred ( self ) :
		"""
		Retourne l'Ã©lÃ©ment prÃ©cÃ©dent dans la liste.
		ComplexitÃ© en temps : O(1)
		"""
		return self._pred

	def retirer ( self ) :
		"""
		Retire la cellule courante de la liste.
		ComplexitÃ© en temps : O(1)
		"""
		self._pred._succ = self._succ
		self._succ._pred = self._pred

	def __repr__( self ) :
		"""
		Retourne une chaine de caractÃ¨re reprÃ©sentant cette cellule,
		le format est :
		' p <--> self.valeur() <--> s"
		oÃ¹ 
		'p' est la valeur de la cellule prÃ©cÃ©dent si elle est valide, X sinon.
		's' est la valeur de la cellule suivant si elle est valide, X sinon.
		ComplexitÃ© en temps : O(1)
		"""
		if isinstance( self._pred , Cellule ) :
			s = str(self._pred._valeur)
		else :
			s = "X"
		s += " <--> " + str(self._valeur) + " <--> "
		if isinstance( self._succ , Cellule ) :
			s += str(self._succ._valeur)
		else :
			s += "X"
		return s

class Liste :
	"""
	ImplÃ©mentation de listes chainÃ©es dont les Ã©lÃ©ments sont de type 'Cellule'.
	"""
	def __init__(self) :
		"""
		Constructeur par dÃ©faut, initialise une liste vide.
		"""
		self._succ = self
		self._pred = self

	def debut ( self ) :
		"""
		Retourne le premier Ã©lÃ©ment de la liste.
		ComplexitÃ© en temps : O(1)
		"""
		return self._succ

	def fin ( self ) :
		"""
		Retourne l'Ã©lÃ©ment de fin de liste. Ce Ã©lÃ©ment n'est pas une cellule.
		Il ne possÃ¨de donc pas de fonction 'valeur()'
		ComplexitÃ© en temps : O(1)
		"""
		return self

	def insererDebut( self, val ) :
		"""
		InsÃ¨re l'Ã©lÃ©ment 'val' en dÃ©but de liste.
		ComplexitÃ© en temps : O(1)
		"""
		c = Cellule( val, self, self._succ )
		self._succ._pred = c
		self._succ = c
	
	def insererFin( self, val ) :
		"""
		InsÃ¨re l'Ã©lÃ©ment 'val' en fin de liste.
		ComplexitÃ© en temps : O(1)
		"""
		c = Cellule( val, self._pred, self )
		self._pred._succ = c
		self._pred = c

	def estVide ( self ) :
		"""
		Retourne 'True' si la liste est vide, 'False' sinon.
		ComplexitÃ© en temps : O(1)
		"""
		return self.debut() == self.fin()

	def __repr__( self ) :
		"""
		Retourne une chaine de caratÃ¨res reprÃ©sentant la liste.
		ComplexitÃ© en temps : O(n) pour une liste de taille n.
		"""
		if self.debut() == self.fin() :
			return "[]"
		s = "["
		i = self.debut()
		while ( i != self.fin() ) :
			s += str( i.valeur() ) + "," 
			i = i.succ()
		return s[:-1] + "]"

	def copie(self) :
		"""
		Retourne une nouvelle liste avec le mÃªme contenue la liste.
		ComplexitÃ© en temps : O(n) pour une liste de taille n.
		"""
		maCopie = Liste()
		i = self.debut()
		while i != self.fin() :
			maCopie.insererFin( i.valeur() )
			i = i.succ()
		return maCopie


class File :
	"""
	ImplÃ©mentation d'une file
	"""
	def __init__ ( self, liste = [] ) :
		"""
		Constructeur. Initialise une file vide.
		ComplexitÃ© en temps : O(1)
		"""
		self._liste = liste
	
	def enfile( self, elem ) :
		"""
		Ajoute 'elem' au dÃ©but de la file.
		ComplexitÃ© en temps : O(1)
		"""
		self._liste.reverse()
		self._liste.append(elem)
		self._liste.reverse()
	
	def defile( self ) :
		"""
		DÃ©pile l'Ã©lÃ©ment au sommet de la pile et le retourne.
		ComplexitÃ© en temps : O(1)
		"""
		return self._liste.pop()

	def estVide( self ) :
		"""
		Retourne 'True' si la pile est vide, 'False' sinon.
		ComplexitÃ© en temps : O(1)
		"""
		return len(self._liste) == 0

class Pile :
	"""
	ImplÃ©mentation d'une pile
	"""
	def __init__ ( self ) :
		"""
		Constructeur. Initialise une pile vide.
		ComplexitÃ© en temps : O(1)
		"""
		self._liste = []
	
	def empile( self, elem ) :
		"""
		Ajoute 'elem' sur le sommet de la pile.
		ComplexitÃ© en temps : O(1)
		"""
		self._liste.append(elem)
	
	def depile( self ) :
		"""
		DÃ©pile l'Ã©lÃ©ment au sommet de la pile et le retourne.
		ComplexitÃ© en temps : O(1)
		"""
		return self._liste.pop()

	def estVide( self ) :
		"""
		Retourne 'True' si la pile est vide, 'False' sinon.
		ComplexitÃ© en temps : O(1)
		"""
		return len(self._liste) == 0

class UnionFind :

	def __init__(self):
		self._g = DiGraph()
		self._rangs = dict()

	def creerEnsemble(self,val):
		self._g.add_vertex(val)
		self._rangs[val] = 0

	def pere(self,val):
		l = self._g.neighbors_in(val)
		for p in l:
			return p
		return val

	def setPere(self,val,nouveauPere):
		self._g.delete_edge( (self.pere(val),val) )
		self._g.add_edge( (nouveauPere,val) )

	def trouverEnsemble(self, elem):
		y = elem
		while self.pere(y) != y:
			y = self.pere(y)
		return y
		

	def unionEnsemble(self, elem1, elem2):
		rep1 = self.trouverEnsemble(elem1)
		rep2 = self.trouverEnsemble(elem2)
		if (rep1 != rep2):
			if (self._rangs[rep1] > self._rangs[rep2]):
				self.setPere(rep2,rep1)
			else :
				self.setPere(rep1,rep2)
			if (self._rangs[rep1] == self._rangs[rep2]):
				self._rangs[rep2] += 1

