#!/usr/bin/env python
# -*- coding: utf-8-unix -*-

from sys import argv,exit,maxint
from random import randint
from math import log
from commands import getstatusoutput
from os import popen

FICHIER_DICTIONNAIRE = "/usr/share/dict/french"
INFINI = maxint # défini dans sys 
INFINI_EN_TEXTE = '+INF' 

class ListeDeMots :

    def __init__( self, data ) :
        """
        Trois manière d'initialiser une liste de mots :

        1. Avec un texte
        Exemples ::

        : l = ListeDeMots( "Ceci est une exemple de texte" )
        : print l
        ListeDeMots( ['Ceci', 'est', 'une', 'exemple', 'de', 'texte'] )

        2. Avec une liste de chaînes de caractères
        Exemples ::

        : l = ListeDeMots( ['Ceci', 'est', 'une', 'exemple', 'de', 'texte'] )
        : print l
        ListeDeMots( ['Ceci', 'est', 'une', 'exemple', 'de', 'texte'] )

        3. Avec un entier. Dans ce cas, le fichier ``FICHIER_DICTIONNAIRE`` est
        chargé. Ce fichier doit être un fichier texte contenant une liste de
        mots, avec un seul mot par ligne. L'entier passé en entrée désigne alors 
        le nombre de mots pigés aléatoirement dans ce fichier.
        Exemple ::

        : l = ListeDeMots( 5 )

        : print l
        ListeDeMots( ['flibustier', 'vinyle', 'silencieuse', 'corner', 'introduite'] )
        """
        iterable = False
        try : 
            iter( data )
            iterable = True
        except TypeError :
            pass
        if isinstance( data, str ) :
            self.mots = data.split()
        elif iterable :
            self.mots = data
        elif isinstance( data, int ) :
            nb_mots = data
            f = open( FICHIER_DICTIONNAIRE, 'r' )
            l = f.readlines()
            f.close()
            n = len( l )
            self.mots = []
            while len( self.mots ) < nb_mots :
                m = l[ randint( 0, n-1 ) ][:-1] # on retire le caractère '\n' en fin de ligne
                ok = True
                for i in m :
                    ok = ('A' <= i <= 'Z') or ('a' <= i <= 'z') #seulement des lettres
                    if not ok :
                        break
                if ok :
                    self.mots.append(m)
        else :
            raise TypeError( "Type de l'entrée (={}) non valide, les entrées attendues sont : liste de mots, entier".format( type( data ) ) )

    def __repr__( self ) :
        return "ListeDeMots( {} )".format( self.mots )

    def nb_mots( self ) :
        return len( self.mots )

    def get_mot( self, n ) :
        """
        Retourne le n-ième mot de la liste.
        Note : les mots sont numérotés de 1 à n
        """
        if 0 < n <= self.nb_mots() :
            return self.mots[ n-1 ]
        else :
            raise IndexError( "L'index demandé (={}) devrait être de 1 à {}".format( \
                               n, self.nb_mots() ) )

    def lng( self, i, j ) :
        """
        Retourne le nombre de caractère utilisée par les mots 
        l[i], l[i+1], ..., l[j]

        On suppose qu'on insère au moins un espace entre chaque mots.
        """
        if i > j :
            # Sous suite vide, on retourne 0
            return 0
        else :
            return j-i + sum( [ len( self.get_mot(k) ) for k in range(i,j+1) ] )

    def lng_max( self ) :
        return max( map( len, self.mots ) )

    def disposer_sur_une_ligne( self, debut, fin, M ) :
        """
        Dispose les mot de la position ``debut`` jusqu'à ``fin`` sur une ligne
        dont le nombre de caractères est exactement ``M``.

        Retourne une chaine de caractère contenant la ligne formatée.
        """
        if debut == fin :
            # si un seul mot, on l'imprime tout simplement au début de la ligne
            s = self.get_mot(debut)
        else :
            l = [ self.get_mot( i ) for i in range( debut, fin+1 ) ]
            nb_char = sum( map( len, l) )
            nb_espaces = M - nb_char
            nb_intermots = len(l) - 1
            # On réparti les espaces le plus uniformément possible parmi les
            # intermots. Sans détailler la méthode employé, il s'agit de
            # répartir de la manière la plus équilibré possible les grands et
            # les petits espaces.
            nb_espaces_par_intermots = nb_espaces / nb_intermots
            reste = nb_espaces % nb_intermots
            nb_petits = nb_intermots - reste
            nb_grands = reste
            i = nb_grands
            s = l[0]
            for m in l[1:] :
                s += ' '*nb_espaces_par_intermots
                if i+nb_grands < nb_intermots :
                    i += nb_grands
                else :
                    i -= nb_petits
                    s += ' '
                s += m
        return s




