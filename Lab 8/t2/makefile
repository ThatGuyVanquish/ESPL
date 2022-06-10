# All
all: myELF

myELF: myELF.o
	gcc -m32 -g -Wall -o myELF myELF.o

myELF.o: t2.c
	gcc -g -Wall -m32 -c -o myELF.o t2.c

.PHONY: clean
clean: 
	rm -f *.o myELF
