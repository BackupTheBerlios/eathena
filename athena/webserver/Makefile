cc = gcc

all:
	$(cc) -c parse.c
	$(cc) -o webserver main.c parse.o

clean:
	rm -f *.o
	rm -f webserver
