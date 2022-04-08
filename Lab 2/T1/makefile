# All
all: addresses

addresses: addresses.o
	gcc -g -Wall -m32 -o addresses addresses.c

addresses.o: addresses.c
	gcc -g -Wall -m32 -o addresses.o addresses.c

.PHONY: clean

clean:
	rm -rf *.o addresses
