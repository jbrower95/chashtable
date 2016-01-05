
all: hashtable tests
hashtable: hashtable.c
	gcc -Wall -g -O0 -c hashtable.c 	
tests: hashtable tests.c
	gcc -Wall -g -O0 -c tests.c -o tests.o
	gcc tests.o hashtable.o -o tests
hashtable-library:
	rm -rf hashtable.o
	gcc -c -fPIC -O3 hashtable.c -o hashtable.o
	gcc -shared -Wl,-soname,libhashtable.so.1 -o libhashtable.so.1.0.0 hashtable.o
save:
	git commit -a 
clean:
	rm -f chash
	rm -rf hashtable.o
	rm -rf tests.o
	rm -rf tests
	rm -rf *.dSYM
