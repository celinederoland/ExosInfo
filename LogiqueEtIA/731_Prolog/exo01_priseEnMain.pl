aime(sylvie,programmer).
aime(sylvie,logique).
aime(jean,logique).
aime(paul,logique).
aime(jean,programmer).
aime(X,prolog) :- aime(X,programmer), aime(X,logique).
