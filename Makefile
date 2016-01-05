

hashtable:
	gcc -Wall -g -O -c hashtable.c 	
tests: hashtable
	gcc -Wall -g main.c -O hashtable.o -o tests
all: hashtable tests
clean:
	rm -f chash
	rm -rf hashtable.o
	rm -rf tests.o
