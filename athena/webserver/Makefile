cc = gcc

all:
	$(cc) -c parse.c
	$(cc) -c generate.c
	$(cc) -o webserver main.c parse.o generate.o

clean:
	rm -f *.o
	rm -f webserver
