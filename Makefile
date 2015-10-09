all:
	gcc -Wall main.c hashtable.c -o chash
clean:
	rm -f chash
