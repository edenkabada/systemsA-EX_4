all: graph	
	 
graph: main.o
	gcc -Wall -o graph main.o

main.o: main.c
	gcc -Wall -c main.c

clean:
	rm -f *.a *.o graph