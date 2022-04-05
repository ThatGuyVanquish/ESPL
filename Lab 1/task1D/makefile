# All:

all: encoder

encoder: encoder.o
	gcc -g -Wall -m32 -o encoder encoder.c

#Dependencies
encoder.o: encoder.c
	gcc -g -Wall -m32 -o encoder.o encoder.c

.PHONY: clean

clean:
	rm -f *.o encoder
