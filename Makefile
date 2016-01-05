
all: hashtable tests
hashtable:
	gcc -Wall -g -O -c hashtable.c 	
tests: hashtable
	gcc -Wall -g -c main.c -o tests.o
	gcc tests.o hashtable.o -o tests
save:
	git commit -a 
clean:
	rm -f chash
	rm -rf hashtable.o
	rm -rf tests.o
	rm -rf tests
	rm -rf *.dSYM
