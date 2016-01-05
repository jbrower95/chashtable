all:
	gcc -Wall -g main.c hashtable.c -o chash
clean:
	rm -f chash
