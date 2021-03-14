CC = gcc
CFLAGS =
RM = rm -fv

.PHONY : all clean

all: serveur.out client.out calculs.o

calculs.o: calculs.c calculs.h
	$(CC) $(CFLAGS) $< -o $@ -c

serveur.out: serveur.c calculs.o
	$(CC) $(CFLAGS) $^ -o $@

client.out: client.c calculs.o
	$(CC) $(CFLAGS) $^ -o $@

clean:
	$(RM) *.o *.out
