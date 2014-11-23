CC=gcc
OPT=-Wall -Wextra -pedantic -g
LIB=-lpthread

all: genAleat
	$(CC) $(OPT) triRapide.c tri.c -o tri $(LIB)

genAleat: genAleat.c
	$(CC) $(OPT) genAleat.c -o genAleat

clean:
	rm -f tri genAleat aTester nbTriees nbAleat



