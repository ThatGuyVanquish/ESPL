# ALl
all: mergesort

mergesort: mergesort.o
	gcc -g -Wall -m32 -o mergesort mergesort.c

mergesort.o: mergesort.c
	gcc -g -Wall -m32 -o mergesort.o mergesort.c

.PHONY: clean
clean:
	rm -rf *.o mergesort
