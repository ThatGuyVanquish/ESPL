# All
all: task1c

task1c: task1c.o
	gcc -g -Wall -m32 -o task1c task1c.c

task1c.o: task1c.c
	gcc -g -Wall -m32 -o task1c.o task1c.c

.PHONY: clean
clean:
	rm -rf *.o task1c