class Matrice :

    def __init__( self, n, m ) :
        """
        Initialise une matrice à ``n`` lignes et ``m`` colonnes.
        """
        self.nb_lignes = n
        self.nb_cols = m
        self.m = [ [ 0 for j in range(m) ] for i in range(n) ]

    def __getitem__( self, ( i, j ) ) :
        return self.m[i][j]

    def __setitem__( self, ( i, j ), val ) :
        self.m[i][j] = val

    def __repr__( self ) :
        def nb_char( n ) :
            if n >= maxint :
                return len( INFINI_EN_TEXTE )
            else :
                return len( str(n) )

        def to_str( n, lng ) :
            if n >= maxint :
                return ( ' '*(lng-len(INFINI_EN_TEXTE)) ) + INFINI_EN_TEXTE
            else :
                return ( ' '*(lng-nb_char(n)) ) + str(n)

        # Pour chaque colonne, on calcule le nombre de décimales nécessaires
        largeur_col = [ 0 for i in range( self.nb_cols ) ]
        for j in range( self.nb_cols ) : 
            for i in range( self.nb_lignes ) :
                largeur_col[j] = max( largeur_col[j], nb_char( self.m[i][j] ) )
        s = ""
        for i in range( self.nb_lignes ) :
            l = ""
            for j in range( self.nb_cols ) :
                l += to_str( self.m[i][j], largeur_col[j] ) + ", "
            s += '[' + l[:-2] + ']\n'
        return s[:-1]


def penalite( debut, fin, M, L ) :
    """
    Pénalité pour une ligne de taille ``M`` contenant les mots de ``debut`` à ``fin``
    de la ListeDeMots ``L``.

    Note, si debut=0 et fin=0 alors on considère que la ligne est vide.
    """
    if debut == 0 and fin == 0 :
        n = 0
    else :
        n = L.lng( debut, fin )
    if n > M :
        return INFINI
    else :
        return ( M - n ) ** 3


