# ALL
all: menu_map

menu_map: menu_map.o
	gcc -g -Wall -m32 -o menu_map menu_map.c

# DEP
menu_map.o: menu_map.c
	gcc -g -Wall -m32 -o menu_map.o menu_map.c

.PHONY: clean

clean:
	rm -rf *.o menu_map
