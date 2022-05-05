# ALL
all: count

count: start.o main.o util.o
	ld -m elf_i386 start.o main.o util.o -o count
	chmod +x count

start.o: start.s
	nasm -f elf32 start.s -o start.o

util.o: util.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector util.c -o util.o
	
main.o: count1a.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector count1a.c -o main.o
	
.PHONY: clean
clean: 
	rm -f *.o count
