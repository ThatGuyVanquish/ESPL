# All Targets
all: main

main: main.o numbers.o add.o
	gcc -g -Wall -m32 -o main main.o numbers.o add.o

# Depends on the source and header files
main.o: main.c
	gcc -g -Wall -m32 -c -o main.o main.c

numbers.o: numbers.c
	gcc -g -Wall -m32 -c -o numbers.o numbers.c

add.o: add.s
	nasm -g -f elf -w+all -o add.o add.s

.PHONY: clean

# Clean the build directory
clean: 
	rm -f *.o main
