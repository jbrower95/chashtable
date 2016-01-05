
all: hashtable tests
hashtable: hashtable.c
	gcc -Wall -g -O0 -c hashtable.c 	
tests: hashtable main.c
	gcc -Wall -g -O0 -c main.c -o tests.o
	gcc tests.o hashtable.o -o tests
save:
	git commit -a 
clean:
	rm -f chash
	rm -rf hashtable.o
	rm -rf tests.o
	rm -rf tests
	rm -rf *.dSYM
