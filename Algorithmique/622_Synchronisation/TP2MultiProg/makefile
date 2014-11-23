CC=gcc
LN=gcc
OPT=-Wall -Wextra -pedantic -g
LIB=-lrt -lpthread

all: 
	$(CC) $(OPT) -c monTube_exo4.c
	$(CC) $(OPT) -c monTube_exo5.c
	$(CC) $(OPT) -c monTube_exo6.c
	$(CC) $(OPT) -c tp2.c
	$(LN) tp2.o monTube_exo4.o monTube_exo5.o monTube_exo6.o -o tp2 $(LIB)

clean:
	rm -f tp2 monTube_exo4.o monTube_exo5.o monTube_exo6.o tp2.o


