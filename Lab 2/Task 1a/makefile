# All

all: addresses

addresses: addresses.c
	gcc -g -Wall -m32 -o addresses addresses.c

# Dependencies

addresses.o: addresses.c
	gcc -g -Wall -m32 -o addresses.o addresses.c

.PHONY: clean
clean:
	rm -rf *.o addresses