def meilleurDispo( L, M ) :
    """
    Calcule une disposition optimale des mots de la ListeDeMots ``L`` sur des
    lignes de taille ``M``.

    Retourne une liste d'entiers telle que si le k-ieme entier indique le 
    nombre de mots à disposer sur la k-ième ligne.
    """
    n = L.nb_mots()

    # La matrice W a l'interprétation suivante :  
    # W[i][j] est la pénalité minimale pour disposer les j premiers mots sur i
    # lignes.
    # La matrice W compte n+1 colonnes car on veut qu'elles soient indexées par
    # 0, 1, 2, ..., n
    # On remarque que le nombre de lignes de la matrice est également n+1 car,
    # au pire cas, on disposera tous les mots sur les lignes différentes. Il
    # faut donc prévoir autant de lignes que de mots. Comme on ne peut pas
    # disposer de mots sur zéro lignes, la première ligne de la matrice ne sera
    # pas utilité.
    W = Matrice( n+1, n+1 )

    # La matrice N a l'interprétation suivante :  
    # N[i][j] est le nombre de mots sur la dernière ligne pour une disposition
    # optimale des j premiers mots sur i lignes.
    N = Matrice( n+1, n+1 )

    # Initialisation de la m1atrice W.
    # Disposer 0 mots sur i lignes signifie qu'on a i lignes vides. On sait
    # calculer la pénalité pour une ligne vide alors on rempli tout de suite la
    # première colonne de la matrice W.
    pen_pour_une_ligne_vide = penalite( 0,0,M,L ) # =0 car debut = fin = 0
    for i in range( 0, n+1 ) :
        W[i,0] = i * pen_pour_une_ligne_vide

    # La pénalité minimum pour disposer j mots sur une seule ligne est facile à
    # calculer puisqu'il n'y a qu'une seule façon de disposer j mots sur une
    # seule ligne.
    for j in range( 0, n+1 ) :
        W[1,j] = penalite( 1, j, M, L )
        N[1,j] = j # tous les mots sont sur la dernière ligne considérée


    ##################################
    ##################################
    #
    # DÉBUT DE L'EXERCICE
    #
    #

	# Décrivez en mot la manière dont vous calculez la valeur d'une case W[i,j]

	# Pour placer j mots sur i lignes, je vais décomposer j en sommes de 2 nombres :
		# j = a + (j-a) (correspond à disposer les a premiers mots sur i-1 lignes et les j-a derniers mots sur la ligne i).
		# avec la définition donnée pour a, la pénalité est W[i-1,a] + Penalite(a + 1,j,M,L)
		# il faut donc parcourir toutes les valeurs possibles de a (de 0 à j)
    # la valeur minimale de W[i-1,a] + Penalite(a + 1,j,M,L) est W[i,j]
		# a est le nombre de mots placés sur les i-1 premières ligne, donc N[i,j] = j-a 

  # Remarque : on voit bien ici l'intérêt de la programmation dynamique : on a retenu dans les matrices W et N les résultats précédents,
  #  et on utilise ces résultats pour les calculs suivants :
  #  on utilise la ligne i-1 de W pour calculer la ligne i
  #  on utilise à la fin la matrice N pour reconstituer la disposition optimale trouvée

    # Initialisation de la matrice W à INFINI pour les valeurs par encore calculées
    for i in range( 2, n+1 ) :
        for j in range( 1, n+1 ) :
            W[i,j] = INFINI

    # Calcul des pénalités minimales utiles
    for i in range( 2, n+1 ) :
        for j in range( 1, n+1 ) :
            pen_min_pour_j_mots_sur_i_lignes = INFINI 
            nb_mots_i_moins_1_premieres_lignes_optimal = 0
            for a in range( 0, j ) :
                # on ne trouvera plus de minimum si un des termes de la somme est infini
                if (W[i-1,a] == INFINI) :
                    break 
                # on regarde si a minimise la pénalité
                candidat = W[i-1,a] + penalite(a+1, j, M, L)
                if ( candidat < pen_min_pour_j_mots_sur_i_lignes) :
                    pen_min_pour_j_mots_sur_i_lignes = candidat
                    nb_mots_i_moins_1_premieres_lignes_optimal = a

            W[i,j] = pen_min_pour_j_mots_sur_i_lignes
            N[i,j] = j - nb_mots_i_moins_1_premieres_lignse_optimal

				# si il était plus intéressant de disposer tous les mots sur i-1 lignes que sur i lignes,
        # c'est qu'on a dépassé le nombre de lignes optimal, inutile donc de continuer.
        if (W[i,n] > W[i-1,n]) :
            break

    #
    #
    # FIN DE L'EXERCICE  !!!
    #################################
    #################################

    # Ok, maintenant qu'on a calculé toutes les dispositions optimales, il
    # reste à déterminercommant disposer les mots.
    # 
    # On rappelle que la dernière ligne ne produit pas de pénalité.
    # 
    # On va donc commencer par calculer le nombre maximum de mots qu'on peut
    # disposer sur la dernière ligne. On appelle ce nombre k.
    #
    # On va chercher la disposition de pénalité minimale pour un nombre de mot
    # allant de : n-k à n. On va donc chercher la pénalité minimale pour
    # disposer de n-k à n mots sur n'importe quel nombre de lignes.
    # 
    k = 0
    while ( k < n-1 ) and penalite( n-k, n, M, L ) < INFINI :
        k += 1

    nb_lignes_opt = 0
    nb_mots_opt = 0
    pen_min = INFINI
    for i in range( 1, n+1 ) :
        for j in range( n-k, n+1 ) :
            if W[i,j] < pen_min :
                pen_min = W[i,j]
                nb_lignes_opt = i
                nb_mots_opt = j

    # ``nb_mots_opt`` indique le nombre de mots dans une disposition optimale.
    # La différence avec ``n`` indique le nombre de mots à disposer sur la
    # dernière ligne. Si ce nombre est zéro, on ne l'inscrit pas car il ne sert
    # à rien d'ajouter une ligne vide à la fin.
    if n == nb_mots_opt :
        nb_mots_par_lignes = []
    else :
        nb_mots_par_lignes = [ n - nb_mots_opt ]
    # ``nb_mots_par_lignes`` est une liste contenant le nombre de mots à
    # disposer sur chacune des lignes. Cette liste est remplie à l'envers, de
    # la dernière à la première ligne.
    # On utilise la matrice N pour récupérer le nombre de mots à disposer sur
    # chacune des lignes.
    i = nb_lignes_opt
    j = nb_mots_opt
    while i > 0 :
        k = N[i,j]
        nb_mots_par_lignes.insert( 0, k )
        i -= 1
        j -= k
    assert( sum( nb_mots_par_lignes ) == n )
    return nb_mots_par_lignes



