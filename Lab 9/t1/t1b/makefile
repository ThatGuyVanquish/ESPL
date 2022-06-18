# All
all: skeleton

skeleton: skeleton.o
	ld -m elf_i386 skeleton.o -o skeleton
	
skeleton.o: skeleton.s
	nasm -f elf32 -o skeleton.o skeleton.s
		
.PHONY: clean

clean:
	rm -f *.o skeleton
