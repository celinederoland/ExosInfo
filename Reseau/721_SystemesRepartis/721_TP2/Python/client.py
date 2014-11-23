import xmlrpclib

server = xmlrpclib.ServerProxy("http://localhost:2009")
resultat = server.Moteur.calculer("3+5")
print resultat
