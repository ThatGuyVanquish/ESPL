# ALL
all: flame2

flame2: start.o main.o util.o
	ld -m elf_i386 start.o main.o util.o -o flame2
	chmod +x flame2

start.o: start.s
	nasm -f elf32 start.s -o start.o

util.o: util.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector util.c -o util.o
	
main.o: flame2.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector flame2.c -o main.o
	
.PHONY: clean
clean: 
	rm -f *.o flame2
