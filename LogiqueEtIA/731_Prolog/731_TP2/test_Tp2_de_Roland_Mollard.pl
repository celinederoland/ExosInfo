/*
	info 730 Prolog
	TP2
	Rémi Mollard
	Céline de Roland
	Quelques exemples d'appels à nos prédicats
*/

trip(a,z,X).
X = [a, b, c, z] ;
X = [a, c, z] ;

tempsCalc(345,[a,b,wait(2),c,wait(1),z],A,D).
A = [ (a, 345), (b, 346), (c, 349), (z, 351)],
D = [ (a, 345), (b, 348), (c, 350)] ;
false.

tempsCalc(346,[a, wait(3), b, c, z],A,D).
A = [ (a, 346), (b, 350), (c, 351), (z, 352)],
D = [ (a, 349), (b, 350), (c, 351)] ;
false.

temps(A,D).
A = [ (a, 345), (b, 346), (c, 349), (z, 351)],
D = [ (a, 345), (b, 348), (c, 350)] ;
A = [ (a, 346), (b, 350), (c, 351), (z, 352)],
D = [ (a, 349), (b, 350), (c, 351)] ;
false.

pasdecollision(tripFixe(346,[a, b, c, z])).
true

pasdecollision(tripFixe(346,[a, b, c, wait(1), z])).
true

pasdecollision(tripFixe(346,[a, c, z])).
true

pasdecollision(tripFixe(346,[a, c, wait(1), z])).
true

tous_trip(pasAttente,346,352,a,z,STrip,[a,b,c,z]).
STrip = [a, wait(1), b, wait(1), c, z] ;
STrip = [a, wait(1), b, wait(1), c, wait(1), z] ;
STrip = [a, wait(1), b, wait(2), c, z] ;
STrip = [a, wait(1), b, c, z] ;
STrip = [a, wait(1), b, c, wait(1), z] ;
STrip = [a, wait(1), b, c, wait(2), z] ;
STrip = [a, wait(2), b, wait(1), c, z] ;
STrip = [a, wait(2), b, c, z] ;
STrip = [a, wait(2), b, c, wait(1), z] ;
STrip = [a, wait(3), b, c, z] ;
STrip = [a, b, wait(1), c, z] ;
STrip = [a, b, wait(1), c, wait(1), z] ;
STrip = [a, b, wait(1), c, wait(2), z] ;
STrip = [a, b, wait(2), c, z] ;
STrip = [a, b, wait(2), c, wait(1), z] ;
STrip = [a, b, wait(3), c, z] ;
STrip = [a, b, c, z] ;
STrip = [a, b, c, wait(1), z] ;
STrip = [a, b, c, wait(2), z] ;
STrip = [a, b, c, wait(3), z] ;
false

tous_trip(pasAttente,346,352,a,z,STrip,[a,b,c,z]), pasdecollision(tripFixe(346,STrip)).
STrip = [a, b, c, z] ;
STrip = [a, b, c, wait(1), z] ;
false.

trip(346,352,a,z,Ts).
Ts = [a, b, c, z] ;
Ts = [a, b, c, wait(1), z] ;
Ts = [a, wait(1), c, z] ;
Ts = [a, wait(1), c, wait(1), z] ;
Ts = [a, wait(2), c, z] ;
Ts = [a, c, z] ;
Ts = [a, c, wait(1), z] ;
Ts = [a, c, wait(2), z] ;
false

tempsAbs([a, b, c, z],T).
3 ;
false
tempsAbs([a, wait(1), c, wait(1), z],T).
4 ;
false

tmin(STrip,[[a, b, c, z],[a, wait(1), c, wait(1), z]]).
[a,b,c,z] ;
false

trip_opt(346,352,a,z,Ts).
Ts = [a, c, z] ;
false.

