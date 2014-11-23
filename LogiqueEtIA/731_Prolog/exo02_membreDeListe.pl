membre(X,[X|_]).
membre(X,[_|R]) :- membre(X,R).
