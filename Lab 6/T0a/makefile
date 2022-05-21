# ALl:
all: mypipe

mypipe: mypipe.o
	gcc -g -Wall -m32 -o mypipe mypipe.o

mypipe.o: mypipe.c
	gcc -g -Wall -m32 -c -o mypipe.o mypipe.c

.PHONY: clean

clean:
	rm -rf *.o mypipe