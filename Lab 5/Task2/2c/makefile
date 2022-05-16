# all:
all: myshell

myshell: myshell.o LineParser.o
	gcc -g -Wall -m32 -o myshell myshell.o LineParser.o

myshell.o: myshell.c
	gcc -g -Wall -m32 -c -o myshell.o myshell.c

LineParser.o: LineParser.c
	gcc -g -Wall -m32 -c -o LineParser.o LineParser.c

.PHONY: clean

clean:
	rm -f *.o myshell