# All
all: lab3

lab3: lab3.o
	gcc -g -Wall -m32 -o lab3 lab3.c

lab3.o: lab3.c
	gcc -g -Wall -m32 -o lab3.o lab3.c

.PHONY: clean
clean:
	rm -rf *.o lab3