def afficher_lignes( L, mots_par_lignes, M ) :
    """
    Affiche les mots de la ListeDeMots ``L`` en fonction de la liste
    ``mots_par_lignes`` de manière à disposer ``mots_par_lignes[k]`` mots sur
    la k-ième ligne.
    ``M`` indique la largeur des lignes.
    """
    print " "
    debut = 1
    for k in mots_par_lignes[:-1] :
        s = L.disposer_sur_une_ligne( debut, debut + k -1, M )
        debut += k
        print "|" + s + " "*(M-len(s)) + "|"
    # la dernière ligne est affichée différemment
    k = mots_par_lignes[-1]
    mots_de_la_derniere_ligne = [ L.get_mot(i) + ' ' for i in range( debut, debut+k-1) ]
    mots_de_la_derniere_ligne += L.get_mot( debut+k-1 )
    map( lambda x : x+' ', mots_de_la_derniere_ligne[:-1] )
    s = ''.join( mots_de_la_derniere_ligne )
    print "|" + s + " "*(M-len(s)) + "|"



def usage( nom ) :
    print "Usage : {} [options]".format( nom )
    print "Affiche un texte sous la forme d'un paragraphe formaté"
    print "OPTIONS :"
    print "  -f fichier : formate le contenu du fichier spécifié."
    print "  -t \"texte\" : formate le texte spécifié, les \" ne sont pas optionnels."
    print "  -n Entier : formate une liste de mots aléatoires choisis" \
            + "dans le fichier {}".format( FICHIER_DICTIONNAIRE )
    print "  -N Entier : spécifie le nombre de caractères par lignes. "\
            + "Si -N n'est pas spécifié, la largeur du terminal est utilisée."



if __name__ == "__main__" :
    M = None # taille des lignes
    L = None # liste de mots

    # lecture des arguments
    k = 1
    while k < len( argv ) :
        if argv[k] == '-f' : 
            k += 1
            f = open( argv[k], 'r' )
            L = ListeDeMots( f.read() )
            f.close()
        elif argv[k] == '-t' :
            k += 1
            L = ListeDeMots( argv[k] )
        elif argv[k] == '-n' :
            k += 1
            L = ListeDeMots( int( argv[k] ) )
        elif argv[k] == '-N' :
            k += 1
            M = int( argv[k] )
        elif argv[k] in [ '-h', '-help', '--help' ] :
            usage( argv[0] )
            exit( 0 )
        else :
            print "Argument \"{}\" invalide".format( argv[k] )
            usage( argv[0] )
            exit( -1 )
        k += 1

    if L == None :
        # Aucune liste de mots spécifiée, on quitte.
        usage( argv[0] )
        exit( -1 )

    if M  == None :
        # Pas de valeur pour la taille des lignes, on récupère le nombre de
        # colonnes du terminal
        p = popen( "stty size", 'r' )
        nb_lignes, nb_cols = map( int, p.read().split() )
        M = nb_cols - 2
        p.close()

    print L
    print "Taille des lignes :", M
    print "Taille du mot le plus long :", L.lng_max()
    print "------------------------"
    if L.lng_max() > M :
        raise ValueError( "Taille des lignes insuffisantes", \
             "Taille du plus grand mot : {}, taille des lignes : {}.".format( \
              L.lng_max(), M ) )
    opt = meilleurDispo( L, M )
    print " "
    print "meilleure dispo :"
    print opt
    afficher_lignes( L, opt, M )
    exit( 0 )


