SRC=simulateur.c
CC=gcc
LD=gcc
CFLAGS=-g -Wextra -Wall 
GTKCFLAGS:=-g $(shell pkg-config --cflags gtk+-2.0)
GTKLIBS:=$(shell pkg-config --libs gtk+-2.0)
LIBS:=$(GTKLIBS) -lm -lpthread

all: simulateur

simulateur: simulateur.o gravite.o
	$(LD) $(CFLAGS) $(GTKCFLAGS) simulateur.o gravite.o -o simulateur $(LIBS)

simulateur.o: simulateur.c gravite.h
	  $(CC) -c $(CFLAGS) $(GTKCFLAGS) simulateur.c

gravite.o: gravite.c gravite.h
	  $(CC) -c $(CFLAGS) gravite.c

clean:
	rm -f simulateur simulateur.o gravite.o

