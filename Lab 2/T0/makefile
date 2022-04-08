# All

all: count-words

count-words: count-words.o
	gcc -g -Wall -m32 -o count-words count-words.c

# Dep
count-words.o: count-words.c
	gcc -g -Wall -m32 -o count-words.o count-words.c

.PHONY: clean

clean:
	rm -rf *.o count-words
