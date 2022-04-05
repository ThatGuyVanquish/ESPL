# All Targets
all: my_echo

my_echo: my_echo.o
	gcc -g -Wall -m32 -o my_echo my_echo.o

# Dependancies:
my_echo.o: my_echo.c
	gcc -g -Wall -m32 -c -o my_echo.o my_echo.c

.PHONY: clean

# Clean the build directory
clean:
	rm -f *.o my_echo
