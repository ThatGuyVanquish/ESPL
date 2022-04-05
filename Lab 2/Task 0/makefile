# All:

all: countwords

countwords: count-words.o
	gcc -g -Wall -m32 -o count-words count-words.c

#Dependencies
count-words.o: count-words.c
	gcc -g -Wall -m32 -o count-words.o count-words.c

.PHONY: clean

clean:
	rm -f *.o countwords
