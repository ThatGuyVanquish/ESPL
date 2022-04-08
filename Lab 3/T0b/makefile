# All
all: hexPrint

hexPrint: hexPrint.o
	gcc -g -Wall -m32 -o hexPrint hexPrint.c

hexPrint.o: hexPrint.c
	gcc -g -Wall -m32 -o hexPrint.o hexPrint.c

.PHONY: clean
clean:
	rm -rf *.o hexPrint
