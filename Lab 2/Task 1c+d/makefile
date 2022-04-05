# ALL
all: main

main: main.o
	gcc -g -Wall -m32 -o main.o task1c.c

# DEP
main.o: task1c.c
	gcc -g -Wall -m32 -o main.o task1c.c

.PHONY: clean

clean:
	rm -rf *.o main
