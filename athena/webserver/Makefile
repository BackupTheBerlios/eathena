cc = gcc

all:
	#Generate framework
	$(cc) -c parse.c
	$(cc) -c generate.c
	
	#Generate "pages"
	cd pages && $(cc) -c about.c && cd ..
	cd pages && $(cc) -c sample.c && cd ..


	$(cc) -o webserver main.c parse.o generate.o pages/about.o \
	pages/sample.o
	



clean:
	rm -f *.o
	rm -f pages/*.o
	rm -f webserver
	
