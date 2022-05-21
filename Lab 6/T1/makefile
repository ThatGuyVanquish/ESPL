# ALl:
all: mypipeline

mypipeline: mypipeline.o LineParser.o
	gcc -g -Wall -m32 -o mypipeline mypipeline.o LineParser.o

mypipeline.o: mypipeline.c
	gcc -g -Wall -m32 -c -o mypipeline.o mypipeline.c

LineParser.o: LineParser.c
	gcc -g -Wall -m32 -c -o LineParser.o LineParser.c

.PHONY: clean

clean:
	rm -rf *.o mypipeline